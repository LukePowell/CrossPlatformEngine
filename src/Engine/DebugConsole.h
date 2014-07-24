/******************************************************************************/
/*!
All content � 2012 DigiPen (USA) Corporation, all rights reserved
\file   DebugConsole.h
\author Luke Powell
\par    Course: GAM300
*/
/******************************************************************************/

#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "StringUtilities.h"

//Debug console interface and the like
namespace DebugLog
{
	enum MessageTypes{E_Error,E_Warning,E_Custom,E_Info, E_Debug};

	class Console;

    //TODO: Replace with static rewrite of console.
	extern Console *g_Console;
    
	//TODO: May want to make the console writer interface to use a ring buffer
	//This will allow some maximum amount of messages, facilate some form of
	//dump every now and then to output to the screen.

	//Some predefined channel definitions
	namespace Channels
	{
		//A few predefined channels 
		const unsigned int None = 0x00000000;
		const unsigned int All =  0xFFFFFFFF;	
		const unsigned int MaxChannels = 32;
	}


	/*!*******************************************************************
	*\class IConsoleWriter Class to abstract away the means text is being
	*       displayed.
	*********************************************************************/
	class IConsoleWriter
	{
	public:
		virtual ~IConsoleWriter(){}

	private:
		
		//Private interface that is can only be called by the Console class
		//formats the message and passes it down to the actual
		void Write(const std::string &message, MessageTypes type);

	protected:
		/*!***********************************
		*\brief Virtual method that abstracts what is actually outputting text
		*\param message Formated message for the writer to display
		*************************************/
		virtual void VWrite(const std::string &message,MessageTypes type) = 0;
		virtual void Clear(){}

		//Allow the Console to access the methods herein this is done to prevent
		//a derivation of this class from being used directly
		friend class Console;
	};

	/*!************************************************************************
	*\class Abstraction of the rendering interface.  This interface will call
	*       into the parent Console after the user has entered a command.
	**************************************************************************/
	class IConsoleReader
	{
	public:
		virtual ~IConsoleReader();
		//void CommandDone(std::string line);
		virtual std::string GetLine() = 0;
	protected:
		Console *m_Parent;
		friend class Console;
	};


	/*!********************************************
	*\class Special class that will do nothing
	*       when a write is requested to the console
	**********************************************/
	class EmptyWriter : public IConsoleWriter
	{
	protected:
		virtual void VWrite(const std::string &message, MessageTypes type){};
	};

	/*!********************************************
	*\class Special class that will not read any data
	*       from the console and will not give any data to the 
	*       console.  Will give back EmptyConsole if a string
	*       is requested.
	**********************************************/
	class EmptyReader : public IConsoleReader
	{
	public:
		virtual std::string GetLine();
	};

	class FileWriter : public IConsoleWriter
	{
	public:
		FileWriter(std::string file);
	protected:
		virtual void VWrite(const std::string &mesage, MessageTypes type);
	    FILE *m_File;
	};

	/*!************************************************************************
	*\class Console class this allows writing to a console regardless of the 
	*       final means that the text gets displayed thanks to an abstraction
	*       on the writer interface
	**************************************************************************/
	class Console
	{
	public:
		//constructor destructor
		Console(IConsoleWriter * writer, IConsoleReader * reader);//Setup what console writer we are going to use
		~Console();//Will need once shared_ptr is up and running

		//Methods to log text to the current writer
		void LogError(const std::string &message, unsigned int channel);
		void LogWarning(const std::string &message, unsigned int channel);
		void LogInfo(const std::string &message, unsigned int channel);
		void LogDebug(const std::string &message, unsigned int channel);
		void LogCustom(const std::string &tag, const std::string &message, unsigned int channel);

		//Input related methods

		//Methods to manipulate writing
		const IConsoleWriter * GetWriter() const;
		void SetWriter(IConsoleWriter * newWriter);

		//Methods to manipulate reading
		const IConsoleReader * GetReader() const;
		void SetReader(IConsoleReader * reader);
		
		void ParseInput(std::string &input);

