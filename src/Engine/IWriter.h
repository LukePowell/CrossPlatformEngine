/******************************************************************************/
/*!
All content © 2012 DigiPen (USA) Corporation, all rights reserved
\file   IWriter.h
\author Luke Powell
\par    Course: GAM300
*/
/******************************************************************************/

#pragma  once
#include <string>

namespace FileIO
{
	class IWriter
	{
	public:
		virtual ~IWriter(){};
		//Open the file in some manner
		virtual bool Open(const char *filename)  = 0;
		//Final call to dump all the data from internal format to output
		virtual void Output() = 0;

		//output functions
		virtual void WriteUnsignedIntAttr(const char *name, unsigned int value)=0;
		virtual void WriteIntValueAttr(const char *name, int value)=0;
		virtual void WriteFloatValueAttr(const char *name, float value)=0;
		virtual void WriteDoubleValueAttr(const char *name, double value)=0;
		virtual void WriteStringValueAttr(const char *name, std::string value)=0;
		virtual void WriteText(std::string text)=0;
		virtual void OpenTag(const char *tag)=0;
		virtual void CloseTag()=0;
	};
}
