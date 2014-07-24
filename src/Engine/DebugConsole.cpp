/******************************************************************************/
/*!
All content � 2012 DigiPen (USA) Corporation, all rights reserved
\file   DebugConsole.cpp
\author Luke Powell
\par    Course: GAM300
*/
/******************************************************************************/

#include "DebugConsole.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include "StringUtilities.h"
#include <fstream>
#include <errno.h>
#include <assert.h>
#include <cmatH>

namespace DebugLog
{
	Console *g_Console = nullptr;

	/*!***********************************
	*\brief Constructor for the console.
	*\param writer  The Writer interface to use with the Console, this writer is an
	*               abstraction on the actual output of the Console. This output 
	*               can be anything from a normal console to an in-game console
	*************************************/
	Console::Console(IConsoleWriter * writer, IConsoleReader * reader) : m_Writer(writer), m_Channel(Channels::All), m_ValidChannels(0), m_Reader(reader)
	{
		assert(writer && "You must pass a writer to the debug console, use Console::EmptyWriter if you want no behavior");
		assert(reader && "You must pass a reader to the debug console, use Console::EmptyReader if you want no behavior");

		m_Reader->m_Parent = this;

		m_ChannelIDS[0] = "None";
		m_ChannelIDS[31] = "All";
		LOG_2 = std::log(2.0f);
		g_Console = this;
		m_Debug = true;
		//Register some functions
		RegisterFunction("help",&DebugLog::Console::ListAllCommands,*this);
		RegisterFunction("clear",&DebugLog::Console::Clear,*this);
		RegisterFunction("cls",&DebugLog::Console::Clear,*this);
		RegisterFunction("channels",&DebugLog::Console::EnumerateChannels,*this);
		RegisterFunction("SetChannel",(void(Console::*)(const std::string))(&DebugLog::Console::SetChannel),*this);
		RegisterFunction("debug",&DebugLog::Console::SetDebug,*this);
		RegisterFunction("run",&DebugLog::Console::Run,*this);
		RegisterFunction("echo",&DebugLog::Console::Echo,*this);
		m_CurrentChannel = 1;
	}

	/*!********************************************
	*\brief This simply clears our reference from 
	*       the writers shared_ptr.
	**********************************************/
	Console::~Console()
	{
	}

	/*!***********************************
	*\brief Logs a error to the current writer
	*\param message  The error message to output
	*\param channel  The channel to output the error message on 
	*************************************/
	void Console::LogError(const std::string &message, unsigned int channel)
	{
		if(channel & m_Channel)
		{
			std::stringstream Stream;
			Stream << GetChannelName(channel) << " " << "Error: " << message << std::endl;
			m_Writer->Write(Stream.str(),E_Error);
		}
	}

	/*!***********************************
	*\brief Logs a warning to the current writer
	*\param message The warning message to output
	*\param channel The channel to output the warning message on
	*************************************/
	void Console::LogWarning(const std::string &message, unsigned int channel)
	{
		if(channel & m_Channel)
		{
			std::stringstream Stream;
			Stream <<GetChannelName(channel) << " " << "Warning: " << message << std::endl;
			m_Writer->Write(Stream.str(),E_Warning);
		}
	}

	/*!***********************************
	*\brief Logs information to the current writer
	*\param message The information to output to the current writer
	*\param channel The channel to output the information on
	*************************************/
	void Console::LogInfo(const std::string &message, unsigned int channel)
	{
		if(channel & m_Channel)
		{
			std::stringstream Stream;
			Stream << GetChannelName(channel) << " " << "Info: " << message << std::endl;
			m_Writer->Write(Stream.str(),E_Info);
		}
	}

	/*!***********************************
	*\brief Logs a debug message to the current writer
	*\param message The debug message to output to the current writer
	*\param channel The channel to to output the debug message 
	*************************************/
	void Console::LogDebug(const std::string &message, unsigned int channel)
	{
		if(!m_Debug)
			return;

		if(channel & m_Channel)
		{
			std::stringstream Stream;
			Stream << GetChannelName(channel) << " " << "Debug: " << message << std::endl;
			m_Writer->Write(Stream.str(),E_Debug);
		}
	}


