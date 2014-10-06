/* Systems*/
#include "WindowingSystem/WindowingSystem.h"
#include "SDLWindow/SDLWindow.h"

#include "GraphicsSystem/GraphicsSystem.h"
#include "GraphicsSystem/ShaderFactory.h"
#include "GraphicsSystem/GLSLShader.h"

#include "PhysicsSystem/PhysicsSystem.h"

#include "Messaging.h"

#include "InputSystem/InputSystem.h"

#include "GameObject.h"
#include "GameObjectFactory.h"

#include "LevelSystem.h"

/*Components */
#include "Transform.h"
#include "PhysicsSystem/RigidBody.h"
#include "PhysicsSystem/Collider.h"
#include "GraphicsSystem/MeshRenderable.h"
#include "GraphicsSystem/Mesh.h"
#include "CameraController.h"
#include "GraphicsSystem/ICamera.h"
#include "GraphicsSystem/Light.h"

/*Utilities*/
#include "Hashing.h"
#include "PhysicsSystem/HalfEdgePhysics.h"

/*Standard libaries */
#include <iostream>
#include <sstream>
#include <map>

#include "Console.h"

#include "CS350BoundingVolume.h"

#include <ctime>

static bool consoleEnabled = false;

#include "ComponentGen.h"


void parseXMLOnThread()
{
    
}
enum BoundVolumes
{
    E_NONE,
    E_AABB,
    E_BOUNDING_SPHERE_CENTROID,
    E_BOUNDING_SPHERE_RITTER,
    E_BOUNDING_SPHERE_LARSSONS,
    E_BOUNDING_SPHERE_PCA,
    E_BOUNDING_ELLIPSOID_PCA,
    E_BOUNDING_BOX_PCA,
    E_COUNT_VOLUME_TYPES
};

enum BVH
{
    E_NO_BVH,
    E_TOP_DOWN,
    E_BOTTOM_UP,
    E_COUNT_BVH_TYPES
};

std::string boundingVolumeNames[] = {"None","Axis Alligned Bounding Box", "Bounding Sphere Centeroid", "Bounding Sphere Ritter","Bounding Sphere Larsson's Method Not Implemented","Bounding Sphere PCA","Bounding Ellipsoid PCA Based Method Not Implemented","Bounding Box PCA Based Method Not Implemented"};

BoundVolumes current = E_NONE;
BVH bvh = E_NO_BVH;
CS350::BoundingVolumeHierarchy *vh = nullptr;

