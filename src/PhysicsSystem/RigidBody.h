#pragma once
#include "GameObject.h"
#include "Transform.h"
#include "Maths/Vectors.h"
#include "Maths/Matrix.h"
#include "Forces.h"

namespace ElysiumEngine
{
	class RigidBody : public IComponent
	{
	public:
		RigidBody();
        RigidBody(const RigidBody *body);
		~RigidBody();

        
        void initialize();
        void lateInitialize();
        
        void updateAcceleration();
        void integrateVelocity(float dt);
        void integratePosition(float dt);
        
        void updateAndIntegrate(float dt);
        
		void serialize(FileIO::IReader &reader);
        void deserialize(){}
        
        void addForce(IForce *force);
        void removeForce(IForce *force);
        
        Vec4 GetPosition();
        
        bool getStatic(){return isStatic;}
        
        void setVelocity(Vec4 &vel);
		void setVelocity(float x, float y, float z);
		Vec4 &getVelocity();
        
		float getMass();
        
        bool isAsleep(){return asleep;}
        
    private:
        
        Vec4 m_Velocity;
		Vec4 m_AngularVelocity;
		Vec4 m_Acceleration;
		float m_Mass, m_InverseMass;
        
		int m_Type;
		Matrix m_IntertiaTensor;
		Matrix m_InverseTensor;
        
		bool m_Gravity;
		float m_GravMag;
		Transform *transform;

        ForceAccumulator accumulator;
        bool isStatic;
        bool ignorePause;
        
        Vec4 com;
        float density;
        bool asleep;
        
        friend class PhysicsSystem;
	};
}