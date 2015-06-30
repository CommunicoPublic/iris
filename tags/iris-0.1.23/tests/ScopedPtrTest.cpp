/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "ScopedPtr.hpp"

#include <stdio.h>
#include <sysexits.h>

using namespace IRIS;

class Foo
{
public:
	Foo() { fprintf(stderr, "Foo::Foo %p\n", (void*) this); }

	void Bar() { fprintf(stderr, "Foo::Bar %p\n", (void*) this); }

	~Foo() { fprintf(stderr, "Foo::~Foo %p\n\n", (void*) this); }
private:
};

int main(int argc, char ** argv)
{
	ScopedPtr<Foo> pFoo;
	for (UINT_32 iCount = 0; iCount < 3; ++iCount)
	{
		pFoo = new Foo;

		pFoo -> Bar();
		(*pFoo).Bar();
	}

	ScopedPtr<Foo> pFoo3(new Foo);

return EX_OK;
}
// End.
