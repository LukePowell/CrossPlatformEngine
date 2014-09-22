#pragma once
#include "ISystem.h"
#include "TemplateConstraints.h"

#include <vector>

namespace ElysiumEngine
{
    class IWindow
    {
    public:
        IWindow(std::string title, int resX, int resY, bool fullscreen);
        virtual ~IWindow(){};

        virtual bool create() = 0;
        virtual void update(float dt) = 0;
        
        void setTitle(std::string title);
        
        virtual bool isOpen() = 0;
        virtual void redraw(){}
        
        int getResX(){return resX;}
        int getResY(){return resY;}
    protected:
        virtual void onSetTitle(std::string title) = 0;
    	std::string title;
        int resX, resY;
        bool fullscreen;
    };

    class IWindowAllocator
    {
    public:
    	virtual IWindow *allocate(std::string title, int resX, int resY, bool fullscreen) = 0;

		virtual void InitalizeSubSystems() = 0;


    	virtual ~IWindowAllocator(){}
    };

    template<typename T>
    class TWindowAllocator : public IWindowAllocator
    {
    public:
    	IWindow* allocate(std::string title, int resX, int resY, bool fullscreen)
    	{
    		Derives<T,IWindow>();

    		return new T(title, resX, resY, fullscreen);
    	}

		void InitalizeSubSystems()
		{
			T::InitSubSystems();
		}
    };

	class WindowingSystem : public ISystem
	{
	public:
		WindowingSystem(IWindowAllocator *allocator);
		~WindowingSystem();
		IWindow  *createWindow(std::string title, int resX, int resY, bool fullscreen = false);

		void update(float dt);
		void onStart();
		void onExit();

		void InitializeSubSystems();

		private:
            std::vector<IWindow *> windows;
            IWindowAllocator *allocator;
	};
}