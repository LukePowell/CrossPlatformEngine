/* Start Header -------------------------------------------------------
Copyright (C) 2011 DigiPen Institute of Technology. 
Reproduction or disclosure of this file or its contents without the prior written 
consent of DigiPen Institute of Technology is prohibited. 
File Name: GraphicsDebug.h
Purpose: Declares GLCall debug macro and CheckGLError function
Language: C++
Platform: MSVC 2008
		  Windows 7
Project: CS300 Project 1			
Author: Luke Powell, luke.powell, 50002009
Creation date: 2012/02/15
End Header --------------------------------------------------------*/
#pragma  once

void CheckGLError(const char * filename, unsigned int linenumber, const char *line);

//Simple macro wrapper around gl error handling simply place the function call 
//in the macro and in debg will build to check and release will build to 
//just the function call.
#ifndef _DEBUG
#pragma message("Calls to OpenGL will be checked.")
#define GLCall(x) do {x; CheckGLError(__FILE__, __LINE__, #x);} while (0)
#else
#pragma message("Calls to OpenGL will not be checked.")
#define GLCall(x) x
#endif