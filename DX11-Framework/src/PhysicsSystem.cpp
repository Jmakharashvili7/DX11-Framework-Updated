#include "PhysicsSystem.h"
#include "ParticleModel.h"
#include "structs.h"
#include "Transform.h"

PhysicsSystem* PhysicsSystem::m_instance;

PhysicsSystem::PhysicsSystem() : m_gravity(0.0f, -9.8f, 0.0f), m_gravityEnabled(false)
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

bool PhysicsSystem::CollCheckSphereSphere(GameObject* lhs, GameObject* rhs)
{
    BoundingSphere* lhsBound, *rhsBound;
    lhsBound = lhs->GetParticleModel()->GetBoundSphere();
    rhsBound = rhs->GetParticleModel()->GetBoundSphere();

    float radSum = lhsBound->Diameter + rhsBound->Diameter;
    float distance = lhsBound->center.Distance(rhsBound->center);

    return radSum > distance;
}

void PhysicsSystem::CollResSphereSphere(GameObject* lhs, GameObject* rhs)
{
    Vector3* lhsPos = lhs->GetTransform()->GetPosition();
    Vector3* rhsPos = rhs->GetTransform()->GetPosition();

    // Calculate basis vector for the X axis
    Vector3 dir = Vector3(lhsPos->x - rhsPos->x, lhsPos->y - rhsPos->y, lhsPos->z - rhsPos->z);
    dir.Normalize();

    // Calculate for first object
    Vector3 velocity1 = lhs->GetParticleModel()->GetVelocity();
    float x1 = dir.Dot(velocity1);

    Vector3 velocityX1 = dir * x1;
    Vector3 velocityY1 = velocity1 - velocityX1;
    float mass1 = lhs->GetParticleModel()->GetMass();

    // Do the same for second object
    Vector3 velocity2 = rhs->GetParticleModel()->GetVelocity();
    float x2 = dir.Dot(velocity2);

    Vector3 velocityX2 = dir * x2;
    Vector3 velocityY2 = velocity2 - velocityX2;
    float mass2 = rhs->GetParticleModel()->GetMass();

    float restitution1 = lhs->GetParticleModel()->GetRestitution();
    float restitution2 = rhs->GetParticleModel()->GetRestitution();

    Vector3 totalVelocity1 = Vector3(velocityX1 * (mass1 - mass2)/(mass1 + mass2) + (velocityX2 * (2 * mass2)/(mass1 + mass2) * restitution1) + velocityY1);
    Vector3 totalVelocity2 = Vector3((velocityX1 * (2 * mass1) / (mass1 + mass2) * restitution2) + velocityX2 * (mass2 - mass1)/(mass1 + mass2) + velocityY2);

    lhs->GetParticleModel()->SetVelocity(totalVelocity1);
    rhs->GetParticleModel()->SetVelocity(totalVelocity2);
}

void PhysicsSystem::ApplyGravity(std::vector<GameObject*> gameObjects, float deltaTime)
{
    if (m_gravityEnabled)
    {
        for (GameObject* obj : gameObjects)
        {
            if (obj != nullptr)
            {
                Vector3* pos = obj->GetTransform()->GetPosition();

                if (obj->GetObjectType() == ObjectType::DYNAMIC)
                {
                    if (pos->y < 0.0f)
                    {
                        pos->y = 0.0f;
                        obj->GetTransform()->SetPosition(pos);
                        obj->GetParticleModel()->SetGrounded(true);
                    }
                    else
                    {
                        if (obj->GetParticleModel()->GetGrounded() == false)
                        {
                            obj->GetParticleModel()->ApplyForce(m_gravity * deltaTime);
                        }
                    }
                }
            }
        }
    }
}