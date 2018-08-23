/**
  \file   Util.hpp
  \author Zayd Gaudet

  \brief 

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _UTIL_HPP_
#define _UTIL_HPP_
#include <cstddef>
#include <utility>
#define UNREFERENCED_PARAMETER(P) (P)

// a pair meant to store the index and generation data in the index vector,
// pair.first = the object table index pair.second = the generation Data
/**
 * \brief 
 */
typedef std::pair<size_t, unsigned int> ObjectId;
#define nullobj std::pair<size_t, unsigned int>((size_t)-1,(unsigned int)-1) 

#endif