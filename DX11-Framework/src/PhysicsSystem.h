#pragma once
#include "GameObject.h"

class PhysicsSystem
{
private:
	static PhysicsSystem* m_instance;
public:
	static PhysicsSystem* GetInstance();
	bool CollCheckSphereSphere(GameObject* lhs, GameObject* rhs);
	void CollResSphereSphere(GameObject* lhs, GameObject* rhs);
private:
	PhysicsSystem();
	~PhysicsSystem();
};

