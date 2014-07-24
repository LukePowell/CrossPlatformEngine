#pragma once

#define DESKTOP 

#ifdef DESKTOP//Generally can get by with just using SFMl
	#ifdef WIN32
		#include <GL/glew.h>//We need to use GLEW on windows
#endif

	#ifdef __APPLE__
		#include <OpenGL/gl3.h>
		#include <OpenGL/glu.h>
		#include <OpenGL/gl3ext.h>
		#include <GLUT/glut.h>
    #endif

#ifdef _ANDROID_ //Android phones and tablets
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#endif