/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _CONFIG_HELPER_HPP__
#define _CONFIG_HELPER_HPP__ 1

/**
  @file ConfigHelper.hpp
  @brief Iris configuration helpers
*/

#include "TCPSocket.hpp"
#include "Variant.hpp"
#include "STLSet.hpp"

namespace IRIS
{
// FWD
class Logger;
class MainConfig;

/**
  @class ConfigHelper ConfigHelper.hpp <ConfigHelper.hpp>
  @brief Confuguration parser helpers
*/
class ConfigHelper
{
public:
	enum State { OK, ERROR, CONFIG_ERROR, NOT_NEED, NO_SUCH_USER, NO_SUCH_GROUP };

	/**
	  @brief Parse bool flag
	  @param sFlag - flag representation, case-insensitive ("On", "Yes" or "Enabled" == true, and false = otherwise)
	  @param bDefaultValue - set default value if sFlag i empty string
	  @return Parsed value or 0
	*/
	static bool ParseBool(const STLW::string  & sFlag,
	                      const bool            bDefaultValue = false);

	/**
	  @brief Parse timeout
	  @param sTimeout - timeout in following format: 1ms 1s 1min or 1h
	  @return Parsed value or 0
	*/
	static INT_64 ParseTimeout(const STLW::string  & sTimeout);

	/**
	  @param sSize - size in following format: 1 1k 1m or 1g
	  @return Parsed value or 0
	*/
	static INT_64 ParseSize(const STLW::string  & sSize);

	/**
	  @brief Parse IPv6, IPv4 and local network address
	  @param sSource - source string
	  @param oAF - Protocol family
	  @param sAddress - IPv6, IPv4 or locan unix address [out]
	  @param iPort - port or unix socket mode [out]
	  @return Parser state
	*/
	static State ParseAddress(const STLW::string        & sSource,
	                          TCPSocket::AddressFamily  & oAF,
	                          STLW::string              & sAddress,
	                          UINT_64                   & iPort);

	/**
	  @brief Parse user and group name
	*/
	static State ParseUserGroup(const VariantNC  & oUserGroup,
	                            STLW::string     & sUser,
	                            INT_64           & iUID,
	                            STLW::string     & sGroup,
	                            INT_64           & iGID);

	/**
	  @brief Parse configuration file
	  @return Parser state
	  @param sConfigFile - configuration file
	  @param vIncludeFiles - list of includes
	  @param oConfigData - configuration data [out]
	  @param oLogger - logger object
	  @return Parser state
	*/
	static State ParseConfigFile(const STLW::string          & sConfigFile,
	                             STLW::vector<STLW::string>  & vIncludeFiles,
	                             STLW::set<STLW::string>     & sSections,
	                             VariantNC                   & oConfigData,
	                             Logger                      & oLogger);

	/**
	  @brief Get list of strings from config file
	  @param oVariantData - Source data
	  @param vStringList - list of strings [out]
	*/
	static void GetList(const VariantNC             & oVariantData,
	                    STLW::vector<STLW::string>  & vStringList);

	/**
	  @brief Get list of directories from config file
	  @param oVariantData - Source data
	  @param vDirList - list of directories [out]
	*/
	static void GetDirList(const VariantNC             & oVariantData,
	                       STLW::vector<STLW::string>  & vDirList);
};

} // namespace IRIS
#endif // _CONFIG_HELPER_HPP__
// End.

