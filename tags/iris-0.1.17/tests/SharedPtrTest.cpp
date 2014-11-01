/*-
 *  Copyright (c) Iris Dev. team. All rights reserved.
 *  See http://www.communico.pro/license for details.
 *
 */

// C++ Includes
#include "SharedPtr.hpp"

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
	SharedPtr<Foo> pFoo = new Foo;
	pFoo -> Bar();
	(*pFoo).Bar();

	SharedPtr<Foo> pFoo2 = pFoo;
	SharedPtr<Foo> pFoo3 = pFoo;

return EX_OK;
}
// End.
