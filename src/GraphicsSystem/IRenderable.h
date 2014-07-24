#pragma once
#include "Engine/GameObject.h"

namespace ElysiumEngine
{
	class IRenderable : public IComponent
	{
	public:
		IRenderable(std::string name);//Adds the renderable to the graphics system list of items to render
        virtual ~IRenderable();
		virtual void render() = 0;
	private:
	};
}