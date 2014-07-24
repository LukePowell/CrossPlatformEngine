/******************************************************************************/
/*!
All content � 2012 DigiPen (USA) Corporation, all rights reserved
\file   IReader.h
\author Luke Powell
\par    Course: GAM300
*/
/******************************************************************************/

#pragma  once
#include <string>

namespace FileIO
{
    class IReaderElement
    {
    public:
        virtual ~IReaderElement(){}
        
        //Get attributes for all values here
        /*!********************************************
         *\brief If name indicates a valid parameter then
         *       the value of valueToSet will be set to that value
         *       else the value of valueToSet will be untouched
         *\param  name
         *        The name of the attribute to attempt to get a value from
         *\param valueToSet
         *       Pointer to the value to set.  If name is not a valid attribute
         *       then nothing happens leaving valueToSet this will allow default
         *       values for all fields cleanly and concisely
         **********************************************/
        virtual void GetUnsignedIntAtr(const char *name, unsigned int *valueToSet) = 0;

        /*!********************************************
         *\brief If name indicates a valid parameter then
         *       the value of valueToSet will be set to that value
         *       else the value of valueToSet will be untouched
         *\param  name
         *        The name of the attribute to attempt to get a value from
         **********************************************/
        virtual void GetIntValueAtr(const char *name, int *valueToSet) = 0;

        /*!********************************************
         *\brief If name indicates a valid parameter then
         *       the value of valueToSet will be set to that value
         *       else the value of valueToSet will be untouched
         *\param  name
         *        The name of the attribute to attempt to get a value from
         **********************************************/
        virtual void GetFloatValueAtr(const char *name, float *valueToSet) = 0;
        /*!********************************************
         *\brief If name indicates a valid parameter then
         *       the value of valueToSet will be set to that value
         *       else the value of valueToSet will be untouched
         *\param  name
         *        The name of the attribute to attempt to get a value from
         **********************************************/
        virtual void GetDoubleValueAtr(const char *name, double *valueToSet) = 0;

        /*!********************************************
         *\brief If name indicates a valid parameter then
         *       the value of valueToSet will be set to that value
         *       else the value of valueToSet will be untouched
         *\param  name
         *        The name of the attribute to attempt to get a value from
         **********************************************/
        virtual void GetStringValueAtr(const char *name, std::string *valueToSet) = 0;

        virtual void GetBoolValueAttr(const char *name, bool *value) = 0;

        //Get Value
        /*!********************************************
         *\brief
         *\param
         *\return
         **********************************************/
        virtual std::string GetText() = 0;

        //GetTag
        /*!********************************************
         *\brief
         *\param
         *\return
         **********************************************/
        virtual std::string GetTag() = 0;
    };

    class IReaderVistor
    {
    public:
        void vistEnter(IReaderElement &element);
        void vistExit(IReaderElement &element);
    };
    
    class IReaderSerializer
    {
    public:
        void vist(std::string name, IReaderVistor vistor);
    };
    
	class IReader
	{
	public:
		virtual ~IReader(){};

		virtual bool Open(const char *filename) = 0;
		virtual void Close() = 0;

		//Get attributes for all values here
		/*!********************************************
		*\brief If name indicates a valid parameter then
		*       the value of valueToSet will be set to that value
		*       else the value of valueToSet will be untouched
		*\param  name
		*        The name of the attribute to attempt to get a value from
		*\param valueToSet
		*       Pointer to the value to set.  If name is not a valid attribute
		*       then nothing happens leaving valueToSet this will allow default
		*       values for all fields cleanly and concisely
		**********************************************/
		virtual void GetUnsignedIntAtr(const char *name, unsigned int *valueToSet) = 0;

		/*!********************************************
		*\brief If name indicates a valid parameter then
		*       the value of valueToSet will be set to that value
		*       else the value of valueToSet will be untouched
		*\param  name
		*        The name of the attribute to attempt to get a value from
		**********************************************/
		virtual void GetIntValueAtr(const char *name, int *valueToSet) = 0;

		/*!********************************************
		*\brief If name indicates a valid parameter then
		*       the value of valueToSet will be set to that value
		*       else the value of valueToSet will be untouched
		*\param  name
		*        The name of the attribute to attempt to get a value from
		**********************************************/
		virtual void GetFloatValueAtr(const char *name, float *valueToSet) = 0;
		/*!********************************************
		*\brief If name indicates a valid parameter then
		*       the value of valueToSet will be set to that value
		*       else the value of valueToSet will be untouched
		*\param  name
		*        The name of the attribute to attempt to get a value from
		**********************************************/
		virtual void GetDoubleValueAtr(const char *name, double *valueToSet) = 0;

			/*!********************************************
		*\brief If name indicates a valid parameter then
		*       the value of valueToSet will be set to that value
		*       else the value of valueToSet will be untouched
		*\param  name
		*        The name of the attribute to attempt to get a value from
		**********************************************/
		virtual void GetStringValueAtr(const char *name, std::string *valueToSet) = 0;

		virtual void GetBoolValueAttr(const char *name, bool *value) = 0;

		//Get Value
		/*!********************************************
		*\brief 
		*\param  
		*\return 
		**********************************************/
		virtual std::string GetText() = 0;

		//GetTag
		/*!********************************************
		*\brief 
		*\param  
		*\return 
		**********************************************/
		virtual std::string GetTag() = 0;
		
		virtual bool ReadNextChild() = 0;
        virtual bool ReadNextSibling() = 0;
		virtual bool ReadNextElement() = 0;

		virtual void GetRoot() = 0;
		virtual void GetFirstChild() = 0;
        
        virtual void GetParent() = 0;

		/*!********************************************
		*\brief 
		*\param  
		*\return 
		**********************************************/
	//	virtual bool IsValid();
	};
}