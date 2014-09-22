# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# For each target create a dummy rule so the target does not have to exist
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/WindowingSystem/Debug/libWindowingSystem.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SFMLWindow/Debug/libWindow.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SDLWindow/Debug/libSDLWindow.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/Debug/libCoreEngine.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/GraphicsSystem/Debug/libGraphics.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/PhysicsSystem/Debug/libPhysics.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/InputSystem/Debug/libInput.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Maths/Debug/libMaths.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/TinyXML2/Debug/libXML.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/WindowingSystem/MinSizeRel/libWindowingSystem.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SFMLWindow/MinSizeRel/libWindow.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SDLWindow/MinSizeRel/libSDLWindow.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/MinSizeRel/libCoreEngine.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/GraphicsSystem/MinSizeRel/libGraphics.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/PhysicsSystem/MinSizeRel/libPhysics.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/InputSystem/MinSizeRel/libInput.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Maths/MinSizeRel/libMaths.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/TinyXML2/MinSizeRel/libXML.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/WindowingSystem/RelWithDebInfo/libWindowingSystem.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SFMLWindow/RelWithDebInfo/libWindow.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SDLWindow/RelWithDebInfo/libSDLWindow.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/RelWithDebInfo/libCoreEngine.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/GraphicsSystem/RelWithDebInfo/libGraphics.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/PhysicsSystem/RelWithDebInfo/libPhysics.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/InputSystem/RelWithDebInfo/libInput.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Maths/RelWithDebInfo/libMaths.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/TinyXML2/RelWithDebInfo/libXML.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/WindowingSystem/Release/libWindowingSystem.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SFMLWindow/Release/libWindow.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SDLWindow/Release/libSDLWindow.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/Release/libCoreEngine.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/GraphicsSystem/Release/libGraphics.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/PhysicsSystem/Release/libPhysics.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/InputSystem/Release/libInput.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Maths/Release/libMaths.a:
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/TinyXML2/Release/libXML.a:


# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.SFMLTest.Debug:
PostBuild.WindowingSystem.Debug: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Window.Debug: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.SDLWindow.Debug: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.CoreEngine.Debug: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Graphics.Debug: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Physics.Debug: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Input.Debug: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Maths.Debug: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.CoreEngine.Debug: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.XML.Debug: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest:\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/WindowingSystem/Debug/libWindowingSystem.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SFMLWindow/Debug/libWindow.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SDLWindow/Debug/libSDLWindow.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/Debug/libCoreEngine.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/GraphicsSystem/Debug/libGraphics.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/PhysicsSystem/Debug/libPhysics.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/InputSystem/Debug/libInput.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Maths/Debug/libMaths.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/Debug/libCoreEngine.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/TinyXML2/Debug/libXML.a
	/bin/rm -f /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest


PostBuild.CoreEngine.Debug:
PostBuild.WindowingSystem.Debug:
PostBuild.Window.Debug:
PostBuild.XML.Debug:
PostBuild.Graphics.Debug:
PostBuild.Physics.Debug:
PostBuild.Maths.Debug:
PostBuild.Input.Debug:
PostBuild.SDLWindow.Debug:
PostBuild.SFMLTest.Release:
PostBuild.WindowingSystem.Release: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Window.Release: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.SDLWindow.Release: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.CoreEngine.Release: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Graphics.Release: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Physics.Release: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Input.Release: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Maths.Release: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.CoreEngine.Release: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.XML.Release: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest:\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/WindowingSystem/Release/libWindowingSystem.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SFMLWindow/Release/libWindow.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SDLWindow/Release/libSDLWindow.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/Release/libCoreEngine.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/GraphicsSystem/Release/libGraphics.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/PhysicsSystem/Release/libPhysics.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/InputSystem/Release/libInput.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Maths/Release/libMaths.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/Release/libCoreEngine.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/TinyXML2/Release/libXML.a
	/bin/rm -f /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest


PostBuild.CoreEngine.Release:
PostBuild.WindowingSystem.Release:
PostBuild.Window.Release:
PostBuild.XML.Release:
PostBuild.Graphics.Release:
PostBuild.Physics.Release:
PostBuild.Maths.Release:
PostBuild.Input.Release:
PostBuild.SDLWindow.Release:
PostBuild.SFMLTest.MinSizeRel:
PostBuild.WindowingSystem.MinSizeRel: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Window.MinSizeRel: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.SDLWindow.MinSizeRel: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.CoreEngine.MinSizeRel: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Graphics.MinSizeRel: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Physics.MinSizeRel: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Input.MinSizeRel: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Maths.MinSizeRel: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.CoreEngine.MinSizeRel: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.XML.MinSizeRel: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest:\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/WindowingSystem/MinSizeRel/libWindowingSystem.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SFMLWindow/MinSizeRel/libWindow.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SDLWindow/MinSizeRel/libSDLWindow.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/MinSizeRel/libCoreEngine.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/GraphicsSystem/MinSizeRel/libGraphics.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/PhysicsSystem/MinSizeRel/libPhysics.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/InputSystem/MinSizeRel/libInput.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Maths/MinSizeRel/libMaths.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/MinSizeRel/libCoreEngine.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/TinyXML2/MinSizeRel/libXML.a
	/bin/rm -f /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest


PostBuild.CoreEngine.MinSizeRel:
PostBuild.WindowingSystem.MinSizeRel:
PostBuild.Window.MinSizeRel:
PostBuild.XML.MinSizeRel:
PostBuild.Graphics.MinSizeRel:
PostBuild.Physics.MinSizeRel:
PostBuild.Maths.MinSizeRel:
PostBuild.Input.MinSizeRel:
PostBuild.SDLWindow.MinSizeRel:
PostBuild.SFMLTest.RelWithDebInfo:
PostBuild.WindowingSystem.RelWithDebInfo: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Window.RelWithDebInfo: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.SDLWindow.RelWithDebInfo: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.CoreEngine.RelWithDebInfo: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Graphics.RelWithDebInfo: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Physics.RelWithDebInfo: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Input.RelWithDebInfo: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.Maths.RelWithDebInfo: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.CoreEngine.RelWithDebInfo: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
PostBuild.XML.RelWithDebInfo: /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest
/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest:\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/WindowingSystem/RelWithDebInfo/libWindowingSystem.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SFMLWindow/RelWithDebInfo/libWindow.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/SDLWindow/RelWithDebInfo/libSDLWindow.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/RelWithDebInfo/libCoreEngine.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/GraphicsSystem/RelWithDebInfo/libGraphics.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/PhysicsSystem/RelWithDebInfo/libPhysics.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/InputSystem/RelWithDebInfo/libInput.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Maths/RelWithDebInfo/libMaths.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/Engine/RelWithDebInfo/libCoreEngine.a\
	/Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/solution/src/TinyXML2/RelWithDebInfo/libXML.a
	/bin/rm -f /Users/lukepowell/Documents/clean_crossplatformengine/git/CrossPlatformEngine/assets/SFMLTest


PostBuild.CoreEngine.RelWithDebInfo:
PostBuild.WindowingSystem.RelWithDebInfo:
PostBuild.Window.RelWithDebInfo:
PostBuild.XML.RelWithDebInfo:
PostBuild.Graphics.RelWithDebInfo:
PostBuild.Physics.RelWithDebInfo:
PostBuild.Maths.RelWithDebInfo:
PostBuild.Input.RelWithDebInfo:
PostBuild.SDLWindow.RelWithDebInfo:
