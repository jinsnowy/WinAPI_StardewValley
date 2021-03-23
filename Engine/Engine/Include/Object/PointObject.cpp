#include "PointObject.h"
PointObject::PointObject()
{
}

PointObject::~PointObject()
{
}

PointObject::PointObject(PointObject& obj)
	:Object(obj)
{
}

bool PointObject::Init()
{
	return true;
}

PointObject* PointObject::Clone()
{
	return new PointObject(*this);
}
