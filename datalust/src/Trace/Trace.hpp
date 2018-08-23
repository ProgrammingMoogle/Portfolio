/**
  \file   Trace.hpp
  \author Taylor Osmond

  \brief Interface for trace message system.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _TRACE_HPP_
#define _TRACE_HPP_

#include <Engine/System.h>
#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <Engine/Engine.h>

/// Trace Message Level
typedef enum TraceLevel {
  INFO,    ///< Debug only
  WARNING, ///< Debug/Release
  ERROR,   ///< Debug/Release
}TraceLevel;

/// Trace namespace
namespace Trace {
  /// Trace message handler
  class TraceHandler : public System {
  public:
    bool Init();           ///< Init function
    void Shutdown();       ///< Shutdown function

    /// Trace message function
    void TraceMessage(TraceLevel level, const char * format, ...);

    friend class LogStream; ///< LogStream friend class
  private:
    std::ofstream logfile; ///< Logfile
  };

  /// Log stream class
  class LogStream : public std::ostream {
  public:
    /// insertion operator overload
    template <typename T>
    std::ostream& operator<<(T rhs);
  };

  template<typename T>
  inline std::ostream & LogStream::operator<<(T rhs)
  {
    //call rhs insertion operator on logfile
    engine.Find<TraceHandler>()->logfile << rhs;
  }
}

#endif