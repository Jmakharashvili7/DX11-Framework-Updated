#pragma once
#include "GameObject.h"

class ParticleModel
{
private:
	XMFLOAT3 m_velocity;
	Transform* m_transform;
	GameObject* m_parent;
public:
	ParticleModel(GameObject* parent);
	~ParticleModel();

	void Update(const float dt);
	void MoveConstVelocity(const float dt);
};

