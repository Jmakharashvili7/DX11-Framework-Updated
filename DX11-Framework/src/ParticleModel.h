#pragma once
#include "GameObject.h"

class ParticleModel
{
private:
	float m_Acceleration, m_Mass;
	Vector3 m_Velocity, m_NetForce;
	Transform* m_transform;
	GameObject* m_parent;
public:
	ParticleModel(GameObject* parent, float mass);
	~ParticleModel();

	void Update(const float dt);
	void MoveRight(const float dt);
	void MoveConstAcceleration(const float dt);

	// Get and set for velocity 
	inline Vector3 GetVelocity() const { return m_Velocity; }
	inline void SetVelocity(Vector3 velocity) { m_Velocity = velocity; }

	// Get and set for acceleration
	inline float GetAcceleration() const { return m_Acceleration; }
	inline void SetAcceleration(float acceleration) { m_Acceleration = acceleration; }

	// Get and set for mass
	inline float GetMass() const { return m_Mass; }
	inline void SetMass(float mass) { m_Mass = mass; }

	// Update net force
	void UpdateNetForce();
};