	/*!***********************************
	*\brief Allows the creation of a custom message to output on the debug channel
	*       the other log 
	messages assume a tag of warning, error, debug, or info will
	*       be added as needed.  This method allows the use of a custom tag or no tag at all.
	*\param tag  The tag to be displayed in lieu of warning, info, error, or debug
	*\param message  The message to be displayed
	*\param channel  The channel to display the message on
	*************************************/
	void Console::LogCustom(const std::string &tag, const std::string &message, unsigned int channel)
	{
		if(channel & m_Channel)
		{
			std::stringstream Stream;
			Stream <<  tag << message << std::endl;
			m_Writer->Write(Stream.str(),E_Custom);
		}
	}

	
	/*!***********************************
	*\brief Return the current writer associated with the Console. This may be null
	*       This returns a shared_ptr. 
	*\return A shared_ptr to the current Writer associated with the Console. 
	*************************************/
	const IConsoleWriter * Console::GetWriter() const
	{
		return m_Writer;
	}

	/*!***********************************
	*\brief This will change what Writer is associated with the console.  If you wish to restore 
	*       the old writer use GetWriter() to get the current value before calling this function.
	*       A writer is the abstraction on the rendering and output of text. 
	*\param newWriter  A valid shared_ptr to a new writer calss
	*************************************/
	void Console::SetWriter(IConsoleWriter * newWriter)
	{
		assert(newWriter);
		m_Writer = newWriter;
	}

	/*!***********************************
	*\brief This will create a new debug channel and associate an ID and Name.
	*       ID must be a value between 1 and 30.  The value cannot overlap 
	*       a previous call to this function.  This channel can then be used
	*       to restrict what will be displayed in the DebugOutput console
	*\param name  Name to associate with the channel
	*\param id    ID to use for the channel some value between 1 and 30 that has not been used before
	*\return      The bitmask for the channel which can be passed to SetChannel(unsigned int), and 
	*             value sent to SetChannel that is not a return from this function will raise
	*             a warning and not change the state of the program.
	*************************************/
	unsigned int Console::CreateChannel(const std::string &name, unsigned int id)
	{
		id = m_CurrentChannel;
		assert(id < 31 && id > 0);//0 and 31 are taken, any value 32 or greater is invalid.
		assert(m_ChannelIDS[id].empty());//Cannot have overlapping channels
		m_ChannelIDS[id] = name;
		m_ValidChannels |= (1 << id);
		++m_CurrentChannel;
		return 1 << id;
	}

	/*!***********************************
	*\brief Will output a custom message to the current writer if one exits detailing
	*       what channels are available and what ID and name the are using
	*************************************/
	void Console::EnumerateChannels()
	{
		std::stringstream Str;//Using this to format our string

		Str << std::endl; //We are going to enclose this in a starting an ending extra newline
		for(unsigned int i = 0; i < Channels::MaxChannels; ++i)
		{
			if(!m_ChannelIDS[i].empty())
				Str << i << "\t" << m_ChannelIDS[i] << std::endl;//Output the ID(number of the channel) and the Name of the channel
		}
		Str << std::endl;

		LogCustom("\nChannels:\nID\tName",Str.str(),Channels::All);
	}

	
		
	/*!***********************************
	*\brief Sets the current output channel of the Console this will restrict the output
	*       to only show that of the chosen channel.
	*\param channel The of the channel to use either one of the predefined special 
	*               channels or the return value of CreateChannel
	*************************************/
	void Console::SetChannel(unsigned int channel)
	{
		//Handles 0 case
		if(channel == 0 || (!(channel & (channel - 1)) && (channel & m_ValidChannels)))
			m_Channel = channel;
		else//The user tried to set an invalid channel, warn them of this and do nothing
			LogWarning("Attempt to set debug to an invalid channel no changes were made!",Channels::All);
	}

	/*!***********************************
	*\brief An alternative SetChannel that will use a channels name rather then the channel's ID
	*       this should only be used where the integral ID is not know as this requires a linear
	*       search through all known channels making strcmps until a match is found at most 32 
	*       iterations will be made.
	*\param channel  The name of the channel that we want to set to be the current channel
	*************************************/
	void Console::SetChannel(const std::string channel)
	{
		//Loop through our channels in a linear fashion there are only 32 so this serach should not be too
		//costly however as such SetChannel(unsigned int) should be preferred
		for(int i = 0; i < 32; ++i)
		{
			//If we found the channel that we are looking for
			if(m_ChannelIDS[i] == channel)
			{
				//Set the bitmask for the found channel
				m_Channel = 1 << i;//Shift the bit to the proper place
				return;//Break out early nothing left to do here
			}

		}

		//If this point is reached the for loop completed and the proper channel was not found
		//The user selected a channel that was not found and as such is invalid
		LogWarning("Attempt to set debug to an invalid channel no changes were made!",Channels::All);
	}

