/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "LoggerFile.hpp"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

int main(int argc, char ** argv)
{
	FILE * F = stdout;
	if (argc == 2)
	{
		F = fopen(argv[1], "w");
		if (F == NULL)
		{
			fprintf(stderr, "FATAL: Can't open `%s` for writing\n", argv[1]);
			return EX_SOFTWARE;
		}
	}

	// Create logger object
	Logger * pLogger = new LoggerFile(F);

	pLogger -> Emerg("Emerg");
	pLogger -> Alert("Alert");
	pLogger -> Crit("Critical");
	pLogger -> Err("Error");
	pLogger -> Error("Error");
	pLogger -> Warn("Warning");
	pLogger -> Warning("Warning");
	pLogger -> Notice("Warning");
	pLogger -> Info("Info");
	pLogger -> Debug("Debug");

	pLogger -> SetPriority(LogPriority::LOG_DEBUG);

	pLogger -> Emerg("Emerg");
	pLogger -> Alert("Alert");
	pLogger -> Crit("Critical");
	pLogger -> Err("Error");
	pLogger -> Error("Error");
	pLogger -> Warn("Warning");
	pLogger -> Warning("Warning");
	pLogger -> Notice("Warning");
	pLogger -> Info("Info");
	pLogger -> Debug("Debug");

	// Destroy object
	delete pLogger;

	if (argc == 2) { fclose(F); }

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
