#pragma once
#include "InputSystem/InputSystem.h"
#include "PhysicsSystem/RigidBody.h"
#include "GraphicsSystem/ICamera.h"

namespace ElysiumEngine
{
    class CameraController : public IController
    {
    public:
        CameraController();
        ~CameraController();
        void update(float dt);
        
        void initialize(){};
        void lateInitialize();
        
        virtual void serialize(FileIO::IReader &reader){}
        virtual void deserialize(){}
        
        
        
    private:
        RigidBody *body;
        FreeCamera *cam;
        float speed;
        float rotationSpeed;
    };
}