	/*!***********************************
	*\brief Uses maths to get back an string ID from a channel
	*       bit mask
	*\return The string that corresponds to the ID from the channel
	*************************************/
	std::string Console::GetChannelName(unsigned int channel)
	{
		int foo = static_cast<int>(std::log(static_cast<float>(channel)) / LOG_2);
		if(foo < 32)
			return m_ChannelIDS[foo];
		else if(foo == 32)
			return "All";
		else
			return "Invalid channel";
	}


	/*!***********************************
	*\brief Wrapper around a protected virtual method.  Console should be 
	*       the only class calling this method.
	*\param message  The message to pass to the virtual method to actually 
	*************************************/
	void IConsoleWriter::Write( const std::string &message, MessageTypes type)
	{
		VWrite(message,type);
	}

	/*!********************************************
	*\brief This will return a a shared_ptr to the
	*       current reader interface
	*\return const shared_ptr to a reader
	**********************************************/
	const IConsoleReader * Console::GetReader() const
	{
		return m_Reader;
	}
	

	/*!********************************************
	*\brief If a non-null reader is given the source
	*       of console input will switch to that reader
	*\param   reader
	*         The reader to switch console input to
	**********************************************/
	void Console::SetReader(IConsoleReader * reader)
	{
		if(reader)
		{
			m_Reader = reader;
			m_Reader->m_Parent = this;
		}
		else
			LogError("Attempt to set a null reader.reset  No changes where made",Channels::All);
	}

	/*!********************************************
	*\class This will take input and parse it into 
	*       it's constituent parts and will pass
	*       it down to the matching functor.
	**********************************************/
	void Console::ParseInput( std::string &input )
	{
		std::vector<std::string> StringOutput;

		StringUtilities::VectorizeString(StringOutput,input);

		if(!StringOutput.empty())//At least on element
		{
			auto Functor = m_Functors.find(StringOutput[0]);
			if(Functor != m_Functors.end())
				(*Functor->second)(StringOutput);
			else
				LogError("Invalid Command!",DebugLog::Channels::All);
		}
	}

	void Console::RegisterFunctor(std::string name, IDebugFunctor * functor)
	{
		if(functor)
		{
			m_Functors[name] = functor;
		}
	}

	void Console::RegisterFunction(std::string name,  void (*funcPtr)(std::vector<std::string>) )
	{
		RegisterFunctor(name, new FFunctor(funcPtr));
	}

	void Console::ListAllCommands()
	{
		LogCustom("","Commands:",Channels::All);
		for(auto It = m_Functors.begin(); It != m_Functors.end(); ++It)
		{
			LogCustom("  ",It->first,Channels::All);
		}
	}

	void Console::Clear()
	{
		m_Writer->Clear();
	}

	void Console::Run( std::string shellScript )
	{
		std::vector<std::string> Lines;
		//# are comments
		std::ifstream File(shellScript.c_str(),std::ifstream::in);
		while(!File.eof())
		{
			std::string Temp;
			std::getline(File,Temp);
			size_t Comment = Temp.find_first_of('#');

			if(Comment != std::string::npos)
				Temp.erase(Comment,Temp.npos);

			if(!Temp.empty())
				Lines.push_back(Temp);
			if(File.eof())
			{
				break;
			}
		}
		//Parse the input we got
		for(auto It = Lines.begin(); It != Lines.end(); ++It)
			ParseInput(*It);
	}

	void Console::Echo( std::string output )
	{
		LogCustom("",output,Channels::All);
	}

	/*!********************************************
	*\brief Will return EmptyReader and nothing
	*       else.
	**********************************************/
	std::string EmptyReader::GetLine()
	{
		return "EmptyReader";
	}


	IConsoleReader::~IConsoleReader()
	{
	}

	FileWriter::FileWriter(std::string file)
	{
		m_File = fopen(file.c_str(),"w");
	}

	void FileWriter::VWrite(const std::string &message, MessageTypes type)
	{
		if(m_File)
			fprintf(m_File, "%s\n", message.c_str());
	}
}
