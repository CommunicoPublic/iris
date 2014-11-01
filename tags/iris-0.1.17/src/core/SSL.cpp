/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "SSL.hpp"

#ifdef IRIS_TLS_SUPPORT

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/safestack.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/engine.h>

#include "STLSet.hpp"
#include "Logger.hpp"
#include "ServerClientTCP4SSLSocket.hpp"

namespace IRIS
{
#ifndef OPENSSL_NO_TLSEXT
//
// Switch the context of the current SSL object to the most appropriate one based on the SNI header
//
int SSLCore::SwitchSNIContext(SSL * pSSL, int * al, void * vThis)
{
	CCHAR_P szServerName = SSL_get_servername(pSSL, TLSEXT_NAMETYPE_host_name);
	if (!szServerName) { return SSL_TLSEXT_ERR_NOACK; }

	ContextMap * pContextMap = (ContextMap *)vThis;

	SSL_CTX * pContext = pContextMap -> ssl_core -> GetContext(pContextMap -> ident, szServerName);
	if (pContext != NULL) { SSL_set_SSL_CTX(pSSL, pContext); }

return SSL_TLSEXT_ERR_NOACK;
}
#endif // OPENSSL_NO_TLSEXT

#ifndef OPENSSL_NO_DH
static unsigned char aDH1024_p[] =
{
	0x30, 0x81, 0x87, 0x02, 0x81, 0x81, 0x00, 0x89, 0x7F, 0x7C, 0x5C, 0x58, 0x42, 0x5D, 0x8C, 0xE0,
	0x16, 0x13, 0x0E, 0x54, 0x1D, 0xE8, 0xE3, 0x37, 0x9E, 0x13, 0xA8, 0xC2, 0x21, 0x2E, 0x34, 0x6D,
	0x46, 0x2E, 0xC3, 0xDE, 0xD3, 0x20, 0x17, 0x44, 0xB9, 0x78, 0x0D, 0x4F, 0x4F, 0xF7, 0x17, 0x4F,
	0x0A, 0x42, 0xEB, 0x11, 0xE3, 0xC4, 0x85, 0xF5, 0xF2, 0xA8, 0x5A, 0x0F, 0xAD, 0x05, 0xFE, 0xD3,
	0xD7, 0xFF, 0xD9, 0x17, 0x0E, 0x17, 0x58, 0xC4, 0xEB, 0xBE, 0x46, 0xD8, 0xA3, 0xEF, 0x35, 0x96,
	0x4E, 0xA1, 0xBD, 0xC3, 0x0A, 0x44, 0x11, 0x79, 0xD4, 0x16, 0xDF, 0xDE, 0x29, 0xDB, 0x08, 0x6B,
	0x1E, 0xB8, 0x35, 0xCD, 0xD1, 0x18, 0x58, 0xC0, 0xE8, 0xA7, 0x78, 0x72, 0x42, 0xD9, 0x21, 0xDF,
	0x45, 0xAE, 0x80, 0xE2, 0x5A, 0xEE, 0xB5, 0x72, 0xE4, 0x26, 0x2C, 0x72, 0xA1, 0xD0, 0x58, 0x88,
	0xB4, 0x02, 0xFE, 0x71, 0x56, 0xBB, 0x47, 0x02, 0x01, 0x05
};
static unsigned char aDH1024_g[] = { 0x02 };

#endif // OPENSSL_NO_DH

//
// Execute this function on SSL handshake and trap client-initiated renegotiations.
//
void SSLCore::SSLInfoCallbak(const SSL * pSSL, int iWhere, int iRet)
{
	if (iWhere & SSL_CB_HANDSHAKE_START)
	{
		ServerClientTCP4SSLSocket * pSSLSocket = (ServerClientTCP4SSLSocket *)SSL_get_app_data(pSSL);
		if (pSSLSocket -> Handshaked())
		{
			pSSLSocket -> Renegotiate();
		}
	}
}

//
// SSL error callback
//
int SSLCore::SSLErrorCallbck(const char * szString, size_t iStringLen, void * vThis)
{
	SSLCore * pThis = (SSLCore *)vThis;

	pThis -> sError.append(szString, iStringLen);

return 0;
}

//
// Initialization flag
//
UINT_64 SSLCore::iRefcount = 0;

//
// Mutex
//
Mutex   SSLCore::oMutex;

//
// Initialize SSL engine
//
void SSLCore::InitEngine()
{
	MutexLocker oLocker(oMutex);

	++iRefcount;
	if (iRefcount != 1) { return; }

	CRYPTO_malloc_init();

	ERR_load_crypto_strings();

	SSL_load_error_strings();
	SSL_library_init();

	ENGINE_load_builtin_engines();
	ENGINE_register_all_complete();

	OpenSSL_add_all_algorithms();
	OPENSSL_load_builtin_modules();
}

//
// Shut down SSL engine
//
void SSLCore::DestroyEngine()
{
	MutexLocker oLocker(oMutex);

	--iRefcount;
	if (iRefcount != 0) { return; }

	CONF_modules_unload(1);
	EVP_cleanup();
	ENGINE_cleanup();
	ERR_remove_state(0);

	CRYPTO_cleanup_all_ex_data();
	ERR_free_strings();
#ifndef __FreeBSD__
	ERR_remove_thread_state(NULL);
#endif
	EVP_cleanup();
}


//
// Constructor
//
SSLCore::SSLCore()
{
	InitEngine();
}

//
// Add context
//
SSL_CTX * SSLCore::AddContext(const STLW::string  & sCertFile,
                              const STLW::string  & sKeyFile,
                              const STLW::string  & sDHFile,
                              const STLW::string  & sSSLCipherList,
                              const STLW::string  & sIdent,
                              const bool            bUseTLS,
                              const bool            bPreferServerCiphers,
                              Logger              & oLogger)
{
	ServiceContextMap::iterator itmServiceContextMap = mServiceContextMap.find(sIdent);
	if (itmServiceContextMap == mServiceContextMap.end())
	{
		ContextMap * pContextMap = new ContextMap;
		pContextMap -> ssl_core = this;
		pContextMap -> ident    = sIdent;

		itmServiceContextMap = mServiceContextMap.insert(STLW::pair<STLW::string, ContextMap *>(sIdent, pContextMap)).first;
	}
	ContextMap * pContextMap = itmServiceContextMap -> second;

	STLW::set<STLW::string> oServerNames;

	SSL_CTX  * pSSLContext = NULL;

	if (bUseTLS) { pSSLContext = SSL_CTX_new(TLSv1_server_method()); }
	else         { pSSLContext = SSL_CTX_new(SSLv23_server_method()); }

	if (pSSLContext == NULL)
	{
		oLogger.Error("SSL context %s: SSL_CTX_new failed", sIdent.c_str());
		return NULL;
	}

	long iSSLOptions = SSL_OP_NO_SSLv2 | SSL_OP_ALL | SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION;
#ifdef SSL_OP_NO_COMPRESSION
	iSSLOptions |= SSL_OP_NO_COMPRESSION;
#endif
	SSL_CTX_set_options(pSSLContext, iSSLOptions);
	SSL_CTX_set_info_callback(pSSLContext, SSLCore::SSLInfoCallbak);

	if (sSSLCipherList.empty())
	{
		oLogger.Info("SSL context %s: Using ciphers `%s`", sIdent.c_str(), SSL_DEFAULT_CIPHER_LIST);
		SSL_CTX_set_cipher_list(pSSLContext, SSL_DEFAULT_CIPHER_LIST);
	}
	else
	{
		oLogger.Info("SSL context %s: Using ciphers `%s`", sIdent.c_str(), sSSLCipherList.c_str());
		if (SSL_CTX_set_cipher_list(pSSLContext, sSSLCipherList.c_str()) != 1)
		{
			sError.erase();
			ERR_print_errors_cb(SSLCore::SSLErrorCallbck, this);
			oLogger.Error("SSL context %s: invalid cipher list `%s`: %s", sIdent.c_str(), sSSLCipherList.c_str(), sError.c_str());
			SSL_CTX_free(pSSLContext);
			return NULL;
		}
	}

	if (bPreferServerCiphers) { SSL_CTX_set_options(pSSLContext, SSL_OP_CIPHER_SERVER_PREFERENCE); }

	if (SSL_CTX_use_certificate_chain_file(pSSLContext, sCertFile.c_str()) <= 0)
	{
		sError.erase();
		ERR_print_errors_cb(SSLCore::SSLErrorCallbck, this);
		oLogger.Error("SSL context %s: (cert file `%s`): %s", sIdent.c_str(), sCertFile.c_str(), sError.c_str());
		SSL_CTX_free(pSSLContext);
		return NULL;
	}

	BIO  * pBIO = BIO_new_file(sKeyFile.c_str(), "r");
	if (!pBIO)
	{
		sError.erase();
		ERR_print_errors_cb(SSLCore::SSLErrorCallbck, this);
		oLogger.Error("SSL context %s: (key file `%s`): %s", sIdent.c_str(), sKeyFile.c_str(), sError.c_str());
		SSL_CTX_free(pSSLContext);
		return NULL;
	}

	RSA * pRSA = PEM_read_bio_RSAPrivateKey(pBIO, NULL, pSSLContext -> default_passwd_callback, pSSLContext -> default_passwd_callback_userdata);
	BIO_free(pBIO);

	if (!pRSA)
	{
		sError.erase();
		ERR_print_errors_cb(SSLCore::SSLErrorCallbck, this);
		oLogger.Error("SSL context %s: (key file `%s`): %s", sIdent.c_str(), sKeyFile.c_str(), sError.c_str());
		SSL_CTX_free(pSSLContext);
		return NULL;
	}

	if (SSL_CTX_use_RSAPrivateKey(pSSLContext, pRSA) <= 0)
	{
		sError.erase();
		ERR_print_errors_cb(SSLCore::SSLErrorCallbck, this);
		oLogger.Error("SSL context %s: (key file `%s`): %s", sIdent.c_str(), sKeyFile.c_str(), sError.c_str());
		RSA_free(pRSA);
		SSL_CTX_free(pSSLContext);
		return NULL;
	}

#ifndef OPENSSL_NO_DH
	DH  * pDH = NULL;
	if (sDHFile.empty())
	{
		oLogger.Info("SSL context %s: Using built-in DH parameters", sIdent.c_str());
		pDH = DH_new();
		if (pDH == NULL)
		{
			oLogger.Error("SSL context %s: Can't create DH", sIdent.c_str());
			RSA_free(pRSA);
			SSL_CTX_free(pSSLContext);
			return NULL;
		}

		pDH -> p = BN_bin2bn(aDH1024_p, sizeof(aDH1024_p), NULL);
		pDH -> g = BN_bin2bn(aDH1024_g, sizeof(aDH1024_g), NULL);
		if ((pDH -> p == NULL) || (pDH -> g == NULL))
		{
			oLogger.Error("SSL context %s: Can't set DH data", sIdent.c_str());
			RSA_free(pRSA);
			SSL_CTX_free(pSSLContext);
			return NULL;
		}
	}
	else
	{
		oLogger.Info("SSL context %s: Using DH parameters from file `%s`", sIdent.c_str(), sDHFile.c_str());
		pBIO = BIO_new_file(sDHFile.c_str(), "r");
		if (!pBIO)
		{
			sError.erase();
			ERR_print_errors_cb(SSLCore::SSLErrorCallbck, this);
			oLogger.Error("SSL context %s: (dh file `%s`): %s", sIdent.c_str(), sDHFile.c_str(), sError.c_str());
			RSA_free(pRSA);
			SSL_CTX_free(pSSLContext);
			return NULL;
		}

		pDH = PEM_read_bio_DHparams(pBIO, NULL, NULL, NULL);
		BIO_free(pBIO);
		if (!pDH)
		{
			sError.erase();
			ERR_print_errors_cb(SSLCore::SSLErrorCallbck, this);
			oLogger.Error("SSL context %s: (dh file `%s`): %s", sIdent.c_str(), sDHFile.c_str(), sError.c_str());
			RSA_free(pRSA);
			SSL_CTX_free(pSSLContext);
			return NULL;
		}
	}
	SSL_CTX_set_tmp_dh(pSSLContext, pDH);

	oLogger.Info("SSL context %s: DH key length is %u bit", sIdent.c_str(), DH_size(pDH) * 8);
	DH_free(pDH);

#ifndef OPENSSL_NO_EC
#ifdef NID_X9_62_prime256v1
	EC_KEY * pECKey = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	SSL_CTX_set_tmp_ecdh(pSSLContext, pECKey);
	EC_KEY_free(pECKey);
	oLogger.Info("SSL context %s: ECDH initialized with NIST P-256", sIdent.c_str());

#endif // NID_X9_62_prime256v1
#endif // OPENSSL_NO_EC
#endif // OPENSSL_NO_DH

#ifndef OPENSSL_NO_TLSEXT
	if (!SSL_CTX_set_tlsext_servername_callback(pSSLContext, SSLCore::SwitchSNIContext))
	{
		oLogger.Error("SSL context %s: Can't set up servername callback", sIdent.c_str());
		RSA_free(pRSA);
		SSL_CTX_free(pSSLContext);
		return NULL;
	}
	 SSL_CTX_set_tlsext_servername_arg(pSSLContext, pContextMap);

#endif // OPENSSL_NO_TLSEXT
	RSA_free(pRSA);

	pBIO = BIO_new(BIO_s_file());
	// TODO: error checking
	if (!BIO_read_filename(pBIO, sCertFile.c_str()))
	{
		sError.erase();
		ERR_print_errors_cb(SSLCore::SSLErrorCallbck, this);
		oLogger.Error("SSL context %s: (cert file `%s`): %s", sIdent.c_str(), sCertFile.c_str(), sError.c_str());
		SSL_CTX_free(pSSLContext);
	}

	X509 * pX509 = PEM_read_bio_X509_AUX(pBIO, NULL, NULL, NULL);
	BIO_free(pBIO);

	STACK_OF(GENERAL_NAME) * pNames = (STACK_OF(GENERAL_NAME) *)X509_get_ext_d2i(pX509, NID_subject_alt_name, NULL, NULL);

	// Subject Alternative Names
	for (INT_32 iPos = 0; iPos < sk_GENERAL_NAME_num(pNames); ++iPos)
	{
		GENERAL_NAME * pName = sk_GENERAL_NAME_value(pNames, iPos);
		if (pName -> type == GEN_DNS)
		{
			UCHAR_P szServerName = NULL;
			ASN1_STRING_to_UTF8(&szServerName, pName -> d.dNSName);

			oServerNames.insert((CHAR_P)szServerName);

			OPENSSL_free(szServerName);
		}
	}

	if (sk_GENERAL_NAME_num(pNames) > 0)
	{
		sk_GENERAL_NAME_pop_free(pNames, GENERAL_NAME_free);
	}
	else if (pNames != NULL)
	{
		sk_GENERAL_NAME_pop_free(pNames, GENERAL_NAME_free);
	}

	X509_NAME * pX509Name = X509_get_subject_name(pX509);
	INT_32 iPos = X509_NAME_get_index_by_NID(pX509Name, NID_commonName, -1);
	if (iPos < 0)
	{
		oLogger.Warn("SSL context %s: (cert file `%s`): Could not find Subject Alternative Names or a CN", sIdent.c_str(), sCertFile.c_str());
	}
	else
	{
		X509_NAME_ENTRY * pX509Entry = X509_NAME_get_entry(pX509Name, iPos);

		UCHAR_P szServerName = NULL;
		ASN1_STRING_to_UTF8(&szServerName, pX509Entry -> value);

		oServerNames.insert((CHAR_P)szServerName);

		OPENSSL_free(szServerName);
	}
	X509_free(pX509);

	STLW::set<STLW::string>::const_iterator itoServerNames = oServerNames.begin();
	while (itoServerNames != oServerNames.end())
	{
		if (pContextMap -> server_names.find(*itoServerNames) != pContextMap -> server_names.end())
		{
			SSL_CTX_free(pSSLContext);
			oLogger.Warn("SSL context %s: (cert file `%s`): Duplicate server name (`%s`) found", sIdent.c_str(), sCertFile.c_str(), itoServerNames -> c_str());
			return NULL;
		}
		++itoServerNames;
	}

	// All done, push context
	vContextes.push_back(pSSLContext);

	itoServerNames = oServerNames.begin();
	while (itoServerNames != oServerNames.end())
	{
		oLogger.Notice("SSL context %s: (cert file `%s`): Adding server `%s`", sIdent.c_str(), sCertFile.c_str(), itoServerNames -> c_str());
		pContextMap -> server_names[*itoServerNames] = vContextes.size() - 1;
		++itoServerNames;
	}

return pSSLContext;
}

//
// Get context
//
SSL_CTX  * SSLCore::GetContext(const STLW::string  & sIdent,
                               const STLW::string  & sHostname)
{
	ServiceContextMap::const_iterator itmServiceContextMap = mServiceContextMap.find(sIdent);
	if (itmServiceContextMap == mServiceContextMap.end()) { return NULL; }

	const ContextMap * pContextMap = itmServiceContextMap -> second;

	ServerNames::const_iterator itmContextMap =  pContextMap -> server_names.begin();
	if (!sHostname.empty()) { itmContextMap = pContextMap -> server_names.find(sHostname); }

	if (itmContextMap == pContextMap -> server_names.end()) { return NULL; }

return vContextes[itmContextMap -> second];
}

//
// A destructor
//
SSLCore::~SSLCore() throw()
{
	STLW::vector<SSL_CTX *>::iterator itvContextes = vContextes.begin();
	while (itvContextes != vContextes.end())
	{
		SSL_CTX_free(*itvContextes);
		++itvContextes;
	}

	ServiceContextMap::iterator itmServiceContextMap = mServiceContextMap.begin();
	while (itmServiceContextMap != mServiceContextMap.end())
	{
		delete itmServiceContextMap -> second;
		++itmServiceContextMap;
	}

	DestroyEngine();
}

} // namespace IRIS
#endif // IRIS_TLS_SUPPORT
// End.
