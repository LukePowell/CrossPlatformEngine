/******************************************************************************/
/*!
All content � 2012 DigiPen (USA) Corporation, all rights reserved
\file   XMLWriter.cpp
\author Luke Powell
\par    Course: GAM300
*/
/******************************************************************************/
#include "XMLWriter.h"


FileIO::XMLWriter::XMLWriter() : m_FileOut(nullptr)
{

}

bool FileIO::XMLWriter::Open(const char *filename)
{
	m_FileOut = std::fopen(filename,"wt");
	m_Filename = filename;
		
	m_XMLPrinter = new tinyxml2::XMLPrinter(m_FileOut);
	m_XMLPrinter->PushHeader(false,true);

	return m_FileOut != nullptr;
}

void FileIO::XMLWriter::Output()
{
	m_XMLDoc.Print(m_XMLPrinter);
	std::fclose(m_FileOut);
}

void FileIO::XMLWriter::WriteUnsignedIntAttr( const char *name, unsigned int value )
{
	m_XMLPrinter->PushAttribute(name,value);
}

void FileIO::XMLWriter::WriteIntValueAttr( const char *name, int value )
{
	m_XMLPrinter->PushAttribute(name,value);
}

void FileIO::XMLWriter::WriteFloatValueAttr( const char *name, float value )
{
	m_XMLPrinter->PushAttribute(name,value);
}

void FileIO::XMLWriter::WriteDoubleValueAttr( const char *name, double value )
{
	m_XMLPrinter->PushAttribute(name,value);
}

void FileIO::XMLWriter::WriteStringValueAttr( const char *name, std::string value )
{
	m_XMLPrinter->PushAttribute(name,value.c_str());
}

void FileIO::XMLWriter::WriteText(std::string text)
{
	m_XMLPrinter->PushText(text.c_str());
}

void FileIO::XMLWriter::OpenTag(const char *tag)
{
	m_XMLPrinter->OpenElement(tag);
}

void FileIO::XMLWriter::CloseTag()
{
	m_XMLPrinter->CloseElement();
}

FileIO::XMLWriter::~XMLWriter()
{
	Output();
}