void simulationControls()
{
    if(ElysiumEngine::g_InputSystem->getKeyTriggered("pause"))
    {
        ElysiumEngine::PhysicsSystem::g_PhysicsSystem->setPause(!ElysiumEngine::PhysicsSystem::g_PhysicsSystem->getPaused());
    }
    if(ElysiumEngine::g_InputSystem->getKeyTriggered("step"))
    {
        if(ElysiumEngine::PhysicsSystem::g_PhysicsSystem->getPaused())
            ElysiumEngine::PhysicsSystem::g_PhysicsSystem->takeStep();
    }
    if(ElysiumEngine::g_InputSystem->getKeyTriggered("nextLevel"))
    {
        int index = ElysiumEngine::LevelSystem::g_LevelSystem->getCurrentLevelIndex();
        index++;
        index %= ElysiumEngine::LevelSystem::g_LevelSystem->getLevelCount();
        ElysiumEngine::LevelSystem::g_LevelSystem->loadLevel(index);
    }
    if(ElysiumEngine::g_InputSystem->getKeyTriggered("previousLevel"))
    {
        int index = ElysiumEngine::LevelSystem::g_LevelSystem->getCurrentLevelIndex();
        index--;
        if(index < 0)
            index = ElysiumEngine::LevelSystem::g_LevelSystem->getLevelCount() + index;
        index %= ElysiumEngine::LevelSystem::g_LevelSystem->getLevelCount();
        ElysiumEngine::LevelSystem::g_LevelSystem->loadLevel(index);
    }
    if(ElysiumEngine::g_InputSystem->getKeyTriggered("wireframe"))
    {
        static bool wireframe = false;
        wireframe = !wireframe;
        ElysiumEngine::GraphicsSystem::g_GraphicsSystem->setWireframe(wireframe);
    }
    if(ElysiumEngine::g_InputSystem->getKeyTriggered("console"))
    {
        consoleEnabled = !consoleEnabled;
    }
#define ASSIGNMENT_2
#ifdef ASSIGNMENT_2
    //Controls to switch bounding volume for Assignment 2
    if(ElysiumEngine::g_InputSystem->getKeyTriggered("nextBV"))
    {
        current = (BoundVolumes)((current + 1) % E_COUNT_VOLUME_TYPES);
        ElysiumEngine::GameObjectFactory::g_GameObjectFactory->forAllGameObjects([](ElysiumEngine::GameObject *obj){
                if(!obj->hasComponent("MeshRenderable"))
                    return;
            
                if(obj->hasComponent("CS350BoundingVolume"))
                {
                    obj->removeComponent(obj->getComponent<CS350::CS350BoundingVolume>("CS350BoundingVolume"));
                }
            
                if(!obj->hasComponent("CS350BoundingVolumeRenderer"))
                {
                    obj->addComponent(new CS350::CS350BoundingVolumeRenderer());
                }
            
                ElysiumEngine::MeshRenderable *renderable = obj->getComponent<ElysiumEngine::MeshRenderable>("MeshRenderable");

				ElysiumEngine::Mesh *mesh = renderable->getMesh();
	            
		        CS350::CS350BoundingVolume * vol = nullptr;
                switch(current)
                {
                    case E_NONE:
                        break;
                    case E_BOUNDING_BOX_PCA:
                       // vol = CS350::createOBBPCAMethod(mesh);
                        break;
                    case E_AABB:
                        vol = CS350::createAABB(mesh);
                        break;
                    case E_BOUNDING_SPHERE_CENTROID:
                        vol = CS350::createSphereCentroidMethod(mesh);
                        break;
                    case E_BOUNDING_SPHERE_RITTER:
                        vol = CS350::createSphereRittersMethod(mesh);
                        break;
                    case E_BOUNDING_SPHERE_LARSSONS:
                        vol = CS350::createSphereLarsonsMethod(mesh);
                        break;
                    case E_BOUNDING_SPHERE_PCA:
                        vol = CS350::createSpherePCAMethod(mesh);
                        break;
                    case E_BOUNDING_ELLIPSOID_PCA:
                        vol = CS350::createEllipsoidPCAMethod(mesh);
                        break;
                    case E_COUNT_VOLUME_TYPES:
                        break;
                }
            std::cout << "Now using " << boundingVolumeNames[current] << std::endl;
            obj->getComponent<CS350::CS350BoundingVolumeRenderer>("CS350BoundingVolumeRenderer")->setVolume(vol);
            if(vol)
            {
                obj->addComponent(vol);
            }
            });
    }
#endif
    if(ElysiumEngine::g_InputSystem->getKeyTriggered("nextBVH"))
    {
        bvh = (BVH)((bvh + 1) % E_COUNT_BVH_TYPES);
        
        
        if(vh)
        {
            delete vh;
            vh = nullptr;
        }
        
        int count = 0;
        ElysiumEngine::GameObjectFactory::g_GameObjectFactory->forAllGameObjects([&count](ElysiumEngine::GameObject *obj){
            if(obj->hasComponent("CS350BoundingVolume"))
            {
                count++;
            }
        });
        
        CS350::CS350BoundingVolume **objects = new CS350::CS350BoundingVolume*[count];
        int i = 0;
        ElysiumEngine::GameObjectFactory::g_GameObjectFactory->forAllGameObjects([&objects,&i](ElysiumEngine::GameObject *obj){
            if(obj->hasComponent("CS350BoundingVolume"))
            {
                objects[i] = obj->getComponent<CS350::CS350BoundingVolume>("CS350BoundingVolume");
                i++;
            }
        });
        
        switch(bvh)
        {
            case E_TOP_DOWN:
                vh = new CS350::BoundingVolumeHierarchy();
                *vh = CS350::buildBVHTopDown(objects, count);
                break;
            case E_BOTTOM_UP:
                break;
            default:
                break;
        }
    }
    if(ElysiumEngine::g_InputSystem->getKeyTriggered("toggleBVH"))
    {
        
    }
}

//Console bound function to load an OBJ file to a given location...
void loadObjToPosition(std::string filename, float x, float y, float z)
{
    ElysiumEngine::GameObject *obj = ElysiumEngine::GameObjectFactory::g_GameObjectFactory->createEmptyGameObject();
    obj->addComponent(new ElysiumEngine::Transform());
    obj->getComponent<ElysiumEngine::Transform>("Transform")->SetPosition(x, y, z);
    
    ElysiumEngine::MeshRenderable *mesh = new ElysiumEngine::MeshRenderable();
    mesh->setMeshFile(filename);
    mesh->setShader("BasicPhong");
    mesh->setDynamic(false);
    
    obj->addComponent(mesh);
    
    obj->initialize();
    obj->lateInitialize();
}

