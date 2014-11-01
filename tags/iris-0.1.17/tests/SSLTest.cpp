/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "SSL.hpp"
#include "LoggerFile.hpp"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

int main(int argc, char ** argv)
{
	if (argc != 4)
	{
		fprintf(stderr, "usage: %s certificate.pem certificate.key certificate.dh\n", argv[0]);
		return EX_USAGE;
	}

	// Create logger object
	Logger * pLogger = new LoggerFile(stderr);
	pLogger -> SetPriority(LogPriority::LOG_DEBUG);

	// SSL
	SSLCore oSSLCore;
	SSL_CTX * pSSLContext = oSSLCore.AddContext(argv[1], argv[2], argv[3], "ALL:!ADH:!EXPORT56:RC4+RSA:+HIGH:+MEDIUM:+LOW:+SSLv3:+EXP", "communico.pro", false, false, *pLogger);
	if (pSSLContext == NULL)
	{
		pLogger -> Emerg("Can't add certificate from files `%s` `%s`", argv[1], argv[2]);
		return EX_SOFTWARE;
	}

	SSL_CTX * pContext = oSSLCore.GetContext("communico.pro", "");
	if (pContext == NULL)
	{
		pLogger -> Emerg("Can't get certificate from files `%s` `%s`", argv[1], argv[2]);
		return EX_SOFTWARE;
	}

	if (pSSLContext != pContext)
	{
		pLogger -> Emerg("Can't check certificate from files `%s` `%s`", argv[1], argv[2]);
		return EX_SOFTWARE;
	}

	// Destroy object
	delete pLogger;

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
