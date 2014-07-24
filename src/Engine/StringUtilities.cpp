/******************************************************************************/
/*!
All content � 2012 DigiPen (USA) Corporation, all rights reserved
\file   StringUtilities.cpp
\author Luke Powell
\par    Course: GAM300
*/
/******************************************************************************/

#include "StringUtilities.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <stdlib.h>

void StringUtilities::VectorizeString( std::vector<std::string> &output, const std::string &input )
{
	std::string Temp;
	bool InQuotes = false;

	for(auto It = input.begin(); It != input.end(); ++It)
	{

		if(*It == '\"')
			InQuotes = !InQuotes;
		else if(*It != ' ' || (InQuotes == true && *It != '\"'))
		{
			Temp += *It;
		}
		else
		{
			output.push_back(Temp);
			Temp.clear();
		}
	}

	if(!Temp.empty())
	{
		output.push_back(Temp);
	}
}
void StringUtilities::VectorizeString( std::vector<std::string> &output, const std::string &input, char delimiter )
{
	std::string Temp;
	bool InQuotes = false;

	for(auto It = input.begin(); It != input.end(); ++It)
	{

		if(*It == '\"')
			InQuotes = !InQuotes;
		else if(*It != delimiter)
		{
			Temp += *It;
		}
		else
		{
			output.push_back(Temp);
			Temp.clear();
		}
	}

	if(!Temp.empty())
	{
		output.push_back(Temp);
	}
}
void StringUtilities::ConvertString( const std::string &stringVal, float *numVal )
{
	*numVal = static_cast<float>(atof(stringVal.c_str()));
}

void StringUtilities::ConvertString( const std::string &stringVal, double *numVal )
{
	*numVal = atof(stringVal.c_str());
}

void StringUtilities::ConvertString( const std::string &stringVal, bool *numVal )
{
	if(!isdigit(stringVal.c_str()[0]))
	{
		std::string Temp = stringVal;
		LowerString(Temp);
		if(Temp == "true")
			*numVal = true;
		else
			*numVal = false;
	}
	else
	{
		if(stringVal.c_str()[0] == '0')
			*numVal = false;
		else
			*numVal = true;
	}
}


void StringUtilities::ConvertString( const std::string &stringVal, int *numVal )
{
	*numVal = atoi(stringVal.c_str());
}

void StringUtilities::ConvertString( const std::string &stringVal, std::string *charVal )
{
	*charVal = stringVal;
}

void StringUtilities::LowerString( std::string &lowerMe )
{
	std::transform(lowerMe.begin(),lowerMe.end(),lowerMe.begin(),std::tolower);
}
