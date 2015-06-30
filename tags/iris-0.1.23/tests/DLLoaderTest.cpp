/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "DLLibraryLoader.hpp"
#include "LoadableObject.hpp"

#include <stdio.h>
#include <sysexits.h>

using namespace IRIS;

int main(int argc, char ** argv)
{
	Loader<Object>  oLoader("_module_init");

	oLoader.LoadLibrary("mod_test_library.so");

	Object * pObject = oLoader.GetObject("mod_test_library.so", "TestLibrary");
	if (pObject == NULL)
	{
		fprintf(stderr, "Loading failed!\n");
		return EX_SOFTWARE;
	}

	fprintf(stdout, "Type: %s\n", pObject -> GetObjectType());
	fprintf(stdout, "Name: %s\n", pObject -> GetObjectName());

	delete pObject;

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

return EX_OK;
}
// End.
