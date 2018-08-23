/*************************************************************************/
/*!
\file   MapTemplate.cpp
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
#include "MapTemplate.h"
#include <fstream>
#include "Engine/Engine.h"
#include "Transform/Transform.hpp"
#include "Graphics/Camera.hpp"
#include <boost/optional/optional.hpp>


/**
 * \brief 
 */
const std::string MPLEVELLOADERFILETYPE(".txt");
/**
 * \brief 
 */
const std::string MPDIR("data\\json\\EricLevels\\");
/**
 * \brief 
 */
const std::string MPARCHETYPEFILETYPE(".json");

/**
 * \brief 
 * \param obj 
 */
void MapTemplate::getmap(Json::Value& obj)
{
  unsigned size = (obj["LAYOUT"].size());

  for (unsigned i = 0; i < size; ++i)
  {
    levelmap.push_back(obj["LAYOUT"][i].asString());
  }
}

/**
 * \brief 
 * \param obj 
 */
void MapTemplate::getkey(Json::Value& obj)
{
  char size = static_cast<char>(obj["Key"].size());

  for (char i = 0; i < size; ++i)
  {
    RoomNames.emplace(i + '0' + 1, obj["Key"][i].asString());
  }
}

/**
 * \brief 
 * \param _pos 
 */
void MapTemplate::SetPos(glm::vec2 _pos)
{
  pos.x = _pos.x;
  pos.y = _pos.y;
}

/**
 * \brief 
 * \param _scale 
 */
void MapTemplate::SetScale(glm::vec2 _scale)
{
  scale.x = _scale.x;
  scale.y = scale.y;
}

#if 0
void MapTemplate::constructmap()
{
  if (!isOpen) return;

  //create map
  for (unsigned i = 0; i < levelmap.size(); ++i)
  {

    unsigned nextsize = levelmap[i].size();
    glm::vec2 temp(pos);

    for (unsigned j = 0; j < nextsize; ++j)
    {
      const auto findresult = RoomNames.find(levelmap[i][j]);

      if (findresult == RoomNames.end())
      {
        pos += scale;
        continue;
      }

      TemplateLevel mapsection(findresult->second, folderdir);
      mapsection.SetPos(pos.x, pos.y);
      mapsection.SetScale(scale.x, scale.y);
      mapsection.constructlevel(man_);
      pos = mapsection.GetPos();
    }

    //pos = temp;
  }
}
#endif

/**
 * \brief 
 * \param levelmap 
 */
void MapTemplate::buildArray(std::vector< std::vector<GameObject> >& levelmap)
{
  if (!isOpen) return;

  unsigned size = RoomNames.size();

  //create map
  for (unsigned i = 0; i < RoomNames.size(); ++i)
  {
    constexpr char holder = '0';
    const auto findresult = RoomNames.find(holder + i + 1);

    if (findresult == RoomNames.end())
    {
      continue;
    }
    
    TemplateLevel mapsection(findresult->second, folderdir); //create new map
    //mapsection.SetPos(pos.x, pos.y); // set pos
    //mapsection.SetScale(scale.x, scale.y);//set scale
    std::vector<GameObject> loader; //create temp vec
    mapsection.constructlevel(loader); //construct level
    levelmap.push_back(std::move(loader));
    //levelmap[i] = loader;//move vector into list
    //pos = mapsection.GetPos();
  }
}

/**
 * \brief 
 * \param file 
 * \param _man 
 */
MapTemplate::MapTemplate(std::string  file, std::vector<GameObject>& _man) : filename(MPDIR + file + "\\" + file + MPARCHETYPEFILETYPE), folderdir(MPDIR + file + "\\Rooms\\"), pos(glm::vec2(0.0f, 0.0f)), scale(glm::vec2(0.0f, 0.0f)), man_(_man)
{
  Json::Value obj;

  //load in buffer
  ReadBuffer(filename.c_str(), &obj);
  bool Key = obj.isMember("Key");
  bool Map = obj.isMember("LAYOUT");

  if (Key  && Map)
  {
    //get key
    getkey(obj);
    //get map
    getmap(obj);

    isOpen = true;
  }
  else
  {
    isOpen = false; //did not load file!
  }
}


/**
 * \brief 
 */
MapTemplate::~MapTemplate()
{
}
