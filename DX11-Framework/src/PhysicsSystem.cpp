#include "PhysicsSystem.h"
#include "ParticleModel.h"

PhysicsSystem* PhysicsSystem::m_instance;

PhysicsSystem::PhysicsSystem() 
{
    m_instance = this;
}

PhysicsSystem* PhysicsSystem::GetInstance()
{
    if (m_instance)
    {
        return m_instance;
    }
    else
    {
        m_instance = new PhysicsSystem();
        return m_instance;
    }
}

bool PhysicsSystem::CollisionCheck(GameObject* lhs, GameObject* rhs)
{
    BoundingSphere* lhsBound, *rhsBound;
    lhsBound = lhs->GetParticleModel()->GetBoundSphere();
    rhsBound = rhs->GetParticleModel()->GetBoundSphere();

    float radSum = lhsBound->radius + rhsBound->radius;
    float distance = lhsBound->center.Distance(rhsBound->center);

    return radSum > distance;
}
