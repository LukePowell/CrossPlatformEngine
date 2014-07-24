/******************************************************************************/
/*!
All content � 2012 DigiPen (USA) Corporation, all rights reserved
\file   XMLReader.cpp
\author Luke Powell
\par    Course: GAM300
*/
/******************************************************************************/

#include "XMLReader.h"
#include <sstream>
#include <assert.h>
#include <iostream>

FileIO::XMLVisitor::XMLVisitor(FileIO::IReaderVistor visitor) : visitor(visitor)
{
}

void FileIO::XMLReaderSerializer::vist(std::string name, IReaderVistor visitor)
{
    tinyxml2::XMLDocument document;
    document.LoadFile(name.c_str());
    XMLVisitor *xmlVisitor = new XMLVisitor(visitor);
    document.Accept(xmlVisitor);
    delete xmlVisitor;
}


bool FileIO::XMLVisitor::VistEnter(const tinyxml2::XMLDocument &doc)
{
    return true;
}

bool FileIO::XMLVisitor::VistExit(const tinyxml2::XMLDocument &doc)
{
    return true;
}

bool FileIO::XMLVisitor::VistEnter(const tinyxml2::XMLElement &element, const tinyxml2::XMLAttribute *attributes)
{
    //XMLElement xmlElement = new XMLElement(element);
    //visitor.vistEnter(xmlElement);
    return true;
}

bool FileIO::XMLReader::Open(const char *filename)
{
	int Error = m_Document.LoadFile(filename);

	if(Error)
	{
        std::stringstream ErrorMsg;
        FILE* check = fopen(filename,"rt");
        if(!check)
        {
            ErrorMsg << "Cannot open the xml file: " << filename;
        }
        else
        {
            fclose(check);
            ErrorMsg << "Invalid XML formatting: " << filename << std::endl;
            ErrorMsg << m_Document.GetErrorStr1() << std::endl;
            //ErrorMsg << m_Document.GetErrorStr2();
            
            std::cout << ErrorMsg.str();
        }
        
		return false;
	}
	

	m_Root = m_Document.RootElement();
	if(m_Root)
	{
		m_Root = m_Document.FirstChildElement();
	}
    
    m_CurrentParent = nullptr;
    m_CurrentChild = m_Document.RootElement();
	
	return true;
}

void FileIO::XMLReader::Close()
{
}

void FileIO::XMLReader::GetUnsignedIntAtr(const char *name, unsigned int *valueToSet)
{
	if(m_CurrentChild)
	{
		//TODO: Add in error checking code here
		int error = m_CurrentChild->QueryUnsignedAttribute(name,valueToSet);
	}
}

void FileIO::XMLReader::GetIntValueAtr(const char *name, int *valueToSet)
{
	if(m_CurrentChild)
	{
		int error = m_CurrentChild->QueryIntAttribute(name,valueToSet);
	}
}

void FileIO::XMLReader::GetFloatValueAtr(const char *name, float *valueToSet)
{
	if(m_CurrentChild)
	{
		int error = m_CurrentChild->QueryFloatAttribute(name,valueToSet);
	}
}

void FileIO::XMLReader::GetDoubleValueAtr(const char *name, double *valueToSet)
{
	if(m_CurrentChild)
	{
		int error = m_CurrentChild->QueryDoubleAttribute(name,valueToSet);
	}
}

void FileIO::XMLReader::GetStringValueAtr(const char *name, std::string *valueToSet)
{
	if(m_CurrentChild)
	{
		const char *Value = m_CurrentChild->Attribute(name);
		if(Value)
			valueToSet->assign(Value);
	}
}

std::string FileIO::XMLReader::GetText()
{
	return m_CurrentChild->GetText();
}

std::string FileIO::XMLReader::GetTag()
{
	if(m_CurrentChild)
		return m_CurrentChild->Name();
	else
		return "INVALID NAME!!!";
}

bool FileIO::XMLReader::ReadNextChild()
{

    if(m_CurrentChild == nullptr)
    {
        m_CurrentChild = m_CurrentParent->FirstChildElement();
    }
    else
    {
        m_CurrentChild = m_CurrentChild->NextSiblingElement();
    }
    
    if(m_CurrentChild)
        return true;
    else
        return false;
}

bool FileIO::XMLReader::ReadNextSibling()
{
    tinyxml2::XMLElement *element = m_CurrentChild->NextSiblingElement();
    
    if(element)
    {
        m_CurrentChild = element;
        return true;
    }
    else
    {
        return false;
    }
}

bool FileIO::XMLReader::ReadNextElement()
{
	if(!m_CurrentParent &&  m_Root)
		m_CurrentParent = m_Root->FirstChildElement();
	else
		m_CurrentParent = m_CurrentParent->NextSiblingElement();

	if(m_CurrentParent)
		return true;
	else
		return false;
}

FileIO::XMLReader::XMLReader() :m_Document(nullptr), m_Root(nullptr), m_CurrentParent(nullptr), m_CurrentChild(nullptr)
{

}

void FileIO::XMLReader::GetRoot()
{
	m_CurrentParent = m_Document.RootElement();
}

void FileIO::XMLReader::GetFirstChild()
{
    m_CurrentParent = m_CurrentChild;
    m_CurrentChild = nullptr;
}

void FileIO::XMLReader::GetBoolValueAttr( const char *name, bool *value )
{
	if(m_CurrentChild)
	{
		m_CurrentChild->QueryBoolAttribute(name,value);
	}
}

void FileIO::XMLReader::GetParent()
{
    m_CurrentChild = m_CurrentParent;
    m_CurrentParent = m_CurrentParent->Parent()->ToElement();
}