#include "PhysicsSystem.h"
#include <assert.h>

#include "RigidBody.h"
//#include "ICollider.h"
#include "Collision.h"
#include "Messaging.h"

ElysiumEngine::PhysicsSystem* ElysiumEngine::PhysicsSystem::g_PhysicsSystem = nullptr;

ElysiumEngine::PhysicsSystem::PhysicsSystem() : ISystem("Physics System"), timer(0.0f)
{
    assert(g_PhysicsSystem == nullptr && "Can only have a single physics system");
    g_PhysicsSystem = this;
    
    pause = false;
    step = true;
    contacts = 0;
}


void ElysiumEngine::PhysicsSystem::addRigidBody(ElysiumEngine::RigidBody *body)
{
    rigidBodies.push_back(body);
    if(body->ignorePause)
    {
        alwaysRun.push_back(body);
    }
}

void ElysiumEngine::PhysicsSystem::removeRigidBody(ElysiumEngine::RigidBody *body)
{
    rigidBodies.remove(body);
    if(body->ignorePause)
    {
        alwaysRun.remove(body);
    }
}

void ElysiumEngine::PhysicsSystem::initialize()
{
    
}

void ElysiumEngine::PhysicsSystem::runAlwaysRunObjects(float dt)
{
    timer += dt;
    
    while(timer > TIME_STEP)
    {
        contacts = 0;
        
        for(auto &body : alwaysRun)
        {
            body->updateAcceleration();
            body->integrateVelocity(TIME_STEP);
        }
        
        //Solve Constraints
        solveConstraints();
        
        //Remove dead constraints
        for(auto it = constraints.begin(); it != constraints.end();)
        {
            (*it)->m_LifeTime--;
			if((*it)->m_LifeTime <= 0)
			{
				delete (*it);
                it = constraints.erase(it);
                continue;
			}
			if(it == constraints.end())
				break;
            ++it;
        }
        
        for(auto &body : alwaysRun)
        {
            body->integratePosition(TIME_STEP);
        }
        
        //Collision Detection
        collisionDetection();
        //Collision Resolution
        collisionResolution();
        contactCache.clear();
        for(auto constraint : constraints)
        {
            if(constraint->contact)
            {
                ContactJacobian *contactJacobian = dynamic_cast<ContactJacobian * >(constraint->m_Jacobian);
                
                std::pair<RigidBody*,RigidBody*> pair(constraint->m_One,constraint->m_Two);
                
                contactCache[pair].push_back(ContactPointCacheEntry(contactJacobian->contactPoint,constraint->lambda));
            }
        }
        timer -= TIME_STEP;
    }
    
}

void ElysiumEngine::PhysicsSystem::update(float dt)
{
    if(pause && !step)
    {
        runAlwaysRunObjects(dt);
        return;
    }
    
    timer += dt;
    
    while(timer > TIME_STEP || step)
    {
        contacts = 0;
        for(auto &body : rigidBodies)
        {
            body->updateAcceleration();
            body->integrateVelocity(TIME_STEP);
        }
        
        //Solve Constraints
        solveConstraints();
        
        //Remove dead constraints
        for(auto it = constraints.begin(); it != constraints.end();)
        {
            (*it)->m_LifeTime--;
			if((*it)->m_LifeTime <= 0)
			{
				delete (*it);
                it = constraints.erase(it);
                continue;
			}
			if(it == constraints.end())
				break;
            ++it;
        }
        
        for(auto &body : rigidBodies)
        {
            body->integratePosition(TIME_STEP);
        }
        
        //Collision Detection
        collisionDetection();
        //Collision Resolution
        collisionResolution();
        
        
        contactCache.clear();
        for(auto constraint : constraints)
        {
            if(constraint->contact)
            {
                ContactJacobian *contactJacobian = dynamic_cast<ContactJacobian * >(constraint->m_Jacobian);
                
                std::pair<RigidBody*,RigidBody*> pair(constraint->m_One,constraint->m_Two);
                
                contactCache[pair].push_back(ContactPointCacheEntry(contactJacobian->contactPoint,constraint->lambda));
            }
        }
        timer -= TIME_STEP;
        step = false;
    }
}

ElysiumEngine::PhysicsSystem::~PhysicsSystem()
{
    g_PhysicsSystem = nullptr;
}

void ElysiumEngine::PhysicsSystem::collisionDetection()
{
    auto first = colliders.begin();
    while(first != colliders.end())
    {
        auto second = first;
        second++;
        while(second != colliders.end())
        {
            //Check Collision between first and second
            narrowPhase(*first,*second);
            second++;
        }
        first++;
    }
}

static const float CACHE_EPSISLON = 0.1f;

