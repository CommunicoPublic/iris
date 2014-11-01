/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "IPv4Map.hpp"

#include <stdio.h>
#include <sysexits.h>

using namespace IRIS;

void PrintResult(IPMap::State oState)
{
	fprintf(stdout, "%s\n", oState == IPMap::OK ? "OK" : "ERROR");
}

int main(int argc, char ** argv)
{
	IPv4Map oIPv4Map;
	IPMap & oIPMap = oIPv4Map;

	fprintf(stdout, "127.0.0.1/8: ");
	PrintResult(oIPMap.AddNet("127.0.0.1/8"));

	fprintf(stdout, "10/8: ");
	PrintResult(oIPMap.AddNet("10/8"));
	fprintf(stdout, "172.16/12: ");
	PrintResult(oIPMap.AddNet("172.16/12"));
	fprintf(stdout, "192.168/16: ");
	PrintResult(oIPMap.AddNet("192.168/16"));

	fprintf(stdout, "192.168.10/24: ");
	PrintResult(oIPMap.AddNet("192.168.10/24"));
	fprintf(stdout, "172.30/16: ");
	PrintResult(oIPMap.AddNet("172.30/16"));

	fprintf(stdout, "8.8.8.8: ");
	PrintResult(oIPMap.CheckIP("8.8.8.8"));
	fprintf(stdout, "127.0.0.1: ");
	PrintResult(oIPMap.CheckIP("127.0.0.1"));
	fprintf(stdout, "127.0.0.2: ");
	PrintResult(oIPMap.CheckIP("127.0.0.2"));
	fprintf(stdout, "10.2.2.2: ");
	PrintResult(oIPMap.CheckIP("10.2.2.2"));

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
