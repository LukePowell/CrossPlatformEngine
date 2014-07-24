#pragma once
#include "GameObject.h"
#include "Maths/MathLib.h"

namespace ElysiumEngine
{
	class Transform : public IComponent
	{
    public:
        Transform();
        ~Transform(){}
        virtual void serialize(FileIO::IReader &reader);
        virtual void deserialize();
        
        
        virtual void initialize();
        virtual void lateInitialize();
        
        Vec4& GetPosition() {return m_Position;}
		Vec4& GetScale()    {return m_Scale;}
		Maths::Quaternion& GetOrientation(){return m_Orientation;}
        
		void SetPosition(float x, float y, float z);
		void SetPosition(const Vec4 &position);
		void SetScale(float x, float y, float z);
		void SetScale(const Vec4 &scale);
		void SetOrientation(float x, float y, float z, float w);
		void SetOrientation(const Maths::Quaternion &orientation);
	private:
		Vec4 m_Position;
		Vec4 m_Scale;
		Maths::Quaternion m_Orientation;
	};
}