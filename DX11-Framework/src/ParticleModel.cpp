#include "ParticleModel.h"
#include "Transform.h"
#include "Log.h"

ParticleModel::ParticleModel(GameObject* parent) :
	m_velocity(1.0f, 0.0f, 0.0f),
	m_parent(parent),
	m_Acceleration(0.0f)
{
}

ParticleModel::~ParticleModel()
{

}

void ParticleModel::Update(const float dt)
{
}

void ParticleModel::MoveConstVelocity(const float dt)
{
	Vector3* position = m_parent->GetTransform()->GetPosition();
	m_parent->GetTransform()->SetPosition(new Vector3(*position + m_velocity * dt));
}

void ParticleModel::MoveConstAcceleration(const float dt)
{
	Vector3* position = m_parent->GetTransform()->GetPosition();
	m_parent->GetTransform()->SetPosition(new Vector3(*position + m_velocity * dt));
}
