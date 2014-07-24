#include "Transform.h"
#include <iostream>
#include <sstream>



ElysiumEngine::Transform::Transform() : IComponent("Transform")
{
	m_Position = Vec4(0.0f,0.0f,0.0f);
	m_Scale    = Vec4(1.0f,1.0f,1.0f);
    //m_Orientation = Maths::Quaternion();
}

void ElysiumEngine::Transform::initialize()
{
    
}

void ElysiumEngine::Transform::lateInitialize()
{
    
}

void ElysiumEngine::Transform::serialize(FileIO::IReader &reader)
{
    reader.GetFirstChild();
	while(reader.ReadNextChild())
	{
		if(reader.GetTag() == "position")
		{
			//float x,y,z;
            
			reader.GetFloatValueAtr("x",&m_Position.x);
			reader.GetFloatValueAtr("y",&m_Position.y);
			reader.GetFloatValueAtr("z",&m_Position.z);
            
		}
		if(reader.GetTag() == "scale")
		{
			//float x,y,z;
            
			reader.GetFloatValueAtr("x",&m_Scale.x);
			reader.GetFloatValueAtr("y",&m_Scale.y);
			reader.GetFloatValueAtr("z",&m_Scale.z);
            
			std::stringstream StringOut;
		}
		if(reader.GetTag() == "orientation")
		{
			//Read in the orientation from the file
			reader.GetFloatValueAtr("x",&m_Orientation.x);
			reader.GetFloatValueAtr("y",&m_Orientation.y);
			reader.GetFloatValueAtr("z",&m_Orientation.z);
			reader.GetFloatValueAtr("w",&m_Orientation.w);
		}
	}
    reader.GetParent();
    
	std::stringstream StringOut;
	StringOut << "\n\tPosition: "  << m_Position.x << " : " << m_Position.y << " : " << m_Position.z << std::endl;
	StringOut << "\tScale: " <<  m_Scale.x << " : " << m_Scale.y << " : " << m_Scale.z << std::endl;
    std::cout << StringOut.str();
}

void ElysiumEngine::Transform::SetPosition( float x, float y, float z )
{
	m_Position = Vec4(x,y,z);
}

void ElysiumEngine::Transform::SetPosition( const Vec4 &position )
{
	m_Position = position;
}

void ElysiumEngine::Transform::deserialize()
{
}

void ElysiumEngine::Transform::SetScale( float x, float y, float z )
{
	m_Scale = Vec4(x,y,z);
}

void ElysiumEngine::Transform::SetScale( const Vec4 &scale )
{
	m_Scale = scale;
}

void ElysiumEngine::Transform::SetOrientation( float x, float y, float z, float w )
{
	m_Orientation = Maths::Quaternion(x,y,z,w);
}

void ElysiumEngine::Transform::SetOrientation( const Maths::Quaternion &orientation )
{
	m_Orientation = orientation;
}

