#include "ParticleModel.h"

#include <cmath>

#include "Transform.h"
#include "Log.h"
#include "KeyboardClass.h"

ParticleModel::ParticleModel(GameObject* parent, float mass, bool grounded) :
	m_Velocity(0.0f, 0.0f, 0.0f),
	m_parent(parent),
	m_Speed(1.0f),
	m_NetForce(0.0f, 0.0f, 0.0f),
	m_Mass(mass),
	m_transform(nullptr),
	m_MaxVelocity(8.5f, 8.5f, 8.5f),
	m_BoundSphere(new BoundingSphere()),
	m_Gravity(0.0f, 0.0f, 0.0f),
	m_Friction(0.0f, 0.0f, 0.0f),
	m_Grounded(grounded)
{
	m_BoundSphere->Diameter = m_parent->GetTransform()->GetScale()->x;
}

ParticleModel::~ParticleModel()
{
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
				m_Velocity.z += m_Speed;
			}
		}
		break;
	case VK_DOWN:
		if (KeyboardClass::IsKeyPressed(VK_DOWN))
		{
			if (m_Velocity.z > (m_MaxVelocity.z * -1))
			{
				m_Velocity.z -= m_Speed;
			}
		}
		break;
	case VK_RIGHT:
		if (KeyboardClass::IsKeyPressed(VK_RIGHT))
		{
			if (m_Velocity.x < m_MaxVelocity.x)
			{
				m_Velocity.x += m_Speed;
			}
		}
		break;
	case VK_LEFT:
		if (KeyboardClass::IsKeyPressed(VK_LEFT))
		{
			if (m_Velocity.x > (m_MaxVelocity.x * -1))
			{
				m_Velocity.x -= m_Speed;
			}
		}
		break;
	case VK_SPACE:
		if (KeyboardClass::IsKeyPressed(VK_SPACE))
		{
			if (m_Velocity.y < m_MaxVelocity.y)
			{
				m_Velocity.y += m_Speed;
				m_Grounded = false;
			}
		}
		break;
	case VK_SHIFT:
		if (KeyboardClass::IsKeyPressed(VK_SHIFT))
		{
			if (m_Velocity.y > (m_MaxVelocity.y * -1))
			{
				m_Velocity.y -= m_Speed;
			}
		}
		break;
	}
}

void ParticleModel::Update(const float dt)
{
	if (!m_InFluid)
	{
		UpdateState(dt);

		if (m_Grounded)
		{
			m_NetForce.y = 0.0f;
		}

		Move(dt);
	}
	else
	{
		MotionInFluid(dt);
	}
}

void ParticleModel::MoveConstAcceleration(const float dt)
{
	Vector3* position = m_parent->GetTransform()->GetPosition();
	m_parent->GetTransform()->SetPosition(new Vector3(*position + m_Velocity * dt));
}

void ParticleModel::MotionInFluid(const float dt)
{
	DragForce();
	UpdateState(dt);
	Move(dt);
}

void ParticleModel::DragForce()
{
	if (m_Laminar)
	{
		DragLamForce();
	}
	else
	{
		DragTurbFlow();
	}
}

void ParticleModel::DragLamForce()
{
	m_Drag = -m_DragFactor * m_Velocity;
}

void ParticleModel::DragTurbFlow()
{
	float mag = m_Velocity.GetMagnitude();
	Vector3 unit = m_Velocity.GetUnitVec();

	float dragMag = m_DragFactor * mag * mag;

	m_Drag = dragMag * unit;
}

void ParticleModel::ApplyForce(Vector3 force)
{
	m_Velocity += force;
}

void ParticleModel::UpdateNetForce()
{
	if (!m_InFluid)
	{
		m_NetForce += m_ExternalForce;
	}
	else
	{
		m_NetForce += m_Drag;
	}
}

void ParticleModel::UpdateAccel()
{
	m_Acceleration = m_NetForce / m_Mass;
}

void ParticleModel::UpdateState(const float dt)
{
	m_NetForce.Zero();
	UpdateNetForce();
	UpdateAccel();
}

void ParticleModel::Move(const float dt)
{
	Vector3* position = m_parent->GetTransform()->GetPosition();
	m_parent->GetTransform()->SetPosition(new Vector3(*position + m_Velocity * dt + 0.5f * m_Acceleration * dt * dt));

	m_Velocity += m_Acceleration * dt;

	m_BoundSphere->center = *position;	
}
