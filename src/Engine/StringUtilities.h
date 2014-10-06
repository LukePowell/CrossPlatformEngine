/******************************************************************************/
/*!
All content � 2012 DigiPen (USA) Corporation, all rights reserved
\file   StringUtilities.h
\author Luke Powell
\par    Course: GAM300
*/
/******************************************************************************/

#pragma  once
#include <vector>
#include <string>

namespace StringUtilities
{
	void VectorizeString(std::vector<std::string> &output, const std::string &input);
	void VectorizeString(std::vector<std::string> &output, const std::string &input, char delimiter );

	void ConvertString(const std::string &stringVal, float *numVal);
	void ConvertString(const std::string &stringVal, double *numVal);
	void ConvertString(const std::string &stringVal, bool *numVal);
	void ConvertString(const std::string &stringVal, int *numVal);
	void ConvertString(const std::string &stringVal, std::string *charVal);

	void LowerString(std::string & lowerMe);
	std::string LowerString(const char *str);
}