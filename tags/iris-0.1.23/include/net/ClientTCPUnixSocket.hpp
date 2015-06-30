/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */
#ifndef _CLIENT_TCP_UNIX_SOCKET_HPP__
#define _CLIENT_TCP_UNIX_SOCKET_HPP__ 1

#include "ClientTCPSocket.hpp"

namespace IRIS
{

/**
  @class ClientTCPUnixSocket TCP4Socket.hpp <TCP4Socket.hpp>
  @brief IPv4 client socket implementation
*/
class ClientTCPUnixSocket:
  public ClientTCPSocket
{
public:
	/**
	  @brief Constructor
	  @param szIHost - host name or IP address
	  @param iIPort - port
	*/
	ClientTCPUnixSocket(CCHAR_P        szIPath,
	                    const UINT_32  iIMode);

	/**
	  @brief Get client IP
	*/
	CCHAR_P GetPath() const;

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
	virtual ~ClientTCPUnixSocket() throw();

private:
	// Does not exist
	ClientTCPUnixSocket(const ClientTCPUnixSocket & oRhs);
	ClientTCPUnixSocket & operator=(const ClientTCPUnixSocket & oRhs);

protected:
	/** Socket path          */
	CHAR_P        szPath;
	/** Socvket mode         */
	UINT_32       iMode;
	/** Close socket at exit */
	bool          bCloseAtExit;
};

} // namespace IRIS
#endif // _CLIENT_TCP_UNIX_SOCKET_HPP__
// End.
