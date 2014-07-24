#pragma once
#include "GameObject.h"
#include "Transform.h"

namespace ElysiumEngine
{
	class Light : public IComponent
	{
	public:
        Light();
        ~Light();
        
        virtual void serialize(FileIO::IReader &reader);
        virtual void deserialize(){}
        
        virtual void initialize(){};
        virtual void lateInitialize();
        
        const Vec4 &getDiffuse(){return diffuse;}
        const Vec4 &getAmbient(){return ambient;}
        const Vec4 &getSpecular(){return specular;}
        const Vec4 &getPosition(){return trans->GetPosition();}
	private:
		Vec4 diffuse, ambient, specular;
        Transform *trans;
	};
}