		//Channel methods
		unsigned int CreateChannel(const std::string &name, unsigned int ID);
		void EnumerateChannels();	
		void SetChannel(unsigned int channel);//Set the channel using the ID
		void SetChannel(const std::string channel);//Set the channel using the name
		void SetDebug(bool value){m_Debug = value;}
		void Run(std::string shellScript);
		void Echo(std::string output);
		//Definitions of the functor interface into the console
		/*!**************************************************************************
		*\class IDebugFunctor Base class allowing you to parse input from the console 
		*       into a usable form.
		****************************************************************************/
		class IDebugFunctor
		{
		public:
			virtual void operator()(std::vector<std::string> arguments) = 0;
		private:
		};

		/*
		   This class is a small utility class to allow the use of lambas and straight
		   function pointers in lieu of a full functor.  This needs to be a derivation 
		   of the main functor class as we cannot have an non-homogeneous array any other
		   way.  This
		 */
		template<typename T>
		class TFunctor : public IDebugFunctor
		{
		public:
			TFunctor(T func) : m_Function(func)
			{
				//m_Function = func;
			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				m_Function(arguments);
			}
		private:
			T m_Function;
		};

		class FFunctor : public IDebugFunctor
		{
		public:
			FFunctor(void (*func)(std::vector<std::string>)) : m_Function(func)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				m_Function(arguments);
			}
		private:
			void (*m_Function)(std::vector<std::string>);
		};

		template<typename returnType>
		class ZeroParameterFunctor : public IDebugFunctor
		{
		public:
			ZeroParameterFunctor(returnType (*function)()) : m_Function(function)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				(*m_Function)();
			}

