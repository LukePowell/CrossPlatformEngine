    #include "InputSystem.h"
#include "Messaging.h"
#include "Hashing.h"
#include <sstream>
#include <algorithm>
#include "XMLReader.h"
#include <cmath>

ElysiumEngine::InputSystem *ElysiumEngine::g_InputSystem = nullptr;

namespace ElysiumEngine
{
    float fmax(float f1, float f2)
    {
        return  f1 > f2 ? f1 : f2;
    }
}

void ElysiumEngine::IInterpreter::Update( float dt )
{
	//Update our derivation
	OnUpdate(dt);
}

ElysiumEngine::IEventListener * ElysiumEngine::IInterpreter::getParent()
{
	return nullptr;
}

void ElysiumEngine::IInterpreter::sendToChildren(IMessage *message )
{
}

ElysiumEngine::IController::~IController()
{
	g_InputSystem->Unregister(this);
}

ElysiumEngine::IController::IController( std::string name ) : IComponent(name) 
{
	g_InputSystem->Register(this);
}

ElysiumEngine::KeyboardInterpreter::KeyboardInterpreter() : IInterpreter("keyboard")
{
	memset(&m_Current,0,sizeof(bool) * 256);
	memset(&m_Previous,0,sizeof(bool) * 256);
}

bool ElysiumEngine::KeyboardInterpreter::isTriggered(const std::string &action)
{
	auto found = keys.find(action);
    if(found == keys.end())
    {
        return false;
    }
    else
    {
        return isTriggered(found->second);
    }
}

bool ElysiumEngine::KeyboardInterpreter::isPressed(const std::string &action)
{
	auto found = keys.find(action);
    if(found == keys.end())
    {
        return false;
    }
    else
    {
        return isPressed(found->second);
    }
}

bool ElysiumEngine::KeyboardInterpreter::isReleased(const std::string &action)
{
	auto found = keys.find(action);
    if(found == keys.end())
    {
        return false;
    }
    else
    {
        return isReleased(found->second);
    }
}

bool ElysiumEngine::KeyboardInterpreter::isTriggered(unsigned int input)
{
	return m_Current[input] && !m_Previous[input];
}

bool ElysiumEngine::KeyboardInterpreter::isPressed(unsigned int input)
{
	return m_Current[input];
}

bool ElysiumEngine::KeyboardInterpreter::isReleased(unsigned int input)
{
	return !m_Current[input] && m_Previous[input];
}	

void ElysiumEngine::KeyboardInterpreter::handleMessage(KeyStateMessage &message )
{
    if(message.key > 255)
        return;
	m_Current[message.key] = message.up;
}

void ElysiumEngine::KeyboardInterpreter::OnUpdate( float dt )
{
	memcpy(m_Previous,m_Current,sizeof(bool) * 256);
}

ElysiumEngine::IEventListener * ElysiumEngine::InputSystem::getParent()
{
	return nullptr;
}

void ElysiumEngine::InputSystem::sendToChildren(IMessage *message )
{
    for(auto interpreter : m_Interpreters)
    {
        message->send(*interpreter.second);
    }
}

void ElysiumEngine::InputSystem::update( float dt )
{
    for(auto controller : m_Controllers)
    {
        controller->update(dt);
    }
    
    for(auto interpreter : m_Interpreters)
    {
        interpreter.second->Update(dt);
    }

	//Do our bound functions
	for (auto pair : responses)
	{
		if (getKeyPressed(pair.first))
		{
			for (auto response : pair.second)
			{
				response->onAction();
			}
		}
	}
}

ElysiumEngine::InputSystem::InputSystem() : ISystem("InputSystem")
{
	g_InputSystem = this;
    
    m_Interpreters[keyboardInterpreter.getName()] = &keyboardInterpreter;
    m_Interpreters[mouseInterpreter.getName()] = &mouseInterpreter;
}

ElysiumEngine::Point ElysiumEngine::InputSystem::GetMousePosition()
{
	return mouseInterpreter.GetPosition();
}

void ElysiumEngine::InputSystem::Register( IController *controller )
{
	m_Controllers.push_back(controller);
}

void ElysiumEngine::InputSystem::Unregister( IController *controller )
{
	auto It = std::find(m_Controllers.begin(),m_Controllers.end(),controller);
	if(It != m_Controllers.end())
		m_Controllers.erase(It);
}

void ElysiumEngine::InputSystem::SetMousePosition( const ElysiumEngine::Point &pt )
{
    mouseInterpreter.SetPosition(pt);
}

void ElysiumEngine::MouseIntepreter::OnUpdate( float dt )
{
	memcpy(&m_Previous,&m_Current,sizeof(MouseState));
}

bool ElysiumEngine::MouseIntepreter::isAnalog(const std::string &input)
{
	return false;
}

bool ElysiumEngine::MouseIntepreter::isTriggered(const std::string &action)
{
      unsigned int input = 0;
	return m_Current.m_MouseButtonState[input] && !m_Previous.m_MouseButtonState[input];
}

