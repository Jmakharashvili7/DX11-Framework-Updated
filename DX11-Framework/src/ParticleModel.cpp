#include "ParticleModel.h"

#include <cmath>

#include "Transform.h"
#include "Log.h"
#include "KeyboardClass.h"

ParticleModel::ParticleModel(GameObject* parent, float mass) :
	m_Velocity(0.0f, 0.0f, 0.0f),
	m_parent(parent),
	m_Acceleration(0.5f),
	m_NetForce(0.0f, 0.0f, 0.0f),
	m_Mass(mass),
	m_transform(nullptr),
	m_MaxVelocity(1.5f, 1.5f, 1.5f)
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
	m_parent->GetTransform()->SetPosition(new Vector3(*position + (m_NetForce / m_Mass) * dt));
}

void ParticleModel::HandleInput(const float dt, const unsigned int key)
{
	switch (key)
	{
	case VK_UP:
		if (KeyboardClass::IsKeyPressed(VK_UP))
		{
			if (m_Velocity.z < m_MaxVelocity.z)
			{
				m_Velocity.z += m_Acceleration;
			}
		}
		break;
	case VK_DOWN:
		if (KeyboardClass::IsKeyPressed(VK_DOWN))
		{
			if (m_Velocity.z > (m_MaxVelocity.z * -1))
			{
				m_Velocity.z -= m_Acceleration;
			}
		}
		break;
	case VK_RIGHT:
		if (KeyboardClass::IsKeyPressed(VK_RIGHT))
		{
			if (m_Velocity.x < m_MaxVelocity.x)
			{
				m_Velocity.x += m_Acceleration;
			}
		}
		break;
	case VK_LEFT:
		if (KeyboardClass::IsKeyPressed(VK_LEFT))
		{
			if (m_Velocity.x > (m_MaxVelocity.x * -1))
			{
				m_Velocity.x -= m_Acceleration;
			}
		}
		break;
	case VK_SPACE:
		if (KeyboardClass::IsKeyPressed(VK_SPACE))
		{
			if (m_Velocity.y < m_MaxVelocity.y)
			{
				m_Velocity.y += m_Acceleration;
			}
		}
		break;
	case VK_SHIFT:
		if (KeyboardClass::IsKeyPressed(VK_SHIFT))
		{
			if (m_Velocity.y > (m_MaxVelocity.y * -1))
			{
				m_Velocity.y -= m_Acceleration;
			}
		}
		break;
	}
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
