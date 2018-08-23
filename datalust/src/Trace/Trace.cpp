/**
  \file   Trace.cpp
  \author Taylor Osmond

  \brief Implementation of trace message system

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Trace/FileSystem.hpp"
#include "Trace.hpp"

/**
 * \brief 
 */
namespace Trace {
  static constexpr char * tags[] =
  {
    "[INFO]:      ",
    "[WARNING]:   ",
    "[ERROR]:     "
  };

  /**
	 * \brief 
	 * \return 
	 */
	bool TraceHandler::Init()
	{
		//open trace logfile
    auto && result = FileSystem{}.getSubDir(Path::MyDocuments, "DigiPen\\ColorBlaster");
    if (!result.first) return false;

		logfile.open(std::move(result.second) + "tracelog.txt", std::ios::trunc);

		//validate opening
    const bool isOpen = static_cast<bool>(logfile);

    if (isOpen)
      TraceMessage(TraceLevel::INFO, "Trace: Init");

    return isOpen;
	}

  /**
	 * \brief 
	 */
	void TraceHandler::Shutdown()
	{
		//validate and close the file
		if (logfile)
			logfile.close();
	}

  /**
	 * \brief 
	 * \param level 
	 * \param format 
	 * \param ... 
	 */
	void TraceHandler::TraceMessage(TraceLevel level, const char * format, ...)
	{
		//if not in debug mode...
        #ifndef _DEBUG

		//do not output INFO messages
		if (level == INFO)
			return;

		#endif

		va_list args;

		//set tag based on trace level
    const char * tag = tags[level];

		va_start(args, format);

		if (logfile)
		{
			//get necessary buffer size
			int buffer_size = vsnprintf(NULL, 0, format, args) + 1;

			//create buffer
			char *buffer = new char[buffer_size];
      if (!buffer) return;
			//fill buffer
			vsnprintf(buffer, buffer_size, format, args);
			
			//output message to file
			logfile << tag << buffer << std::endl;

      delete[] buffer;
		}

		va_end(args);
	}
}