#pragma once
#include "GameObject.h"

class PhysicsSystem
{
private:
	static PhysicsSystem* m_instance;
	Vector3 m_gravity;
	bool m_gravityEnabled;
public:
	static PhysicsSystem* GetInstance();

	bool CollCheckSphereSphere(GameObject* lhs, GameObject* rhs);
	void CollResSphereSphere(GameObject* lhs, GameObject* rhs);

	inline void EnableGravity() { if (m_gravityEnabled) m_gravityEnabled = false; else m_gravityEnabled = true; }
	inline bool GetGravityState() { return m_gravityEnabled; }

	void ApplyGravity(std::vector<GameObject*> gameObjects, float deltaTime);
private:
	PhysicsSystem();
	~PhysicsSystem();
};

