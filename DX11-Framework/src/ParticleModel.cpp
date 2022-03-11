#include "ParticleModel.h"
#include "Transform.h"
#include "Log.h"

ParticleModel::ParticleModel(GameObject* parent, float mass) :
	m_Velocity(0.0f, 0.0f, 0.0f),
	m_parent(parent),
	m_Acceleration(1.0f),
	m_NetForce(0.0f, 0.0f, 0.0f),
	m_Mass(mass),
	m_transform(nullptr)
{
}

ParticleModel::~ParticleModel()
{

}

void ParticleModel::Update(const float dt)
{
	// Reset netforce
	m_NetForce.Zero();
	UpdateNetForce();

	Vector3* position = m_parent->GetTransform()->GetPosition();
	m_parent->GetTransform()->SetPosition(new Vector3(*position + m_NetForce * dt));
}

void ParticleModel::MoveRight(const float dt)
{
	m_Velocity.x = m_Acceleration;
}

void ParticleModel::MoveConstAcceleration(const float dt)
{
	Vector3* position = m_parent->GetTransform()->GetPosition();
	m_parent->GetTransform()->SetPosition(new Vector3(*position + m_Velocity * dt));
}

void ParticleModel::UpdateNetForce()
{
	m_NetForce += m_Velocity;
}
