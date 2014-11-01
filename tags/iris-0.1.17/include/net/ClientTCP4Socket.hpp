/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _TCP4_SOCKET_HPP__
#define _TCP4_SOCKET_HPP__ 1

#include "ClientTCPSocket.hpp"

namespace IRIS
{

/**
  @class ClientTCP4Socket TCP4Socket.hpp <TCP4Socket.hpp>
  @brief IPv4 client socket implementation
*/
class ClientTCP4Socket:
  public ClientTCPSocket
{
public:
	/**
	  @brief Constructor
	  @param szIHost - host name or IP address
	  @param iIPort - port
	*/
	ClientTCP4Socket(CCHAR_P        szIHost,
	                 const UINT_32  iIPort);

	/**
	  @brief Get client IP
	*/
	CCHAR_P GetIP() const;

	/**
	  @brief Get client Port
	*/
	UINT_32 GetPort() const;

	/**
	  @brief Open connection to the server
	  @return OK - if success, ERROR - if some error occured, TIMEOUT - if timeout reached
	*/
	TCPSocket::State Open();

	/**
	  @brief Connect to the server
	  @return OK - if success, ERROR - if some error occured, TIMEOUT - if timeout reached
	*/
	TCPSocket::State Connect();

	/**
	  @brief Disconnect from server
	  @return OK, always success
	*/
	TCPSocket::State Close();

	/**
	  @brief Clone socket descriptor
	*/
	INT_32 CloneSocket();

	/**
	  @brief Get address family
	  @return One of { INET, INET6, UNIX }
	*/
	AddressFamily GetAddressFamily() const;

	/**
	  @brief A destructor
	*/
	virtual ~ClientTCP4Socket() throw();

private:
	// Does not exist
	ClientTCP4Socket(const ClientTCP4Socket & oRhs);
	ClientTCP4Socket & operator=(const ClientTCP4Socket & oRhs);

	/** Server Hostname      */
	CHAR_P        szHost;
	/** Server IP            */
	CHAR_P        szIP;
	/** Port                 */
	UINT_32       iPort;
	/** Close socket at exit */
	bool          bCloseAtExit;
};

} // namespace IRIS
#endif // _TCP4_SOCKET_HPP__
// End.
