#pragma once
#include <float.h>
#include "Maths/MathLib.h"

namespace ElysiumEngine
{
    class RigidBody;
    class IConstraint;
    
	class IJacobian
	{
	public:
        virtual ~IJacobian(){}
		Vec4 *Get();
		virtual void Calculate() = 0;
		virtual float GetMax(){return FLT_MAX;}
		virtual float GetMin(){return -FLT_MAX;}
		virtual float GetBias(){return 0.0f;}
	protected:
		Vec4 m_Elements[4];
	};

	class DistanceJacobian : public IJacobian
	{
	public:
		DistanceJacobian(RigidBody *one, RigidBody *two, Vec4 pointONe, Vec4 pointTwo);
		virtual void Calculate();
	private:
		RigidBody *m_One, *m_Two;
		Vec4 m_PointOne, m_PointTwo;
	};
	
	class ContactJacobian : public IJacobian
	{
	public:
		ContactJacobian(Vec4 normal, Vec4 r1, Vec4 r2, Vec4 x1, Vec4 x2, float penetrationDepth);
		virtual void Calculate();
		virtual float GetMin(){return 0.0f;}
		virtual float GetBias();
        Vec4 contactPoint;
	private:
		Vec4 m_Normal, m_R1, m_R2, m_X1, m_X2;
        float penetrationDepth;
	};
    
    class FrictionJacobian : public IJacobian
    {
    public:
        FrictionJacobian(IConstraint *contact, RigidBody *one, RigidBody *two, Vec4 contactPoint, Vec4 normal, Vec4 dir);
        virtual void Calculate();
		virtual float GetMin();
        virtual float GetMax();
		virtual float GetBias();
    private:
        RigidBody *one, *two;
        IConstraint *contact;
        Vec4 contactPoint, normal, dir, r1, r2;
    };
    
	class IConstraint
	{
	public:
		IConstraint(RigidBody *one, RigidBody *two, IJacobian *jOne, IJacobian *jTwo, int lifeTime);
		//Allow for Inequality ElysiumEngine, default to FLT_MAX, and -FLT_MAX (-inf,inf)
        
        ~IConstraint(){delete m_Jacobian;}
	
		IJacobian *m_Jacobian;
		RigidBody *m_One, *m_Two;
		int m_LifeTime;
        float lambda;
        bool contact;
	};
}