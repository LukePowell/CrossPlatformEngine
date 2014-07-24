#include <Engine/WindowingSystem.cpp>

int main(int argc, char **argv)
{
	ElysiumEngine::WindowingSystem windowingSytem = new ElysiumEngine::WindowingSystem();

	windowingSystem.createWindow("PHY 399 Demo",800,600);

	bool quit = false;

	while(!quit)
	{
		windowingSystem.update(1.0f/60.0f);
	}
}