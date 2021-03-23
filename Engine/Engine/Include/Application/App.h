#pragma once
#include "../framework.h"
class App
{
public:
	App();
	~App();
	void Init();
	int Go();
private:
	void Process();
	void Input(float dt);
	int Update(float dt);
	int LateUpdate(float dt);
	void Collision(float dt);
	void Draw(float dt);
};

