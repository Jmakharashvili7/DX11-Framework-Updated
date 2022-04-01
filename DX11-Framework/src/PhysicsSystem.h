#pragma once
#include "GameObject.h"

class PhysicsSystem
{
private:
	static PhysicsSystem* m_instance;
public:
	static PhysicsSystem* GetInstance();
	bool CollisionCheck(GameObject* lhs, GameObject* rhs);
private:
	PhysicsSystem();
	~PhysicsSystem();
};

