/*************************************************************************/
/*!
\file   MapTemplate.h
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
#include <sstream>
#include <vector>
#include <map>
#include "TemplateLevelCreator/TemplateLevel.h"
#include <Json/json.h>

/**
 * \brief 
 */
class MapTemplate
{
  std::vector<std::string> levelmap;
  std::map<char, std::string> RoomNames;
  bool isOpen = false;
  std::string filename;
  std::string folderdir;
  glm::vec2 pos;
  glm::vec2 scale;
  std::vector<GameObject>& man_;
public:
  void getmap(Json::Value& obj);
  void getkey(Json::Value& obj);
  void SetPos(glm::vec2 pos);
  void SetScale(glm::vec2 _scale);
  //void constructmap();
  void buildArray(std::vector<std::vector<GameObject>>& levelmap);
  MapTemplate(std::string  _file, std::vector<GameObject>& man);
  ~MapTemplate();
};

