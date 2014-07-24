#pragma once
#include "GameObject.h"
#include "Maths/MathLib.h"
#include "Transform.h"

namespace ElysiumEngine
{
	class ICamera : public IComponent
	{
	public:
		ICamera(std::string name);
		virtual ~ICamera();
		virtual const Vec4& getEye() = 0;
		virtual const Vec4& getLookAt() = 0;
		virtual void setDirection(Vec4 &direction) = 0;
	};
    
	class FreeCamera : public ICamera
	{
	public:
		FreeCamera() : ICamera("FreeCamera") {}
        
		virtual const Vec4& getEye();
		virtual const Vec4& getLookAt();
		virtual void setDirection(Vec4 &direction);
        const Vec4 &getDirection(){return m_Direction;}
        
        virtual void initialize(){}
		virtual void lateInitialize();
        
		virtual void serialize(FileIO::IReader &reader);
		virtual void deserialize();
	private:
        Vec4 m_Eye;
        Vec4 m_LookAt;
		Vec4 m_Direction;
		Transform *m_Transform;
	};
}