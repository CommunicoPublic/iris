/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "FileHandleCache.hpp"
#include "FileUtils.hpp"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

using namespace IRIS;

int main(int argc, char ** argv)
{
	{
		FileHandleCache oCache(3);

		INT_64 iFile = 0;
		for (UINT_32 iPos = 0; iPos < 3; ++iPos)
		{
			iFile = oCache.Open("test.txt", OpenMode::READWRITE | OpenMode::CREATE);
		}

		for (UINT_32 iPos = 0; iPos < 3; ++iPos)
		{
			oCache.Close(iFile);
		}

		for (UINT_32 iPos = 0; iPos < 3; ++iPos)
		{
			iFile = oCache.Open("test.txt", OpenMode::WRITE | OpenMode::CREATE);
			oCache.Close(iFile);
		}

		for (UINT_32 iPos = 0; iPos < 3; ++iPos)
		{
			iFile = oCache.Open("test.txt", OpenMode::READ | OpenMode::CREATE);
			oCache.Close(iFile);
		}

		for (UINT_32 iPos = 0; iPos < 3; ++iPos)
		{
			iFile = oCache.Open("test.txt", OpenMode::WRITE);
			oCache.Close(iFile);
		}

		for (UINT_32 iPos = 0; iPos < 3; ++iPos)
		{
			iFile = oCache.Open("test.txt", OpenMode::READWRITE | OpenMode::CREATE);
			oCache.Close(iFile);
		}
	}
	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
