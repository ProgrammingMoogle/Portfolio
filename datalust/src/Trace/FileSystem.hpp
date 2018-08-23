/*************************************************************************/
/*!
\file   FileSystem.h
\author studio 200
\par
\par    DigiPen login:
\par    Course: GAME 200
\par   Copyright � 2017 DigiPen (USA) Corp. and its owners. All Rights Reserved.
\date   9/28/2017
\brief
Functions include:
*/
/*************************************************************************/
#pragma once
#include <string>

#if  defined(_WIN32) || defined(WIN32)

/**
 * \brief 
 */
enum class Path : int
{
  // for bookkeeping
  INVALID = -1,

  AppData = 0,
  MyDocuments,
  Documents,

  // For bookkeeping
  COUNT
};

#else

#error "Unsupported OS"
enum class Path
{
  INVALID = -1,
  
  Home = 0,
  
  COUNT
};

#endif

/**
 * \brief 
 */
class FileSystem
{
public:
  FileSystem();

  std::pair<bool, std::string> getSubDir(Path commonPath, std::string const& subDir);

private:
  static std::pair<bool, std::string> getCommonPath(Path path);
  static std::pair<bool, std::string> getWorkingDirectory(void);
  static bool DirectoryExists(std::string const& path);
  static std::string ConvertSeparator(std::string const& path);
  bool createDirectory(std::string const& path);

  bool gotoPath(std::string const& path);





private:
  //std::string dir_;
};