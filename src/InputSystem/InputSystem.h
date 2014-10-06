#pragma  once
#include <map>
#include <vector>
#include "ISystem.h"
#include "GameObject.h"
#include "Messaging.h"

namespace ElysiumEngine
{
	namespace Controls
	{
		//Analog controls for GamePads
		const unsigned int E_LEFT_STICK_X = 0;
		const unsigned int E_LEFT_STICK_Y = 1;
		const unsigned int E_RIGHT_STICK_X = 2;
		const unsigned int E_RIGHT_STICK_Y = 3;
		const unsigned int E_LEFT_TRIGGER = 4;
		const unsigned int E_RIGHT_TRIGGER = 5;
		//Mouse input
		const unsigned int E_LEFT_MOUSE_BUTTON = 0;
		const unsigned int E_RIGHT_MOUSE_BUTTON = 1;
		const unsigned int E_MIDDLE_MOUSE_BUTTON = 2;
		const unsigned int E_FOURTH_MOUSE_BUTTON = 3;
		const unsigned int E_FIFTH_MOUSE_BUTTON = 4;
	};

	class IInterpreter;
    
	class IController : public IComponent
	{
	public: 
		IController(std::string name);
		virtual ~IController();
        
		virtual void update(float dt) = 0;
	};

	//Generates actions for IController derivations
	class IInterpreter : public IEventListener
	{
	public:
        IInterpreter(std::string name) : name(name) {}
		//Other stuffs
		void Update(float dt);
        const std::string &getName(){return name;}

		//Per-interpreter update loop
		virtual void OnUpdate(float dt) = 0;
		virtual bool isTriggered(const std::string &action) = 0;
		virtual bool isPressed(const std::string &action) = 0;
		virtual bool isReleased(const std::string &action) = 0;
		virtual bool isAnalog(const std::string &action) = 0;

		virtual float getAxis(const std::string &action) = 0;

		virtual IEventListener *getParent();
		virtual void sendToChildren(IMessage *message );

		virtual void addKey(const std::string &action, const std::string &key) = 0;
		virtual void addAxis(const std::string &axis, const std::string &pos, const std::string &neg) = 0;
    private:
        std::string name;
	};
	
	//Means to enable feedback to the user such as rumble or other means 
	class IFeedBack
	{

	};

	class KeyboardInterpreter : public IInterpreter
	{
	public:
		KeyboardInterpreter();
		void handleMessage(KeyStateMessage &message);

		virtual void OnUpdate(float dt);
		virtual bool isTriggered(const std::string &action);
		virtual bool isPressed(const std::string &action);
		virtual bool isReleased(const std::string &action);
		virtual bool isAnalog(const std::string &action){return false;}

		virtual void addKey(const std::string &action, const std::string &key);
        virtual void addAxis(const std::string &axis, const std::string &pos, const std::string &neg);
        
		virtual float getAxis(const std::string &action);
	private:
        bool isTriggered(unsigned int input);
        bool isPressed(unsigned int input);
		bool isReleased(unsigned int input);
		struct Axis
		{
			unsigned int postiveKey;
			unsigned int negativeKey;
		};
        
        std::map<std::string, unsigned int> keys;
        std::map<std::string, Axis> axes;
        
		bool m_Current[256];
		bool m_Previous[256];
	};

	class MouseState
	{
	public:
		bool m_MouseButtonState[5];
		long m_X, m_Y;
	};


	struct Point
	{
		Point(long x, long y) : x(x),y(y) {}
		long x,y;
	};

	class MouseIntepreter : public IInterpreter
	{
	public:
		MouseIntepreter();
		Point GetPosition();
		Point GetPositionPrevious();
		Point GetPositionChange();
		void SetPosition(const Point &pt);
		virtual void handleMessage(MouseStateMessage &message);

		virtual void OnUpdate(float dt);
		virtual bool isTriggered(const std::string &action);
		virtual bool isPressed(const std::string &action);
		virtual bool isReleased(const std::string &action);
		virtual bool isAnalog(const std::string &action);

        virtual void addAxis(const std::string &axis, const std::string &pos, const std::string &neg){};
		virtual void addKey(const std::string &action, const std::string &key){}
		virtual float getAxis(const std::string &action){return -2.0f;}
	private:
		MouseState m_Current;
		MouseState m_Previous;
	};

	class InputSystem : public ISystem
	{
	public:
		InputSystem();

		virtual void update(float dt);
		virtual void initialize(){};
        
		virtual IEventListener * getParent();
		virtual void sendToChildren(IMessage *message );

        IInterpreter &getInterpreter(std::string name);
        void addInterpreter(IInterpreter *intepreter);

		Point GetMousePosition();
		void SetMousePosition(const Point &pt);

		void Register(IController *controller);
		void Unregister(IController *controller);
        
        
        void loadInputConfig(std::string filename);
        
        float getAxis(std::string axis);
        bool getKeyPressed(std::string action);
        bool getKeyTriggered(std::string action);
        bool getKeyReleased(std::string action);
        
		//TODO: switch to a binding method
		//void bindKeyPressed(std::string action, (void)(function *)());
	private:
        KeyboardInterpreter keyboardInterpreter;
        MouseIntepreter mouseInterpreter;
        
        std::map<std::string, IInterpreter *> m_Interpreters;
		std::vector<IController *> m_Controllers;
	};

	extern InputSystem *g_InputSystem;
}