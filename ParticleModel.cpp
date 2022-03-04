#include "ParticleModel.h"
#include "Transform.h"

ParticleModel::ParticleModel(GameObject* parent) :
	m_transform(m_parent->GetTransform()),
	m_velocity(0.0f, 0.0f, 0.0f),
	m_parent(parent)
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

}
