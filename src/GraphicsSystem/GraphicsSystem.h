#pragma once
#include "Engine/ISystem.h"
#include "WindowingSystem/WindowingSystem.h"
#include "IRenderable.h"
#include "ShaderFactory.h"
#include "ModelLibrary.h"
#include "Maths/MathLib.h"
#include "Light.h"
#include <list>
#include "RenderMatrixStack.h"//Replace OpenGL/DirectX calculations allows for greater portability

namespace ElysiumEngine
{
    class ICamera;
    class DebugDrawRenderable;

	struct VideoMode
	{
		int resX, resY;//Resolution x and y
		int refreshRate;//Refresh rate
	};
    
    class VertexData
    {
    public:
        Vec4 position;
        Vec4 normal;
        float texture[2];
		Vec4 color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    };

	class Strip
	{
	public:
		unsigned int *indices;
		unsigned int count;
	};

	class GraphicsSystem : public ISystem
	{
	public:
		GraphicsSystem(IWindow *window);

		virtual void initialize();
		virtual void update(float dt);

        void addRenderable(IRenderable *renderable);
        void removeRenderable(IRenderable *renderable);
        
		//std::vector<VideoMode> getFullScreenVideoModes();
        
        static GraphicsSystem *g_GraphicsSystem;
        
        ShaderFactory &getShaderFactory();
        ModelLibrary &getModelLibrary();
        
        void handleMessage(WindowResizedMessage &msg);
        
        ICamera *getCamera(){return camera;}
        void addCamera(ICamera *camera);
        void removeCamera(ICamera *camera);
        
        std::list<Light *> &getLights(){return lights;}
    
        void addLight(Light *light);
        void removeLight(Light *light);
        
        void sendToChildren(IMessage *message);
        void setWireframe(bool wireframe);
        
        RenderMatrixStack &getRenderMatrixStack(){ return stack;}
        
	private:
        ICamera *camera;
		IWindow *window;
        ShaderFactory shaderFactory;
        ModelLibrary modelLibrary;
        RenderMatrixStack stack;
        
        DebugDrawRenderable *debugDraw;
        std::list<IRenderable *> renderables;
        std::list<Light *> lights;
	};
}