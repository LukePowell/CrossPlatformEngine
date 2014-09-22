#pragma once
#include "ISystem.h"
#include "Constraint.h"
#include "Collider.h"
#include <list>
#include <algorithm>

#define TIME_STEP 0.016f

namespace ElysiumEngine
{
	class RigidBody;
	class ICollider;

	class PhysicsSystem : public ISystem
	{
	public:
		PhysicsSystem();

		void initialize();
		void update(float dt);
        
        void addRigidBody(RigidBody *body);
        void removeRigidBody(RigidBody *body);
        
        void addCollider(ICollider *collider);
        void removeCollider(ICollider *collider);

        void collisionDetection();
        void collisionResolution();
        
        void solveConstraints();
        
        void addCollisionPair(const ColliderPair &pair);
		~PhysicsSystem();
        
        static PhysicsSystem *g_PhysicsSystem;
        
        bool getPaused(){return pause;}
        void setPause(bool pause);
        void takeStep();
        
        std::list<GameObject *> checkRay(Vec4 dir, Vec4 origin);
        
        unsigned int getNumberOfContacts(){return contacts;}
        
	private:
        
        void runAlwaysRunObjects(float dt);
        
        float timer;
        
        struct ContactPointCacheEntry
        {
            ContactPointCacheEntry(Vec4 point, float lambda) : contactPoint(point), lambda(lambda){}
            Vec4 contactPoint;
            float lambda;
        };
        
        std::list<IConstraint *> constraints;
		std::list<RigidBody *> rigidBodies;
		std::list<ICollider *> colliders;
        std::list<RigidBody *> alwaysRun;
        
        
        std::map<std::pair<RigidBody *, RigidBody*>,std::list<ContactPointCacheEntry> > contactCache;
        bool pause;
        bool step;
        unsigned int contacts;
        std::list<ColliderPair> collisionPairs;
	};
}