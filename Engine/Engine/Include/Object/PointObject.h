#pragma once
#include "Object.h"
class PointObject :
    public Object
{
	friend class Object;
private:
	PointObject();
	virtual ~PointObject();
	PointObject(PointObject& obj);
public:
	virtual bool Init();
	virtual PointObject* Clone();
};
