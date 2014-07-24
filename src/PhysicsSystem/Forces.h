#pragma once
#include "Maths/MathLib.h"
#include <list>

namespace ElysiumEngine
{
	class IForce
	{
	public:
        virtual ~IForce(){}
		virtual Vec4 GetForce() = 0;
	private:
	};

	class Gravity : public IForce
	{
	public:
		Gravity(Vec4 magnitude, float mass);
		virtual Vec4 GetForce();
	private:
		float m_Mass;
		Vec4 m_Magnitude;
	};

	class ForceAccumulator
	{
	public:
		void AddForce(IForce *force);
		void RemoveForce(IForce *force);
		void ClearForces();
		~ForceAccumulator();

		Vec4 Calculate();
	private:
		//Current foces applied to this accumulator
		std::list<IForce *> m_Forces;
		Vec4 m_AccumlatedForce;//The accumulated force for the object
	};
}