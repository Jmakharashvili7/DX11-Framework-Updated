#pragma once
#include "GameObject.h"

class ParticleModel
{
private:
	float m_Acceleration;
	Vector3 m_velocity;
	Transform* m_transform;
	GameObject* m_parent;
public:
	ParticleModel(GameObject* parent);
	~ParticleModel();

	void Update(const float dt);
	void MoveConstVelocity(const float dt);
	void MoveConstAcceleration(const float dt);

	// Get and set for velocity 
	inline Vector3 GetVelocity() const { return m_Velocity; }
	inline void SetVelocity(vector3 velocity) { m_Velocity = velocity; }

	// Get and set for acceleration
	inline float GetAcceleration() const { return m_Acceleration; }
	inline void SetAcceleration(float acceleration) { m_Acceleration = acceleration; }
};

