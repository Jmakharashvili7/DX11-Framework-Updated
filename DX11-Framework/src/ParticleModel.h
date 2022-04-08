#pragma once
#include "GameObject.h"

#define TERMINAL_VECOCITY -15.0f

class ParticleModel
{
private:
	float m_Acceleration, m_Mass, m_Restitution = 1.0f;
	Vector3 m_Velocity, m_MaxVelocity, m_NetForce, m_ExternalForce, m_Gravity, m_Friction;
	Transform* m_transform;
	GameObject* m_parent;
	BoundingSphere* m_BoundSphere;
public:
	ParticleModel(GameObject* parent, float mass);
	~ParticleModel();

	void Update(const float dt);
	void HandleInput(const float dt, const unsigned int key);

	void ApplyForce(Vector3 force);
	void ApplyGravity(Vector3 gravity);

	// Get and set for velocity 
	inline Vector3 GetVelocity() const { return m_Velocity; }
	inline void SetVelocity(Vector3 velocity) { m_Velocity = velocity; }

	// Get and set for acceleration
	inline float GetAcceleration() const { return m_Acceleration; }
	inline void SetAcceleration(float acceleration) { m_Acceleration = acceleration; }

	// Get and set for mass
	inline float GetMass() const { return m_Mass; }
	inline void SetMass(float mass) { m_Mass = mass; }

	// Get and set bounding Sphere
	inline BoundingSphere* GetBoundSphere() const { return m_BoundSphere; }
	inline void SetBoundSphere(BoundingSphere* boundSphere) { m_BoundSphere = boundSphere; }

	// Get and set coefficient of restitution
	inline float GetRestitution() { return m_Restitution; }
	inline void SetRestitution(float restitution) { m_Restitution = restitution; }


private:
	// Update net force
	void UpdateNetForce();

	// Calculate and apply friction
	void ApplyFriction();

	// Move object at a constant acceleration
	void MoveConstAcceleration(const float dt);

	// Respond to collision
	void OnCollision(GameObject* other);
};