void ElysiumEngine::PhysicsSystem::collisionResolution()
{
    for(auto pair : collisionPairs)
    {
        RigidBody *one = pair.m_One->getSibling<RigidBody>("RigidBody");
		RigidBody *two = pair.m_Two->getSibling<RigidBody>("RigidBody");
        
        
		if(one->m_Velocity.Dot(two->m_Velocity) < 0)
			continue;
        
        ContactJacobian *contact = new ContactJacobian(pair.m_CollisionNormal,pair.m_CollisionPoint-one->GetPosition(),pair.m_CollisionPoint-two->GetPosition(),one->GetPosition(),two->GetPosition(),pair.penetrationDistance);
    
        contact->contactPoint = pair.m_CollisionPoint;
        
        IConstraint *contactConstraint = new IConstraint(one,two,contact,nullptr,1);
    
        
        //Check for cache hit
        std::pair<RigidBody *, RigidBody *> cachePair(one,two);
        auto found = contactCache.find(cachePair);
        if(found != contactCache.end())
        {
            for(auto cacheElement : found->second)
            {
                if((pair.m_CollisionPoint - cacheElement.contactPoint).Length() <= CACHE_EPSISLON)
                {
                    contactConstraint->lambda = cacheElement.lambda;
                }
            }
        }
        
        contactConstraint->contact = true;
		constraints.push_back(contactConstraint);
        
        Vec4 b = (two->getVelocity() - one->getVelocity());
    
        Vec4 u1 = b - ((b.Dot(pair.m_CollisionNormal)) * pair.m_CollisionNormal);
        u1.Normalize();
        Vec4 u2 = u1.Cross(pair.m_CollisionNormal);
        u2.Normalize();
        
        DrawDebugLine obj1Normal = DrawDebugLine(one->GetPosition(),one->GetPosition()+pair.m_CollisionNormal * 1.5,Vec4(1.0f,0.0f,0.0f));
        DrawDebugLine obj1u1 = DrawDebugLine(one->GetPosition(),one->GetPosition()+u1 * 1.5,Vec4(0.0f,1.0f,0.0f));
        DrawDebugLine obj1u2 = DrawDebugLine(one->GetPosition(),one->GetPosition()+u2 * 1.5,Vec4(0.0f,0.0f,1.0f));
        
        MessagingSystem::g_MessagingSystem->broadcastMessage(&obj1Normal);
        MessagingSystem::g_MessagingSystem->broadcastMessage(&obj1u1);
        MessagingSystem::g_MessagingSystem->broadcastMessage(&obj1u2);

        //push back the collision constraints
        FrictionJacobian *u1Jacobian = new FrictionJacobian(contactConstraint,one,two,pair.m_CollisionPoint,pair.m_CollisionNormal,u1);
        FrictionJacobian *u2Jacobian = new FrictionJacobian(contactConstraint,one,two,pair.m_CollisionPoint,pair.m_CollisionNormal,u2);
        
        constraints.push_back(new IConstraint(one,two,u1Jacobian,nullptr,1));
        constraints.push_back(new IConstraint(one,two,u2Jacobian,nullptr,1));
    }
    collisionPairs.clear();
}

void ElysiumEngine::PhysicsSystem::addCollider(ElysiumEngine::ICollider *collider)
{
    colliders.push_back(collider);
}

void ElysiumEngine::PhysicsSystem::removeCollider(ElysiumEngine::ICollider *collider)
{
    colliders.remove(collider);
}

void ElysiumEngine::PhysicsSystem::solveConstraints()
{
    static int IterationCount = 10;
    static float lambda0 = 0;
    
    for(int i = 0; i < IterationCount; ++i)
    {
        for(auto constraint : constraints)
        {
            constraint->m_Jacobian->Calculate();
            
            Vec4 *Elements = constraint->m_Jacobian->Get();
            
            float JVb = (Elements[0].Dot(constraint->m_One->m_Velocity) + Elements[1].Dot(constraint->m_One->m_AngularVelocity) + Elements[2].Dot(constraint->m_Two->m_Velocity) + Elements[3].Dot(constraint->m_Two->m_AngularVelocity)) + constraint->m_Jacobian->GetBias();
            
            float JMInverseJTranspose = Elements[0].Dot(Elements[0]) * constraint->m_One->m_InverseMass + Elements[2].Dot(Elements[2]) * constraint->m_Two->m_InverseMass + Elements[1].Dot(constraint->m_One->m_InverseTensor * Elements[1]) + Elements[3].Dot(constraint->m_Two->m_InverseTensor * Elements[3]);
            
            float delta = -JVb/JMInverseJTranspose;
            
            lambda0 = constraint->lambda;
            
            constraint->lambda = std::max(constraint->m_Jacobian->GetMin(),std::min(constraint->lambda+delta,constraint->m_Jacobian->GetMax())) ;
            
            delta = constraint->lambda - lambda0;
            
            if(!constraint->m_One->isStatic)
            {
                constraint->m_One->m_Velocity += constraint->m_One->m_InverseMass * delta * Elements[0];
                constraint->m_One->m_AngularVelocity += constraint->m_One->m_InverseTensor * delta * Elements[1];
            }
            if(!constraint->m_Two->isStatic)
            {
                constraint->m_Two->m_Velocity += constraint->m_Two->m_InverseMass * delta * Elements[2];
                constraint->m_Two->m_AngularVelocity += constraint->m_Two->m_InverseTensor * delta * Elements[3];
            }
            
        }
    }
}

void ElysiumEngine::PhysicsSystem::addCollisionPair(const ElysiumEngine::ColliderPair &pair)
{
    contacts++;
    collisionPairs.push_back(pair);
}

void ElysiumEngine::PhysicsSystem::setPause(bool pause)
{
    this->pause = pause;
}

void ElysiumEngine::PhysicsSystem::takeStep()
{
    step = true;
}

std::list<ElysiumEngine::GameObject *> ElysiumEngine::PhysicsSystem::checkRay(Vec4 dir, Vec4 origin)
{
    std::list<GameObject *> objects;
    
    
    return objects;
}