bool ElysiumEngine::MouseIntepreter::isPressed(const std::string &action)
{
      unsigned int input = 0;
	return m_Current.m_MouseButtonState[input];
}

bool ElysiumEngine::MouseIntepreter::isReleased(const std::string &action)
{  unsigned int input = 0;
	return !m_Current.m_MouseButtonState[input] && m_Previous.m_MouseButtonState[input];
}

ElysiumEngine::MouseIntepreter::MouseIntepreter() : IInterpreter("mouse")
{
	//Setup our mouse states
	memset(&m_Current,0,sizeof(m_Current));
	memset(&m_Previous,0,sizeof(m_Previous));
}

ElysiumEngine::Point ElysiumEngine::MouseIntepreter::GetPosition()
{
	return Point(m_Current.m_X,m_Current.m_Y);
}

void ElysiumEngine::MouseIntepreter::handleMessage(MouseStateMessage &message )
{
	/*if(message.m_MouseButton < 6)
		m_Current.m_MouseButtonState[message.m_MouseButton] = message.m_Down;

	m_Current.m_X = message.m_X;
	m_Current.m_Y = message.m_Y;*/
}

void ElysiumEngine::MouseIntepreter::SetPosition( const ElysiumEngine::Point &pt )
{
	m_Current.m_X = pt.x;
	m_Current.m_Y = pt.y;
}

ElysiumEngine::Point ElysiumEngine::MouseIntepreter::GetPositionPrevious()
{
	return Point(m_Previous.m_X,m_Previous.m_Y);
}

ElysiumEngine::Point ElysiumEngine::MouseIntepreter::GetPositionChange()
{
	return Point(m_Current.m_X - m_Previous.m_X, m_Current.m_Y - m_Previous.m_Y);
}

void ElysiumEngine::InputSystem::loadInputConfig(std::string filename)
{
    FileIO::XMLReader reader;
    
    if(!reader.Open(filename.c_str()))
    {
        std::cout << "Cannot load input configuration no input will be enabled.\n";
        return;
    }
       
    reader.GetFirstChild();
    while(reader.ReadNextChild())
    {
        std::string pos,neg,key,action;
        auto interpreter = m_Interpreters.find(reader.GetTag());
        if(interpreter != m_Interpreters.end())
        {
            reader.GetFirstChild();
            while(reader.ReadNextChild())
            {
                if(reader.GetTag() == "axis")
                {
                    action = reader.GetText();
                    
                    reader.GetStringValueAtr("pos", &pos);
                    reader.GetStringValueAtr("neg", &neg);
                    
                    interpreter->second->addAxis(action, pos, neg);
                }
                else if(reader.GetTag() == "key")
                {
                    reader.GetStringValueAtr("key", &key);
                    action = reader.GetText();
                    
                    interpreter->second->addKey(action, key);
                }
            }
            reader.GetParent();
        }
    }
    reader.GetParent();
}

bool ElysiumEngine::InputSystem::getKeyPressed(std::string action)
{
    bool pressed = false;
    for(auto intepreter : m_Interpreters)
    {
        pressed = pressed || intepreter.second->isPressed(action);
    }
    return pressed;
}

bool ElysiumEngine::InputSystem::getKeyReleased(std::string action)
{
    bool released = false;
    for(auto interpreter : m_Interpreters)
    {
        released = released || interpreter.second->isReleased(action);
    }
    return released;
}


bool ElysiumEngine::InputSystem::getKeyTriggered(std::string action)
{
    bool triggered = false;
    for(auto interpreter : m_Interpreters)
    {
        triggered = triggered || interpreter.second->isTriggered(action);
    }
    return triggered;
}

float ElysiumEngine::InputSystem::getAxis(std::string axis)
{
    float result = -2.0;
    
    for(auto intepreter : m_Interpreters)
    {
        result = ElysiumEngine::fmax(result, intepreter.second->getAxis(axis));
    }
    
    return result == -2.0 ? 0.0f : result;
}

void ElysiumEngine::KeyboardInterpreter::addKey(const std::string &action, const std::string &key)
{
    keys[action] = key[0];
}

void ElysiumEngine::KeyboardInterpreter::addAxis(const std::string &axis, const std::string &pos, const std::string &neg)
{
    Axis toAdd;
    toAdd.postiveKey = pos[0];
    toAdd.negativeKey = neg[0];
    
    axes[axis] = toAdd;
}

float ElysiumEngine::KeyboardInterpreter::getAxis(const std::string &axis)
{
    auto ax = axes.find(axis);
    if(ax == axes.end())
        return -2.0f;
    else
    {
        if(isPressed(ax->second.postiveKey))
            return 1.0f;
        else if(isPressed(ax->second.negativeKey))
            return -1.0f;
        else
            return 0.0f;
    }
}

ElysiumEngine::FunctionResponse::FunctionResponse(void(*function)()) : function(function)
{
}

void ElysiumEngine::FunctionResponse::onAction()
{
	function();
}

void ElysiumEngine::InputSystem::bindKeyPressed(std::string action, void(*function)())
{
	responses[action].push_back(new FunctionResponse(function));
}

