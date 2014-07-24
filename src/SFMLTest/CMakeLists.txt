FILE(GLOB files *.cpp *.h)
add_executable(SFMLTest ${files})

FIND_PACKAGE(OpenGL REQUIRED)
FIND_LIBRARY(sdl SDL2)

IF(MSVC)
	target_link_libraries (SFMLTest sfml-graphics.lib sfml-window.lib sfml-system.lib glew32.lib SDL2.lib SDL2main.lib ${OPENGL_LIBRARIES} WindowingSystem Window SDLWindow CoreEngine Graphics Physics Input)
ENDIF(MSVC)

IF(UNIX)
	IF(APPLE)	
		target_link_libraries(SFMLTest sfml-window sfml-graphics sfml-system ${sdl} ${OPENGL_LIBRARIES} WindowingSystem Window SDLWindow CoreEngine Graphics Physics Input)
	ENDIF(APPLE)
ENDIF(UNIX)