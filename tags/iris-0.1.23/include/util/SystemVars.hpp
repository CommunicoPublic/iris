/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _SYSTEM_VARS_HPP__
#define _SYSTEM_VARS_HPP__ 1

/**
  @file SystemVars.hpp
  @brief Various system and OS-dependent variables
*/

// Local includes
#include "STLString.hpp"
#include "Types.h"

namespace IRIS
{

/**
  @class SystemVars SystemVars.hpp <SystemVars.hpp>
  @brief Get system variables
*/
class SystemVars
{
public:
	struct Time
	{
		UINT_64     sec;
		UINT_64     nsec;
	};

	struct SystemUser
	{
		INT_64        uid;
		INT_64        gid;
		STLW::string  homedir;
	};

	/**
	  @brief Get system time
	*/
	static Time GetTime();

	/**
	  @brief Get host name
	*/
	static STLW::string GetHostname();

	/**
	  @brief Sleep
	*/
	static UINT_64 Sleep(const UINT_64 iMicroseconds);

	/**
	  @brief Get unix error code
	*/
	static UINT_32 ErrNo();

	/**
	  @brief Get system user data
	*/
	static SystemUser GetSystemUserData(CCHAR_P  szUserName);

	/**
	  @brief Set user Id
	*/
	static INT_64 SetUID(INT_64  iUserId);

	/**
	  @brief Get user Id
	*/
	static INT_64 GetUID(CCHAR_P  szUserName = NULL);

	/**
	  @brief Set user Id
	*/
	static INT_64 SetGID(INT_64  iGroupId);

	/**
	  @brief Get user Id
	*/
	static INT_64 GetGID(CCHAR_P  szGroupName);

	/**
	  @brief Get group name
	*/
	static STLW::string GetGroupNam(const UINT_64  iGroupId);

private:

};

} // namespace IRIS
#endif // _SYSTEM_VARS_HPP__
// End.
