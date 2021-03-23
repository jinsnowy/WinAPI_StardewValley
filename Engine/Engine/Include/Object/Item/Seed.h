#pragma once
#include "Item.h"
class Seed :
    public Item
{
	friend class Object;
protected:
	Seed();
	Seed(const Seed& item);
	virtual ~Seed();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual Seed* Clone();
private:
	string m_strPlantTag;
public:
	void SetPlantTag(const string& strTag)
	{
		m_strPlantTag = strTag;
	}
	string GetPlantTag() const
	{
		return m_strPlantTag;
	}
	class Plant* SpawnPlant() const;

};

