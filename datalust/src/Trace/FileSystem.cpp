/*************************************************************************/
/*!
\file   FileSystem.cpp
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
#if  defined(_WIN32) || defined(WIN32)
constexpr bool WINDOWS = true;
constexpr char SEPARATOR = '\\';

#include <ShlObj.h>
#include <direct.h> 
#include <FileAPI.h>
#include <Shlwapi.h>

#else 

constexpr bool WINDOWS = false;
constexpr char SEPARATOR = '/';

// in a perfect world, I'd support Linux
#error "Unsupported OS"
#endif

#include <algorithm>
#include <iterator>
#include <memory>
#include <stdlib.h>
#include <sstream>
#include "FileSystem.hpp"


/**
 * \brief 
 */
static const std::pair<bool, std::string> FAILURE = std::make_pair(false, std::string{});

#if defined(_WIN32) || defined (WIN32)
static constexpr int PATHS[static_cast<int>(Path::COUNT)] = {
  CSIDL_APPDATA,         // AppData\Roaming
  CSIDL_PERSONAL,        // My Documents
  CSIDL_COMMON_DOCUMENTS // Documents
};
#endif

/**
 * \brief 
 */
FileSystem::FileSystem() //:
  //dir_{ getWorkingDirectory().second } // unused
{
}

/**
 * \brief 
 * \param commonPath 
 * \param subDir 
 * \return 
 */
std::pair<bool, std::string> FileSystem::getSubDir(Path commonPath, std::string const & subDir)
{
  auto && result = getCommonPath(commonPath);
  if (!result.first) return FAILURE;
  
  const std::string s_dir = ConvertSeparator(subDir);

  std::string path;
  path.reserve(result.second.length() + s_dir.length());

  path += std::move(result.second) + SEPARATOR;

  std::stringstream ss;
  ss << std::move(s_dir);
  std::string token;

  // validate path
  while (std::getline(ss, token, SEPARATOR))
  {
    path.append(std::move(token));

    // if the path doesn't exist and we can't create it...
    if (!DirectoryExists(path) && !createDirectory(path))
      return FAILURE;

    path += SEPARATOR;
  }

  return std::make_pair(true, std::move(path));
}

/**
 * \brief 
 * \param path 
 * \return 
 */
std::pair<bool, std::string> FileSystem::getCommonPath(Path path)
{
  if (path <= Path::INVALID || path >= Path::COUNT)
    return FAILURE;

  TCHAR szPath[MAX_PATH];

  if (SUCCEEDED(SHGetFolderPath(nullptr,
    PATHS[static_cast<int>(path)] | CSIDL_FLAG_CREATE,
    nullptr,
    0,
    szPath)))
  {
    return std::make_pair(true, std::string{ szPath });
  }

  return FAILURE;

}

/**
 * \brief 
 * \return 
 */
std::pair<bool, std::string> FileSystem::getWorkingDirectory(void)
{
  char *buffer = _getcwd(nullptr, 0);

  if (!buffer) return FAILURE;

  const std::string path{ buffer };

  free(buffer);

  return std::make_pair(true, std::move(path));
}

/**
 * \brief 
 * \param path 
 * \return 
 */
bool FileSystem::DirectoryExists(std::string const & path)
{
  // https://stackoverflow.com/a/8233867/7087793

  const DWORD ftyp = GetFileAttributes(path.c_str());

  //               is a valid filepath                 is a directory
  return ((ftyp != INVALID_FILE_ATTRIBUTES) && (ftyp & FILE_ATTRIBUTE_DIRECTORY));
}


/**
 * \brief 
 * \param path 
 * \return 
 */
bool FileSystem::createDirectory(std::string const & path)
{
         // creation succeeds                       // folder already existed, anyway
  return CreateDirectory(path.c_str(), nullptr) ||  GetLastError() == ERROR_ALREADY_EXISTS;
}

/**
 * \brief 
 * \param path 
 * \return 
 */
bool FileSystem::gotoPath(std::string const & path)
{
  return false;
}

/**
 * \brief 
 * \param path 
 * \return 
 */
std::string FileSystem::ConvertSeparator(std::string const & path)
{
  static constexpr char otherSeparator = (WINDOWS) ? '/' : '\\';

  std::string p;
  p.reserve(path.length());

  for (auto && c : path)
    p.push_back((c != otherSeparator) ? c : SEPARATOR);

  return std::move(p);
}
