/******************************************************************************/
/*!
All content � 2012 DigiPen (USA) Corporation, all rights reserved
\file   XMLWriter.h
\author Luke Powell
\par    Course: GAM300
*/
/******************************************************************************/

#pragma  once
#include "IWriter.h"
#include "TinyXML2/tinyxml2.h"

namespace FileIO
{
	class XMLWriter : public IWriter
	{
	public:
		XMLWriter();
		~XMLWriter();
		virtual void Output();
		virtual bool Open( const char *filename);
		virtual void WriteUnsignedIntAttr(const char *name, unsigned int value);
		virtual void WriteIntValueAttr(const char *name, int value);
		virtual void WriteFloatValueAttr(const char *name, float value);
		virtual void WriteDoubleValueAttr(const char *name, double value);
		virtual void WriteStringValueAttr(const char *name, std::string value);
		virtual void WriteText(std::string text);
		virtual void OpenTag(const char *tag);
		virtual void CloseTag();
	private:
		std::string m_Filename;
		tinyxml2::XMLPrinter *m_XMLPrinter;
		tinyxml2::XMLDocument m_XMLDoc;
		FILE *m_FileOut;
	};
}