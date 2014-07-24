//
//  OpenGLHeaders.h
//  SFMLTest
//
//  Created by Luke Powell on 2/6/14.
//
//

#ifndef SFMLTest_OpenGLHeaders_h
#define SFMLTest_OpenGLHeaders_h

#ifdef _DESKTOP_//Windows,Mac,Linux
#include "SFML/OpenGL.hpp"
#endif

//Will need to test these settings first need to work on setting
//up an iOS wrapper
//TODO: wee
#ifdef _iOS_ //Apple iPhone and iPod
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

#ifdef _ANDROID_ //Android phones and tablets
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#endif
