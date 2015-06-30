/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes

#include "TimeProfiler.hpp"

#include <stdio.h>
#include <sysexits.h>
#include <unistd.h>

using namespace IRIS;

int main(int argc, char ** argv)
{
	TimeProfiler oTimeProfiler;

	W_FLOAT dTimeDelta = 0.0;

	oTimeProfiler.StartEvent("usleep(10)");
	usleep(10);
	dTimeDelta = oTimeProfiler.EndEvent("usleep(10)");
	fprintf(stderr, "usleep(10): %.*G\n", 12, dTimeDelta);

	oTimeProfiler.StartEvent("usleep(100)");
	usleep(100);
	dTimeDelta = oTimeProfiler.EndEvent("usleep(100)");
	fprintf(stderr, "usleep(100): %.*G\n", 12, dTimeDelta);

	oTimeProfiler.StartEvent("usleep(1000)");
	usleep(1000);
	dTimeDelta = oTimeProfiler.EndEvent("usleep(1000)");
	fprintf(stderr, "usleep(1000): %.*G\n", 12, dTimeDelta);

	oTimeProfiler.StartEvent("usleep(10000)");
	usleep(10000);
	dTimeDelta = oTimeProfiler.EndEvent("usleep(10000)");
	fprintf(stderr, "usleep(10000): %.*G\n", 12, dTimeDelta);

	oTimeProfiler.StartEvent("usleep(100000)");
	usleep(100000);
	dTimeDelta = oTimeProfiler.EndEvent("usleep(100000)");
	fprintf(stderr, "usleep(100000): %.*G\n", 12, dTimeDelta);

	oTimeProfiler.StartEvent("usleep(1000000)");
	usleep(1000000);
	dTimeDelta = oTimeProfiler.EndEvent("usleep(1000000)");
	fprintf(stderr, "usleep(1000000): %.*G\n", 12, dTimeDelta);

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
