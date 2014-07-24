/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior written 
consent of DigiPen Institute of Technology is prohibited. 
File Name: GraphicsDebug.cpp
Purpose: Implementation of OpenGL error checking according to spec
Language: C++
Platform: MSVC 2008
		  Windows 7
Project: CS300 Project 1			
Author: Luke Powell, luke.powell, 50002009
Creation date: 2012/02/15
End Header --------------------------------------------------------*/
//Simple implementation of a looped OpenGL check function will only print out values 
//if there is an error if there is then it wil print out erors 
//until it GL_NO_ERROR is returned.
#include "GraphicsDebug.h"
#include <iostream>
#include "OpenGLHeaders.hpp"

void CheckGLError(const char *filename, unsigned int linenumber, const char *line)
{
	unsigned int Error = glGetError();
	while(Error != GL_NO_ERROR)
	{
		std::cout << "GLError: " << Error << " " << gluErrorString(Error) << " on line: " << linenumber << " in file: " << filename << " " << line << std::endl;
		Error = glGetError();//Get the next error if there is any
	}
}

