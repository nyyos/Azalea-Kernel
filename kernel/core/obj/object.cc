#include <obj/object.h>

namespace core
{

Object::Object()
{
	refcnt = 1;
}

Object::~Object()
{
}

void Object::release()
{
	release(0);
}

void Object::release(int freeWhen)
{
	refcnt.fetch_sub(1);
	if (refcnt.load() == freeWhen)
		free();
}

void Object::free() {
	delete this;
}

};
