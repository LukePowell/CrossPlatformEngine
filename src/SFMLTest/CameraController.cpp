#include "CameraController.h"

ElysiumEngine::CameraController::CameraController() : IController("CameraController")
{
    g_InputSystem->Register(this);
    speed = 10.0f;
    rotationSpeed = 0.10f;
}

ElysiumEngine::CameraController::~CameraController()
{
    g_InputSystem->Unregister(this);
}

void ElysiumEngine::CameraController::update(float dt)
{
    float axisHori = g_InputSystem->getAxis("Horizontal");
    float axisVert = g_InputSystem->getAxis("Vertical");
    float rotateHori = g_InputSystem->getAxis("RotateHorizontal");
    float axisUp = g_InputSystem->getAxis("Up");
    Vec4 velocity;
    
    Vec4 right = Vec4(0.0f,1.0f,0.0f);
    right = cam->getDirection().Cross(right);
    
    velocity += axisVert * cam->getDirection();
    velocity += axisHori * right;
    velocity += axisUp * Vec4(0.0f,1.0f,0.0f);
    velocity.Normalize();
    velocity *= speed;
    
    body->setVelocity(velocity);
    
    Vec4 angularVelcoty;
    angularVelcoty += dt * rotateHori * Vec4(1.0f, 0.0f,0.0f);
    
    Matrix m = BuildYRotationMatrix(rotateHori * rotationSpeed);
    Vec4 direction = m * cam->getDirection();
    cam->setDirection(direction);
    
    //Do something to rotate here.
}


void ElysiumEngine::CameraController::lateInitialize()
{
    if(hasSibling("RigidBody"))
    {
        body = getSibling<RigidBody>("RigidBody");
    }
    else
    {
        body = nullptr;
    }
    
    if(hasSibling("FreeCamera"))
    {
        cam = getSibling<FreeCamera>("FreeCamera");
    }
    else
    {
        cam = nullptr;
    }
}