int main(int argc, char **argv)
{
    
    ElysiumEngine::InputSystem *inputSystem = new ElysiumEngine::InputSystem();
    inputSystem->loadInputConfig("inputconfig.xml");
    
    ElysiumEngine::GameObjectFactory *gameObjFactory = new ElysiumEngine::GameObjectFactory();
    registerComponents();
    
	std::clock_t clock = 0;
    
    ElysiumEngine::MessagingSystem *messagingSystem = new ElysiumEngine::MessagingSystem();
    
    ElysiumEngine::WindowingSystem* windowingSystem = new ElysiumEngine::WindowingSystem(new ElysiumEngine::TWindowAllocator<ElysiumEngine::SDLWindow>);
	windowingSystem->InitializeSubSystems();
	SDL_DisplayMode mode;
	int error = SDL_GetDesktopDisplayMode(0, &mode);
	if (error)
	{
		std::cout << SDL_GetError() << std::endl;
	}

    ElysiumEngine::IWindow *mainWindow = windowingSystem->createWindow("CS 350",mode.w,mode.h);
    
#ifdef WIN32 //Needed for windows
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		std::cout << ("It is the end of the world!");
#endif

    //Graphics
    ElysiumEngine::GraphicsSystem *graphicsSystem = new ElysiumEngine::GraphicsSystem(mainWindow);
    graphicsSystem->getShaderFactory().registerShader("GLSL",new ElysiumEngine::TShaderAllocator<ElysiumEngine::GLSLShader>());
    graphicsSystem->getShaderFactory().loadShaders("GLSLShaders.xml");
    graphicsSystem->initialize();
    //End Graphics
    
    //Physics
    ElysiumEngine::PhysicsSystem *physics = new ElysiumEngine::PhysicsSystem();
    //End Physics
    
    //Level System
    ElysiumEngine::LevelSystem *levelSystem = new ElysiumEngine::LevelSystem();
    
    levelSystem->loadLevels("LevelList.xml");
    levelSystem->loadLevel(0);
    
    unsigned int frames = 0;
    float elapsed = 0.0f;
    float dt = 0.0f;
    
    //Register systems to various messages for broadcast purposes
    messagingSystem->registerListener("WindowResize",graphicsSystem);
    messagingSystem->registerListener("DrawDebugPoint", graphicsSystem);
    messagingSystem->registerListener("DrawDebugLine", graphicsSystem);
    messagingSystem->registerListener("DrawDebugBox", graphicsSystem);
    messagingSystem->registerListener("DrawDebugSphere", graphicsSystem);
    
    
    messagingSystem->registerListener("KeyStateMessage",inputSystem);

    float systemTimes[10] = {0.0f};
    float tempElapsed = 0.0f;
    std::string stream;
    float total = 0.0f;

    while(mainWindow->isOpen())
    {
        total += dt;
		dt = static_cast<float>(std::clock() - clock) / CLOCKS_PER_SEC;
        elapsed += dt;
        if(elapsed >= 1.0f)
        {
            std::stringstream stream;
            stream << "FPS: " << frames << std::endl << "Objects: " << gameObjFactory->getNumOfObjects() << std::endl <<
            "Graphics: " << systemTimes[1] << " , " << systemTimes[1] / dt * 100 << "%" << std::endl << "Physics: " << systemTimes[2] << " , " << systemTimes[2] / dt * 100<< "%" << std::endl << "Time: " << total;
            
#ifdef ASSIGNMENT_2
            stream << std::endl << boundingVolumeNames[current] << std::endl;
#endif
            elapsed = 0.0f;
            frames = 0;
            
            //text.setString(stream.str());
        }
		clock = std::clock();
        frames++;
        
        //tempElapsed = clock.getElapsedTime().asSeconds();
        windowingSystem->update(dt);
        //systemTimes[0] = clock.getElapsedTime().asSeconds() - tempElapsed;
        
        simulationControls();
        inputSystem->update(dt);
        gameObjFactory->update(dt);
        
        //tempElapsed = clock.getElapsedTime().asSeconds();
        physics->update(dt);
        //systemTimes[2] = clock.getElapsedTime().asSeconds() - tempElapsed;
        
        //tempElapsed = clock.getElapsedTime().asSeconds();
        graphicsSystem->update(dt);
        //systemTimes[1] = clock.getElapsedTime().asSeconds() - tempElapsed;
        
        //w->pushGLStates();
        if(!consoleEnabled)
        {
            //w->draw(text);
        }
        else
        {
            //writer->draw();
        }
        
        //w->popGLStates();
        mainWindow->redraw();
        
    }

    delete graphicsSystem;
    delete windowingSystem;
    delete messagingSystem;
    delete gameObjFactory;
    delete inputSystem;

    return 0;
}