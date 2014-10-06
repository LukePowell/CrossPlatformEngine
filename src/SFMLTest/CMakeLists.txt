FILE(GLOB files *.cpp *.h)

add_executable(SFMLTest ${files})


FIND_PACKAGE(OpenGL REQUIRED)
FIND_LIBRARY(sdl SDL2)

IF(MSVC)
	target_link_libraries (SFMLTest glew32.lib SDL2.lib SDL2main.lib ${OPENGL_LIBRARIES} WindowingSystem SDLWindow CoreEngine Graphics Physics Input)
ENDIF(MSVC)

IF(UNIX)
	IF(APPLE)	
		target_link_libraries(SFMLTest  ${sdl} ${OPENGL_LIBRARIES} WindowingSystem  SDLWindow CoreEngine Graphics Physics Input)
	ENDIF(APPLE)
ENDIF(UNIX)