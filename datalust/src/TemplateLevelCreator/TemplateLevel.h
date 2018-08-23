/*************************************************************************/
/*!
\file   TemplateLevel.h
\author Studio200
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
#include <sstream>
#include <vector>
#include <map>
#include <glm/detail/type_vec2.hpp>
#include "GameObject/GameObjectManager.h"
/**
 * \brief 
 */
const std::string LEVELLOADERFILETYPE(".txt");
/**
 * \brief 
 */
const std::string DIR("data\\json\\");
/**
 * \brief 
 */
const std::string ARCHETYPEFILETYPE(".json");
/**
 * \brief 
 */
class TemplateLevel
{
public:
  TemplateLevel(std::string  file);
  TemplateLevel(std::string  file, std::string newdir);

  ~TemplateLevel();


  void SetPos(float x, float y);
  void SetScale(float x, float y);
  
  glm::vec2 GetScale() const { return scale; };
  glm::vec2 GetPos() const { return grid; };
  void constructlevel(std::vector<GameObject>& man);
 //void construct_ui(std::vector<GameObject>& man); old not needed keeping for code count
  void LevelIO(std::string file, std::string _dir);
 
  private:
  //helpers
  void getmap(std::stringstream& ss);
  bool stringtobool(const std::string& cs);
  void getkey(std::stringstream& ss);
  std::string dir;
  std::string filename;
  std::string ext;
  std::vector<std::string> map;
  std::map<char, std::pair<std::string, bool> > Tile2Archetype;
  glm::vec2 TileSize;
  bool isOpen;
  glm::vec2 grid;
  glm::vec2 scale;
};
  //glm::vec2 FindGridLocation(unsigned i, unsigned j);