		private:
			//returnType m_ReturnValue;
			returnType (*m_Function)();
		};

		template<typename returnType, typename param1>
		class OneParameterFunctor : public IDebugFunctor
		{
			public:
				OneParameterFunctor(returnType (*function)(param1)) : m_Function(function)
				{
	
				}
	
				virtual void operator()(std::vector<std::string> arguments)
				{
					if(arguments.size() < 2)
						g_Console->LogError("Insufficient number of arguments",Channels::All);
					else
					{	
						param1 ParamterOne;
						StringUtilities::ConvertString(arguments[1],&ParamterOne);
						(*m_Function)(ParamterOne);
					}
				}
	
			private:
				//returnType m_ReturnValue;
				returnType (*m_Function)(param1);
		};

		template<typename returnType, typename param1, typename param2>
		class TwoParameterFunctor : public IDebugFunctor
		{
		public:
			TwoParameterFunctor(returnType (*function)(param1, param2)) : m_Function(function)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				if(arguments.size() < 3)
					g_Console->LogError("Insufficient number of arguments",Channels::All);
				else
				{	
					param1 ParamterOne;
					StringUtilities::ConvertString(arguments[1],&ParamterOne);
					param2 ParameterTwo;
					StringUtilities::ConvertString(arguments[2],&ParameterTwo);
					(*m_Function)(ParamterOne,ParameterTwo);
				}
			}

		private:
			//returnType m_ReturnValue;
			returnType (*m_Function)(param1,param2);
		};

		template<typename returnType, typename param1, typename param2, typename param3>
		class ThreeParameterFunctor : public IDebugFunctor
		{
		public:
			ThreeParameterFunctor(returnType (*function)(param1,param2,param3)) : m_Function(function)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				if(arguments.size() < 4)
					g_Console->LogError("Insufficient number of arguments",Channels::All);
				else
				{	
					param1 ParamterOne;
					StringUtilities::ConvertString(arguments[1],&ParamterOne);
					param2 ParameterTwo;
					StringUtilities::ConvertString(arguments[2],&ParameterTwo);
					param3 ParameterThree;
					StringUtilities::ConvertString(arguments[3],&ParameterThree);
					(*m_Function)(ParamterOne,ParameterTwo,ParameterThree);
				}
			}

		private:
			//returnType m_ReturnValue;
			returnType (*m_Function)(param1,param2,param3);
		};

		template<typename returnType, typename param1, typename param2, typename param3, typename param4>
		class FourParameterFunctor : public IDebugFunctor
		{
		public:
			FourParameterFunctor(returnType (*function)(param1,param2,param3,param4)) : m_Function(function)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				if(arguments.size() < 5)
					g_Console->LogError("Insufficient number of arguments",Channels::All);
				else
				{	
					param1 ParamterOne;
					StringUtilities::ConvertString(arguments[1],&ParamterOne);
					param2 ParameterTwo;
					StringUtilities::ConvertString(arguments[2],&ParameterTwo);
					param3 ParameterThree;
					StringUtilities::ConvertString(arguments[3],&ParameterThree);
					param4 ParameterFour;
					StringUtilities::ConvertString(arguments[4],&ParameterFour);
					(*m_Function)(ParamterOne,ParameterTwo,ParameterThree,ParameterFour);
				}
			}

		private:
			//returnType m_ReturnValue;
			returnType (*m_Function)(param1,param2,param3,param4);
		};

		template<typename returnType, typename param1, typename param2, typename param3, typename param4, typename param5>
		class FiveParameterFunctor : public IDebugFunctor
		{
		public:
			FiveParameterFunctor(returnType (*function)(param1, param2, param3, param4, param5)) : m_Function(function)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				if(arguments.size() < 6)
					g_Console->LogError("Insufficient number of arguments",Channels::All);
				else
				{	
					param1 ParamterOne;
					StringUtilities::ConvertString(arguments[1],&ParamterOne);
					param2 ParameterTwo;
					StringUtilities::ConvertString(arguments[2],&ParameterTwo);
					param3 ParameterThree;
					StringUtilities::ConvertString(arguments[3],&ParameterThree);
					param4 ParameterFour;
					StringUtilities::ConvertString(arguments[4],&ParameterFour);
					param5 ParameterFive;
					StringUtilities::ConvertString(arguments[5],&ParameterFive);
					(*m_Function)(ParamterOne, ParameterTwo,ParameterThree,ParameterFour,ParameterFive);
				}
			}

		private:
			//returnType m_ReturnValue;
			returnType (*m_Function)(param1,param2,param3,param4,param5);
		};

		//CLASS METHODS
		template<typename returnType, typename classType>
		class ZeroParamClassFunctor : public IDebugFunctor
		{
		public: 
			ZeroParamClassFunctor(returnType (classType::*funcPtr)(void), classType &classInst) : m_Function(funcPtr), m_Class(classInst)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				//We can ignore arguments here
				(m_Class.*m_Function)();
			}

		private:
			classType &m_Class;
			returnType (classType::*m_Function)(void);
		};

		template<typename returnType, typename classType, typename param1>
		class OneParamClassFunctor : public IDebugFunctor
		{
		public:
			OneParamClassFunctor(returnType (classType::*funcPtr)(param1), classType &classInst) : m_Function(funcPtr), m_Class(classInst)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				if(arguments.size() < 2)
					g_Console->LogError("Insufficient number of arguments",Channels::All);
				else
				{	
					param1 ParamterOne;
					StringUtilities::ConvertString(arguments[1],&ParamterOne);
					(m_Class.*m_Function)(ParamterOne);
				}
			}

		private:
			//returnType m_ReturnValue;
			classType &m_Class;
			returnType (classType::*m_Function)(param1);
		};

		template<typename returnType, typename classType, typename param1, typename param2>
		class TwoParamClassFunctor : public IDebugFunctor
		{
		public:
			TwoParamClassFunctor(returnType (classType::*funcPtr)(param1, param2), classType &classInst) : m_Function(funcPtr), m_Class(classInst)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				if(arguments.size() < 3)
					g_Console->LogError("Insufficient number of arguments",Channels::All);
				else
				{	
					param1 ParamterOne;
					StringUtilities::ConvertString(arguments[1],&ParamterOne);
					param2 ParameterTwo;
					StringUtilities::ConvertString(arguments[2],&ParameterTwo);
					(m_Class.*m_Function)(ParamterOne,ParameterTwo);
				}
			}

		private:
			//returnType m_ReturnValue;
			classType &m_Class;
			returnType (classType::*m_Function)(param1,param2);
		};

		template<typename returnType, typename classType, typename param1, typename param2, typename param3>
		class ThreeParamClassFunctor : public IDebugFunctor
		{
		public:
			ThreeParamClassFunctor(returnType (classType::*funcPtr)(param1,param2,param3), classType &classInst) : m_Function(funcPtr), m_Class(classInst)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				if(arguments.size() < 4)
					g_Console->LogError("Insufficient number of arguments",Channels::All);
				else
				{	
					param1 ParamterOne;
					StringUtilities::ConvertString(arguments[1],&ParamterOne);
					param2 ParameterTwo;
					StringUtilities::ConvertString(arguments[2],&ParameterTwo);
					param3 ParameterThree;
					StringUtilities::ConvertString(arguments[3],&ParameterThree);
					(m_Class.*m_Function)(ParamterOne,ParameterTwo,ParameterThree);
				}
			}

		private:
			//returnType m_ReturnValue;
			classType &m_Class;
			returnType (classType::*m_Function)(param1,param2,param3);
		};

		template<typename returnType, typename classType, typename param1, typename param2, typename param3, typename param4>
		class FourParamClassFunctor : public IDebugFunctor
		{
		public:
			FourParamClassFunctor(returnType (classType::*funcPtr)(param1,param2,param3,param4), classType &classInst) : m_Function(funcPtr), m_Class(classInst)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				if(arguments.size() < 5)
					g_Console->LogError("Insufficient number of arguments",Channels::All);
				else
				{	
					param1 ParamterOne;
					StringUtilities::ConvertString(arguments[1],&ParamterOne);
					param2 ParameterTwo;
					StringUtilities::ConvertString(arguments[2],&ParameterTwo);
					param3 ParameterThree;
					StringUtilities::ConvertString(arguments[3],&ParameterThree);
					param4 ParameterFour;
					StringUtilities::ConvertString(arguments[4],&ParameterFour);
					(m_Class.*m_Function)(ParamterOne,ParameterTwo,ParameterThree,ParameterFour);
				}
			}

		private:
			//returnType m_ReturnValue;
			classType &m_Class;
			returnType (classType::*m_Function)(param1,param2,param3,param4);
		};

		template<typename returnType, typename classType, typename param1, typename param2, typename param3, typename param4, typename param5>
		class FiveParamClassFunctor : public IDebugFunctor
		{
		public:
			FiveParamClassFunctor(returnType (classType::*funcPtr)(param1, param2, param3, param4, param5), classType &classInst) : m_Function(funcPtr), m_Class(classInst)
			{

			}

			virtual void operator()(std::vector<std::string> arguments)
			{
				if(arguments.size() < 6)
					g_Console->LogError("Insufficient number of arguments",Channels::All);
				else
				{	
					param1 ParamterOne;
					StringUtilities::ConvertString(arguments[1],&ParamterOne);
					param2 ParameterTwo;
					StringUtilities::ConvertString(arguments[2],&ParameterTwo);
					param3 ParameterThree;
					StringUtilities::ConvertString(arguments[3],&ParameterThree);
					param4 ParameterFour;
					StringUtilities::ConvertString(arguments[4],&ParameterFour);
					param5 ParameterFive;
					StringUtilities::ConvertString(arguments[5],&ParameterFive);
					(m_Class.*m_Function)(ParamterOne, ParameterTwo,ParameterThree,ParameterFour,ParameterFive);
				}
			}

		private:
			//returnType m_ReturnValue;
			classType &m_Class;
			returnType (classType::*m_Function)(param1,param2,param3,param4,param5);
		};

		//Functor methods
		void RegisterFunctor(std::string name, IDebugFunctor *functor);

		void RegisterFunction(std::string name, void (*funcPtr)(std::vector<std::string>));

		template<typename returnType>
		void RegisterFunction(std::string name, returnType (*funcPtr)())
		{
			RegisterFunctor(name, new ZeroParameterFunctor<returnType>(funcPtr));
		}

		template<typename returnType, typename paramOne>
		void RegisterFunction(std::string name, returnType (*funcPtr)(paramOne))
		{
			RegisterFunctor(name,new OneParameterFunctor<returnType,paramOne>(funcPtr));
		}

		template<typename returnType, typename paramOne, typename paramTwo>
		void RegisterFunction(std::string name, returnType (*funcPtr)(paramOne,paramTwo))
		{
			RegisterFunctor(name, new TwoParameterFunctor<returnType,paramOne, paramTwo>(funcPtr));
		}

		template<typename returnType, typename paramOne, typename paramTwo, typename paramThree>
		void RegisterFunction(std::string name, returnType (*funcPtr)(paramOne,paramTwo,paramThree))
		{
			RegisterFunctor(name, new ThreeParameterFunctor<returnType,paramOne, paramTwo, paramThree>(funcPtr));
		}
		template<typename returnType, typename paramOne, typename paramTwo, typename paramThree, typename paramFour>
		void RegisterFunction(std::string name, returnType (*funcPtr)(paramOne,paramTwo,paramThree,paramFour))
		{
			RegisterFunctor(name, new FourParameterFunctor<returnType,paramOne, paramTwo, paramThree, paramFour>(funcPtr));
		}

		template<typename returnType, typename paramOne, typename paramTwo, typename paramThree, typename paramFour, typename paramFive>
		void RegisterFunction(std::string name, returnType (*funcPtr)(paramOne,paramTwo,paramThree,paramFour))
		{
			RegisterFunctor(name, new FiveParameterFunctor<returnType,paramOne,paramTwo,paramThree,paramFour,paramFive>(funcPtr));
		}

		template<typename returnType, typename classType>
		void RegisterFunction(std::string name, returnType (classType::*funcPtr)(), classType &classInst)
		{
			RegisterFunctor(name, new ZeroParamClassFunctor<returnType,classType>(funcPtr,classInst));
		}

		template<typename returnType, typename classType, typename paramOne>
		void RegisterFunction(std::string name, returnType (classType::*funcPtr)(paramOne), classType &classInst)
		{
			RegisterFunctor(name, new OneParamClassFunctor<returnType,classType,paramOne>(funcPtr,classInst));
		}

		template<typename returnType, typename classType, typename paramOne, typename paramTwo>
		void RegisterFunction(std::string name, returnType (classType::*funcPtr)(paramOne,paramTwo), classType &classInst)
		{
			RegisterFunctor(name, new TwoParamClassFunctor<returnType,classType,paramOne,paramTwo>(funcPtr,classInst));
		}

		template<typename returnType, typename classType, typename paramOne, typename paramTwo, typename paramThree>
		void RegisterFunction(std::string name, returnType (classType::*funcPtr)(paramOne,paramTwo,paramThree), classType &classInst)
		{
			RegisterFunctor(name, new ThreeParamClassFunctor<returnType,classType,paramOne,paramTwo,paramThree>(funcPtr,classInst));
		}

		template<typename returnType, typename classType, typename paramOne, typename paramTwo, typename paramThree, typename paramFour>
		void RegisterFunction(std::string name, returnType (classType::*funcPtr)(paramOne,paramTwo,paramThree,paramFour), classType &classInst)
		{
			RegisterFunctor(name, new FourParamClassFunctor<returnType,classType,paramOne,paramTwo,paramThree,paramFour>(funcPtr,classInst));
		}	
		
		template<typename returnType, typename classType, typename paramOne, typename paramTwo, typename paramThree, typename paramFour, typename paramFive>
		void RegisterFunction(std::string name, returnType (classType::*funcPtr)(paramOne,paramTwo,paramThree,paramFour,paramFive), classType &classInst)
		{
			RegisterFunctor(name, new FiveParamClassFunctor<returnType,classType,paramOne,paramTwo,paramThree,paramFour,paramFive>(funcPtr,classInst));
		}

	private:

		//Writer output
		IConsoleWriter * m_Writer;//Current means to write to a console
		unsigned int m_Channel;//The current channel that we will be writing to
		unsigned int m_ValidChannels;//Channels that are created and usable
		std::string m_ChannelIDS[Channels::MaxChannels];//We can only have a max of 32 channels
		float LOG_2;

		std::string GetChannelName(unsigned int channel);

		//Reader
		IConsoleReader * m_Reader;//Current means to get input from the console

		//functors	
		std::map<std::string, IDebugFunctor *> m_Functors;

		void ListAllCommands();
		void Clear();

		unsigned int m_CurrentChannel;
		//void OutputToFile(std::string file)

		bool m_Debug;
	};
}
