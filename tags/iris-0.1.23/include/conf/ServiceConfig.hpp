/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SERVICE_CONFIG_HPP__
#define _SERVICE_CONFIG_HPP__ 1

/**
  @file ServiceConfig.hpp
  @brief Iris network service configuration data
*/

#include "IPv4Map.hpp"
//#include "IPv6Map.hpp"
#include "ObjectFactory.hpp"
#include "TCPSocket.hpp"

namespace IRIS
{
// FWD
class Object;
class Logger;

/**
  @class ServiceConfig ServiceConfig.hpp <ServiceConfig.hpp>
  @brief Iris network service configuration data
*/
struct ServiceConfig
{
	enum State { OK, ERROR, CONFIG_ERROR };
	enum AllowDenyOrder { ALLOW_DENY, DENY_ALLOW };

	struct ListenInterface
	{
		/** Address family       */
		TCPSocket::AddressFamily   af;
		/** Address              */
		STLW::string               address;
		/** Port or socket mode  */
		UINT_64                    port;
	};

	/** Service name   */
	STLW::string                   name;

	/** inet:host:port
	    inet6:[ipv6host]:port
	    unix:/path/to/the/unix.socket:mode */
	STLW::vector<ListenInterface>  listen;
	/** Allow connection from specified IP's or subnets */
	IPv4Map                        ipv4_allowed_networks;
	/** Deny connection from specified IP's or subnets  */
	IPv4Map                        ipv4_denied_networks;
	/** Order of checking networks                      */
	AllowDenyOrder                 allow_deny_order;
	/** Number of active connections for this service   */
	UINT_32                        active_clients;
	/** Maximum number of connections for this service  */
	UINT_32                        max_clients;
	/** Read/Write timeout                              */
	W_FLOAT                        io_timeout;
	/** Socket listen queue                             */
	UINT_32                        listen_queue;
	/** Buffer for data                                 */
	UINT_32                        recv_buffer_size;
#ifdef IRIS_TLS_SUPPORT
	/** Files with the RSA certificate in PEM format.   */
	STLW::vector<STLW::string>     tls_cert_file;
	/** Files with the RSA private key in PEM format    */
	STLW::vector<STLW::string>     tls_key_file;
	/** File with the RSA private key in PEM format     */
	STLW::string                   dh_file;
	/** List of algorithms                              */
	STLW::string                   ciphers;
	/** Enable SSL/TLS support                          */
	bool                           enable_ssl_tls;
	/** Use Transport Level Security at connection time */
	bool                           use_tls;
	/** Enable STARTLS                                  */
	bool                           start_tls;
	/** Enable STARTLS                                  */
	bool                           prefer_server_ciphers;
#endif // IRIS_TLS_SUPPORT
	/** Handler name   */
	STLW::string                   handler_name;
	/** Handler object */
	Object                       * handler;

	/**
	  @brief Get configuration from variant data type
	*/
	static State  GetConfig(const VariantNC  & oData,
	                        ServiceConfig    & oConfig,
	                        Logger           & oLogger);

	/**
	  @brief parse list of networks
	*/
	static State ParseNetworks(const VariantNC     & oData,
	                           const STLW::string  & sBranch,
	                           ServiceConfig       & oServiceConfig,
	                           IPv4Map             & oIPv4Map,
	                           Logger              & oLogger);
};

} // namespace IRIS
#endif // _SERVICE_CONFIG_HPP__
// End.
