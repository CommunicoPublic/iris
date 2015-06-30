/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SSL_HPP__
#define _SSL_HPP__ 1

/**
  @file SSH.hpp
  @brief SSL subsystem
*/
#include "Types.h"
#include "STLMap.hpp"
#include "STLVector.hpp"
#include "STLString.hpp"

#include "Mutex.hpp"

#ifdef IRIS_TLS_SUPPORT

typedef struct ssl_ctx_st SSL_CTX;
typedef struct ssl_st SSL;

namespace IRIS
{
// FWD
class Logger;

/**
  @class NetworkHandler NetworkHandler.hpp <NetworkHandler.hpp>
  @brief Base class class for network handlers
*/
class SSLCore
{
public:
	enum State { OK, ERROR };

	/**
	  @brief Initialize SSL engine
	*/
	static void InitEngine();

	/**
	  @brief Initialize SSL engine
	*/
	static void DestroyEngine();

	/**
	  @brief Constructor
	*/
	SSLCore();

	/**
	  @brief Add SSL context
	*/
	SSL_CTX * AddContext(const STLW::string  & sCertFile,
	                     const STLW::string  & sKeyFile,
	                     const STLW::string  & sDHFile,
	                     const STLW::string  & sSSLCipherList,
	                     const STLW::string  & sIdent,
	                     const bool            bUseTLS,
	                     const bool            bPreferServerCiphers,
	                     Logger              & oLogger);

	/**
	  @brief Get SSL context by name
	*/
	SSL_CTX  * GetContext(const STLW::string  & sIdent,
	                      const STLW::string  & sHostname);

	/**
	  @brief A destructor
	*/
	~SSLCore() throw();
private:
	// Does not exist
	SSLCore(const SSLCore  & oRhs);
	SSLCore& operator=(const SSLCore  & oRhs);

	typedef STLW::map<STLW::string, UINT_32>    ServerNames;

	struct ContextMap
	{
		/** Pointer to self      */
		SSLCore       * ssl_core;
		/** List of server names */
		ServerNames     server_names;
		/** Service name         */
		STLW::string    ident;
	};

	typedef STLW::map<STLW::string, ContextMap *> ServiceContextMap;

	/** Mutex                  */
	static Mutex             oMutex;
	/** Initialization flag    */
	static UINT_64           iRefcount;

	/** SSL contextes          */
	STLW::vector<SSL_CTX *>  vContextes;
	/** ContextMap             */
	ServiceContextMap        mServiceContextMap;
	/** Error description      */
	STLW::string             sError;

	/**
	  @breif SSL error callback
	*/
	static int SSLErrorCallbck(const char * szString, size_t iStringLen, void * vThis);

	/**
 	  @brief Switch the context of the current SSL object to the most appropriate one based on the SNI header
	*/
	static int SwitchSNIContext(SSL * pSSL, int * al, void * vThis);


	/**
	  @brief Execute this function on SSL handshake and trap client-initiated renegotiations.
	*/
	static void SSLInfoCallbak(const SSL * pSSL, int iWhere, int iRet);
};

} // namespace IRIS
#endif // IRIS_TLS_SUPPORT
#endif // _SSL_HPP__
// End.
