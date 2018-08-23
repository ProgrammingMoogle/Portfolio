#pragma once
/******************************************************************************/
/*!
\file    ObjectFactory.h
\author  Zayd Gaudet
\par     zayd.gaudet\@digipen.edu
\par     10/15/2017

\brief
Interface of the ObjectFactory class

All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H
#include <vector>
#include <string>
#include <Json/json.h>
#include <json/json-forwards.h>
#include <glm/vec2.hpp>
#include "GameObject.h"
#include "GameObjectManager.h"


struct BehaviorParameters;
class ObjectFactory
{

  //static std::list<std::pair<int, std::vector<GameObject> > > levelobjectlist;
public:

  static void Add_Behaviors(const std::vector<std::reference_wrapper<GameObject>>& vector, const GameObjectManager& manager, const std::vector<std::vector<ObjectId>> pairses);
  /*************************************************************************/
  /*!
  Takes the path to a jason file containing the number of objects needed,
  the types of objects, and the values to instantiate them with it then
  creates all of these objects and adds them to the manager

  \param filename
  the path to the json file

  \return
  returns true if the objects were created succesfully else returns
  false
  */
  /*************************************************************************/
  static std::vector<std::reference_wrapper<GameObject>> Create(const char* filename, GameObjectManager& manager);


  static void AddBehaviors(const std::vector<std::reference_wrapper<GameObject>>& vector, GameObjectManager& manager);

  /*************************************************************************/
  /*!
  Serialize all of the objects in the GameObjectManager

  \return
  returns true if the objects were serialized succesfully els returns
  false
  */
  /*************************************************************************/

  static bool Serialize(GameObject const& gameObject, const char* filename);
  
  static GameObject ArchetypeGameObject(const char*filename, glm::vec2 pos);
  static GameObject DeSerializeArchetype(Json::Value::const_iterator::reference value);
private:
  static void DeSerializeWorldText(Json::Value::const_iterator::reference obj, GameObject& object);
  static glm::vec2& DeSerializeVec2(Json::Value obj, glm::vec2 & vec);
  static void DeSerializeTextSprite(Json::ValueConstIterator::reference value, GameObject& object);
  static void DeSerializeBehaviorsAG(Json::Value::const_iterator::reference value, const GameObject& game_object);
  static void DeSerializeParticle(Json::Value::const_iterator::reference value, GameObject& game_object);
  static void DeSerializeCircleSprite(Json::Value::const_iterator::reference value, GameObject& game_object);
  static void ArchetypeUISprite(GameObject& game_object, Json::Value::const_iterator::reference value);
  static GameObject Archetype(Json::Value Archetype);
  static void DeSerializeCollider(Json::ValueConstIterator::reference value, GameObject& object);
  static void DeSerializeUISprite(Json::ValueConstIterator::reference value, GameObject& object);
  static void DeSerializeParameters(Json::Value obj, BehaviorParameters &);
  static void DeSerializeTileMap(Json::Value::const_iterator::reference value, std::vector<std::vector<GameObject> >& manager);
  static void DeSerializeHealth(Json::Value::const_iterator::reference value, GameObject& game_object);
  // function to write the FileName, Alpha, BlendAlpha, Colors, BlendMode, and Depth to a json file 
  template <typename T>
  static void WriteFN_A_BA_C_BM_D(GameObject const & gameObject, Json::Value Root, unsigned int NumObj, const char* name);

  // function to write the Size and the Frame to a json file
  template <typename T>
  static void WriteS_F(GameObject const& gameObject, Json::Value Root, unsigned int NumObj, const char* name);

  // function to read non-default values into a graphics component
  template <typename T>
  static void ReadGraphicsComponents(GameObject& object, Json::Value obj, const char* name);

};

/*************************************************************************/
/*!
This class will load all of the levels from a jason file
(this will be used to automate the construction and transition between
gamestates)
*/
/*************************************************************************/
class LevelSerializer
{
public:
  // dctor only needs to construct a vector the size is determined by 
  // the Manifest File
  LevelSerializer();
  // copying isn't allowed
  LevelSerializer(LevelSerializer const & o) = delete;
  // moving isn't allowed
  LevelSerializer(LevelSerializer&& o) = delete;
  // there are no non-default constructors

  // destructor
  ~LevelSerializer() = default;
  std::string CurrentLevel();
  std::string SetNextLevel(const int level);
  std::string SetNextLevel();
  void clear();
private:
  std::vector<std::string> Levels_;
  std::vector<std::string>::iterator currentLevel_;
};

// private helper function to pasrse a json object
void ReadBuffer(const char * filename, Json::Value * obj);

#include "ObjectFactory_timpl.cpp"



#endif // OBJECTFACTORY_H