#pragma once
#include "GameObject.h"

#define TERMINAL_VELOCITY -10.0f

class ParticleModel
{
private:
	bool m_Grounded, m_Laminar, m_InFluid = false;
	float m_Mass, m_Restitution = 1.0f, m_Speed, m_DragFactor = 0.47f;
	Vector3 m_Velocity, m_MaxVelocity, m_NetForce, m_ExternalForce, m_Gravity, m_Friction, m_Acceleration, m_Drag;
	Transform* m_transform;
	GameObject* m_parent;
	BoundingSphere* m_BoundSphere;
public:
	ParticleModel(GameObject* parent, float mass, bool grounded = true);
	~ParticleModel();

	void Update(const float dt);
	void HandleInput(const float dt, const unsigned int key);

	void ApplyForce(Vector3 force);
	void ApplyGravity(Vector3 gravity);

	// Get and set for velocity 
	inline Vector3 GetVelocity() const { return m_Velocity; }
	inline void SetVelocity(Vector3 velocity) { m_Velocity = velocity; }

	// Get and set for acceleration
	inline Vector3 GetAcceleration() const { return m_Acceleration; }
	inline void SetAcceleration(Vector3 acceleration) { m_Acceleration = acceleration; }

	// Get and set for mass
	inline float GetMass() const { return m_Mass; }
	inline void SetMass(float mass) { m_Mass = mass; }

	// Get and set bounding Sphere
	inline BoundingSphere* GetBoundSphere() const { return m_BoundSphere; }
	inline void SetBoundSphere(BoundingSphere* boundSphere) { m_BoundSphere = boundSphere; }

	// Get and set coefficient of restitution
	inline float GetRestitution() { return m_Restitution; }
	inline void SetRestitution(float restitution) { m_Restitution = restitution; }

	inline bool GetGrounded() { return m_Grounded; }
	inline void SetGrounded(bool grounded) { m_Grounded = grounded; }

private:
	// Update net force
	void UpdateNetForce();

	// Update Acceleration
	void UpdateAccel();

	void UpdateState(const float dt);

	// function for moving the object
	void Move(const float dt);

	// Move object at a constant acceleration
	void MoveConstAcceleration(const float dt);

	void MotionInFluid(const float dt);
	void DragForce();
	void DragLamForce();
	void DragTurbFlow();

	// Respond to collision
	void OnCollision(GameObject* other);
};

