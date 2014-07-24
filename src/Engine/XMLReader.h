/******************************************************************************/
/*!
All content � 2012 DigiPen (USA) Corporation, all rights reserved
\file   XMLReader.h
\author Luke Powell
\par    Course: GAM300
*/
/******************************************************************************/

#pragma once
#include "IReader.h"
#include "TinyXML2/tinyxml2.h"

namespace FileIO
{
    class XMLElement : public IReaderElement
    {
    public:
        XMLElement(tinyxml2::XMLElement &element);
    };
    
    class XMLVisitor : public tinyxml2::XMLVisitor
    {
    public:
        XMLVisitor(FileIO::IReaderVistor visitor);
        
        bool VistEnter(const tinyxml2::XMLDocument &doc);
        bool VistExit(const tinyxml2::XMLDocument &doc);
        
        bool VistEnter(const tinyxml2::XMLElement &element, const tinyxml2::XMLAttribute *attributes);
        bool VistExit(const tinyxml2::XMLElement &element);
        
        bool Vist(const tinyxml2::XMLDeclaration &decl);
        bool Vist(const tinyxml2::XMLText& text);
        bool Vist(const tinyxml2::XMLComment& comment);
        bool Vist (const tinyxml2::XMLUnknown& unknown);
    private:
        FileIO::IReaderVistor visitor;
    };
    
    class XMLReaderSerializer : public IReaderSerializer
    {
    public:
        void vist(std::string name, IReaderVistor vistor);
    };
    
	class XMLReader : public IReader
	{
	public:
		XMLReader();

		virtual bool Open(const char *filename);
		virtual void Close();

		virtual void GetUnsignedIntAtr(const char *name, unsigned int *valueToSet);
		virtual void GetIntValueAtr(const char *name, int *valueToSet);

		virtual void GetFloatValueAtr(const char *name, float *valueToSet);
		virtual void GetDoubleValueAtr(const char *name, double *valueToSet);

		virtual void GetStringValueAtr(const char *name, std::string *valueToSet);
		virtual void GetBoolValueAttr(const char *name, bool *value);

		virtual std::string GetText();

		virtual std::string GetTag();

		virtual bool ReadNextChild();
        virtual bool ReadNextSibling();
		virtual bool ReadNextElement();

		void GetRoot();
		void GetFirstChild();
        
        void GetParent();
	private:

		tinyxml2::XMLDocument m_Document;
		tinyxml2::XMLElement *m_Root;
		tinyxml2::XMLElement *m_CurrentParent;
		tinyxml2::XMLElement *m_CurrentChild;
	};
}