/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#include "ConfigHelper.hpp"
#include "Logger.hpp"
#include "ServiceConfig.hpp"
#include "UnicodeUtils.hpp"

namespace IRIS
{
ServiceConfig::State ServiceConfig::ParseNetworks(const VariantNC     & oData,
                                                  const STLW::string  & sBranch,
                                                  ServiceConfig       & oServiceConfig,
                                                  IPv4Map             & oIPv4Map,
                                                  Logger              & oLogger)
{
	const STLW::vector<STLW::string> vNetworks = oData[sBranch];
	STLW::vector<STLW::string>::const_iterator itvNetworks = vNetworks.begin();
	while (itvNetworks != vNetworks.end())
	{
		// IPv4
		if (itvNetworks -> find(".") != STLW::string::npos)
		{
			if (oIPv4Map.AddNet(*itvNetworks) != IPMap::OK)
			{
				oLogger.Emerg("Invalid format of parameter `Service/%s/%s`: invalid network `%s`", oServiceConfig.name.c_str(), sBranch.c_str(), itvNetworks -> c_str());
				return CONFIG_ERROR;
			}
		}
		// IPv6, TBD
		else if (itvNetworks -> find(":") != STLW::string::npos)
		{
			oLogger.Emerg("Invalid format of parameter `Service/%s/%s`: IPv6 networks `%s` not supported yet", oServiceConfig.name.c_str(), sBranch.c_str(), itvNetworks -> c_str());
		}

		++itvNetworks;
	}
return OK;
}

//
// Get configuration from variant data type
//
ServiceConfig::State ServiceConfig::GetConfig(const VariantNC  & oData,
                                              ServiceConfig    & oServiceConfig,
                                              Logger           & oLogger)
{
	// Allow connection from specified IP's or subnets
	if (ParseNetworks(oData, "AllowConnect", oServiceConfig, oServiceConfig.ipv4_allowed_networks, oLogger) != OK)
	{
		return CONFIG_ERROR;
	}

	// Deny connection from specified IP's or subnets
	if (ParseNetworks(oData, "DenyConnect", oServiceConfig, oServiceConfig.ipv4_denied_networks, oLogger) != OK)
	{
		return CONFIG_ERROR;
	}

	// Order of networks check
	oServiceConfig.allow_deny_order = ALLOW_DENY;

	const STLW::vector<STLW::string> vDenyAllow = oData["AllowDenyOrder"];
	if (!vDenyAllow.empty() && vDenyAllow[0] != "")
	{
		if (vDenyAllow.size() != 2)
		{
		 	oLogger.Emerg("Invalid format of parameter `Service/%s/AllowDenyOrder`: need `allow deny` or `deny allow`", oServiceConfig.name.c_str());
		 	return CONFIG_ERROR;
		}

		if (Unicode::CompareIgnoreCase(vDenyAllow[0].data(), vDenyAllow[0].size(), "deny", sizeof("deny") - 1) == 0)
		{
			oServiceConfig.allow_deny_order = DENY_ALLOW;
		}
	}

	// Listen to inteface
	STLW::vector<STLW::string> vListen;
	ConfigHelper::GetList(oData["Listen"], vListen);
	STLW::vector<STLW::string>::const_iterator itvListen = vListen.begin();
	while(itvListen != vListen.end())
	{
		ListenInterface  oInterface;
		ConfigHelper::State oRC = ConfigHelper::ParseAddress(*itvListen, oInterface.af, oInterface.address, oInterface.port);
		if (oRC != ConfigHelper::OK)
		{
			oLogger.Emerg("Invalid address: `%s`", itvListen -> c_str());
			return CONFIG_ERROR;
		}
		oServiceConfig.listen.push_back(oInterface);
		++itvListen;
	}
	// Socket listen queue
	oServiceConfig.listen_queue = oData["MaxQueueLength"];
	if (oServiceConfig.listen_queue == 0)
	{
		oLogger.Emerg("Invalid format of parameter `Service/%s/MaxQueueLength` not set", oServiceConfig.name.c_str());
		return CONFIG_ERROR;
	}

	// Read buffer size
	oServiceConfig.recv_buffer_size = ConfigHelper::ParseSize(oData["RecvBufferSize"]);
	if (oServiceConfig.recv_buffer_size == 0 || oServiceConfig.recv_buffer_size == UINT_32(-1))
	{
		oLogger.Emerg("Invalid format of parameter `Service/%s/RecvBufferSize` not set", oServiceConfig.name.c_str());
		return CONFIG_ERROR;
	}

	//  Maximum number of connections for this service
	oServiceConfig.max_clients    = oData["MaxClients"];
	if (oServiceConfig.max_clients == 0) { oLogger.Warn("Parameter `Service/%s/MaxClients`: not set", oServiceConfig.name.c_str()); }

	// Number of active clients
	oServiceConfig.active_clients = 0;

	// Read/Write timeout
	oServiceConfig.io_timeout = ConfigHelper::ParseTimeout(oData["IOTimeout"]) / 1000.0;
	if (oServiceConfig.io_timeout == 0)
	{
		oLogger.Emerg("Parameter `Service/%s/IOTimeout`: need to set positive value", oServiceConfig.name.c_str());
		return CONFIG_ERROR;
	}

#ifdef IRIS_TLS_SUPPORT
	// TLS parameters
	oServiceConfig.enable_ssl_tls = ConfigHelper::ParseBool(oData["EnableTLSSSL"]);
	if (oServiceConfig.enable_ssl_tls)
	{
		// File with the RSA certificate in PEM format.
		oServiceConfig.tls_cert_file = oData["TLSCertFile"];
		// File with the RSA private key in PEM format
		oServiceConfig.tls_key_file  = oData["TLSKeyFile"];
		// File with the DH data
		oServiceConfig.dh_file.assign(oData["DHFile"]);
		// SSL Ciphers
		oServiceConfig.ciphers.assign(oData["SSLCiphers"]);

		if (oServiceConfig.tls_cert_file.size() != oServiceConfig.tls_key_file.size())
		{
			oLogger.Emerg("Number of files `Service/%s/TLSCertFile` and Service/%s/TLSKeyFile` must be equal", oServiceConfig.name.c_str());
			return CONFIG_ERROR;
		}

		// Use Transport Level Security at connection time
		oServiceConfig.use_tls               = ConfigHelper::ParseBool(oData["UseTLS"]);
		// Allow upgrade an existing insecure connection to a secure connection using SSL/TLS
		oServiceConfig.start_tls             = ConfigHelper::ParseBool(oData["StartTLS"]);
		// Prefer to user server ciphers
		oServiceConfig.prefer_server_ciphers = ConfigHelper::ParseBool(oData["PreferServerCiphers"], true);
	}
#endif // IRIS_TLS_SUPPORT

	// Handler name
	oServiceConfig.handler_name.assign(oData["Handler"]);
	// Handler object
	oServiceConfig.handler = NULL;

return OK;
}

} // namespace IRIS
// End.
