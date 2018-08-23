#pragma once
/******************************************************************************/
/*!
\file    ObjectFactory.cpp
\author  Zayd Gaudet
\par     zayd.gaudet\@digipen.edu
\par     10/15/2017

\brief
Implementation of json parsing

All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <fstream>
#include <exception>
#include <vector>
//#include <Behaviors/Behaviors.hpp>
#include <Behaviors/EnemyBullet.hpp>
#include <Audio/MusicComponent.hpp>
#include "Behaviors/FadeOut.hpp"
#include "Behaviors/ParticleFadeout.hpp"
#include "Behaviors/DelayedDestructionBehavior.hpp"
#include <Behaviors/CameraBehavior.hpp>
#include <Behaviors/MenuBehaviors.hpp>
#include <Behaviors/PlatformBehavior.hpp>
#include <Behaviors/Player1Behavior.hpp>
#include <Behaviors/Player2Behavior.hpp>
#include <Behaviors/PunchBehaviors.hpp>
#include <Behaviors/BulletBehavior.h>
#include <Behaviors/HUDBehavior.hpp>
#include <Behaviors/EnemyChaseBehavior.h>
#include <Cheats/CheatOptions.hpp>
#include <Engine/Engine.h>
#include <Graphics/Sprite.hpp>
#include <Menus/MenuComponent.h>
#include <Physics/Colliders.hpp>
#include "Physics\ParticleSystem.hpp"
#include <Physics/ForceEmitter.hpp>
#include <Physics/ForceGenerator.hpp>
#include <Physics/ForceRegistry.hpp>
#include <Physics/ParticleEmitter.hpp>
#include <Graphics/ParticleAnimator.hpp>
#include <Physics/RigidBody.hpp>
#include <Transform/Transform.hpp>   
#include "Graphics\Camera.hpp"
#include "ObjectFactory.h"
#include "TemplateLevelCreator/TemplateLevel.h"
#include "HealthComponent.h"
#include "Behaviors/EnemyPaceBehavior.h"
#include "CoinBehavior.h"
#include "Graphics/TextSprite.hpp"
#include "TemplateLevelCreator/MapTemplate.h"
#include "GoalBehavior.h"
#include "Behaviors/HoleBehavior.hpp"
#include "RoomSwitchBehavior.h"
#include "RSwitchComponent.h"
#include "HealthPickupComponent.h"
#include "Messaging/Messaging.hpp"
#include "EnemyDoorBehavior.h"
#include "DoorTextComponent.h"


enum behaviors
{
  camera,
  player1,
  unused,
  platform,
  menu,
  punch,
  HUDCOIN,
  enemychase,
  enemypace,
  coinlocate,
  HUDLIFE,
  ChangeLevel,
  Hole,
  RoomSwitch,
  Cheat,
  EnemyCountDoor,
  EnemyBul
};

namespace Factory
{
  
  struct HUD
  {
    HUDTYPE type;
  };

  struct EnemyShoot
  {
    bool fires;
    bool shootsPlayer;
    glm::vec2 shootDir;
    float fireRate;
    float bulletSpeed;
  };

  struct EnemyPace
  {
    EnemyShoot shoot;
    bool reverses;
    float movespeed;
    std::vector<glm::vec2> offset;
  };

  struct EnemyChase
  {
    EnemyShoot shoot;
    float movespeed;
  };

  struct RoomSwitch
  {
    glm::vec2 pos;
    unsigned _room;
  };

  struct Player
  {
    bool canShoot;
  };

  struct ChangeLevel
  {
    int level;
  };
}

struct BehaviorParameters
{
  BehaviorParameters() = delete;

  BehaviorParameters(BehaviorParameters const& other) :
    id{ other.id }, name{ other.name }, param{}
  {
    switch (id)
    {
    case behaviors::enemypace: {
      new (&param.pace) Factory::EnemyPace{other.param.pace};
    }break;

    default: {
      std::memcpy(&param, &other.param, sizeof(union param));
    }break;
    }
  }

  BehaviorParameters(BehaviorParameters && other) : 
    id{ std::move(other.id) }, name{ std::move(other.name) }, param{}
  {
    switch (other.id)
    {
    case behaviors::enemypace: {
      new (&param.pace) Factory::EnemyPace{ std::move(other.param.pace) };
    }break;

    default: {
      std::memcpy(&param, &other.param, sizeof(union param));
    }
    }
  }

  BehaviorParameters(std::string && _name, behaviors const& _id)
    : name{ std::move(_name) }, id{ _id }, param{} {
    switch (id)
    {
    case behaviors::enemypace: {
      new (&param.pace) Factory::EnemyPace{};
    }break;

    default: {
    }break;
    }
  }

  BehaviorParameters(std::string const& _name, behaviors const& _id)
    : name{ _name }, id{ _id }, param{} {
    switch (id)
    {
    case behaviors::enemypace: {
      new (&param.pace) Factory::EnemyPace{};
    }break;

    default: {
    }break;
    }
  }

  ~BehaviorParameters()
  {
    switch (id)
    {
    case behaviors::enemypace: {
      param.pace.offset.~vector();
    }break;

    default: {
    }break;
    }
  }

  std::string name;
  behaviors id;

  union param 
  {
  public:
    param() {}
    ~param() {}

    struct Factory::EnemyPace pace;
    struct Factory::EnemyChase chase;
    struct Factory::HUD hud;
    struct Factory::RoomSwitch roomswitch;
    struct Factory::Player player;
    struct Factory::ChangeLevel levelswitch;
  }param;
};

std::vector < BehaviorParameters > DelayedBehavior;
void ObjectFactory::AddBehaviors(const std::vector<std::reference_wrapper<GameObject>>& output, GameObjectManager& manager)
{

  auto object = output.begin();
  auto end = output.end();


  std::for_each(DelayedBehavior.begin(), DelayedBehavior.end(), [&](BehaviorParameters & behavior)
  {

    //consturct behaviors now that they have an object id

    // find the next matching object in our array
    object = std::find_if(object, end, [&](std::reference_wrapper<GameObject> const& obj)
    {
      return obj.get().GetName() == behavior.name;
    });

    // if we could not find, just return (this should really throw)
    if (object == end)
      return;

    // objects are reference wrapped
    GameObject & obj = object->get();

    switch (behavior.id)
    {
    case camera:
      obj.Add(new CameraBehavior(obj));
      break;
    case player1: {
    Factory::Player& player = behavior.param.player;
      obj.Add(new Player1Behavior(obj, player.canShoot));
    }break;
    case unused:
      break;
    case platform:
      obj.Add(new PlatformBehavior(obj));
      break;
    case menu:
      obj.Add(new MenuBehavior(obj));
      break;
    case punch: {
      obj.Add(new BulletBehavior(obj, 1, 1, 1));
      break;
    }
    case HUDCOIN: {
      obj.Add(new HUDBehavior(obj, static_cast<HUDTYPE>(0)));
      break; }
    case HUDLIFE: {
      obj.Add(new HUDBehavior(obj, static_cast<HUDTYPE>(1)));
      break; }

    case enemychase: {
      Factory::EnemyChase & chase = behavior.param.chase;
      Factory::EnemyShoot & shoot = chase.shoot;
      obj.Add(new EnemyChaseBehavior{ obj,
        chase.movespeed, shoot.fires, shoot.shootsPlayer, shoot.shootDir, shoot.fireRate, shoot.bulletSpeed });
    }break;

    case enemypace: {
      Factory::EnemyPace & pace = behavior.param.pace;
      Factory::EnemyShoot & shoot = pace.shoot;
      obj.Add(new EnemyPaceBehavior(obj,
        shoot.fires, shoot.shootsPlayer, shoot.shootDir, shoot.fireRate, shoot.bulletSpeed,
        std::move(pace.reverses), std::move(pace.movespeed), std::move(pace.offset))
      );
    }break;

    case coinlocate:
      obj.Add(new CoinBehavior(obj));
      break;
    case ChangeLevel: {
      Factory::ChangeLevel & level = behavior.param.levelswitch;
      obj.Add(new GoalBehavior(obj, level.level));
    }break;
    case Hole:
      obj.Add(new HoleBehavior(obj));
      break;
    case Cheat:
      obj.Add(new CheatOptions(obj));
      break;
    case EnemyBul:
      obj.Add(new EnemyBullet{ obj });
    }

    // move on to the next object
    ++object;
  });

  DelayedBehavior.clear();

  // update the manager so everything will be rendered where it should be
  //manager.Update(0); // <-- suspect

}

void ObjectFactory::Add_Behaviors(const std::vector<std::reference_wrapper<GameObject>>& output,
  const GameObjectManager& manager, const std::vector<std::vector<ObjectId> >  levelobjects)
{
  auto object = output.begin();
  auto end = output.end();


  std::for_each(DelayedBehavior.begin(), DelayedBehavior.end(), [&](BehaviorParameters & behavior)
  {

    //consturct behaviors now that they have an object id

    // find the next matching object in our array
    object = std::find_if(object, end, [&](std::reference_wrapper<GameObject> const& obj)
    {
      return obj.get().GetName() == behavior.name;
    });

    // if we could not find, just return (this should really throw)
    if (object == end)
      return;

    // objects are reference wrapped
    GameObject & obj = object->get();

    switch (behavior.id)
    {
    case camera:
      obj.Add(new CameraBehavior(obj));
      break;
    case player1: {
      Factory::Player& player = behavior.param.player;
      obj.Add(new Player1Behavior(obj, player.canShoot));
    }break;
    case unused:
      break;
    case platform:
      obj.Add(new PlatformBehavior(obj));
      break;
    case menu:
      obj.Add(new MenuBehavior(obj));
      break;
    case punch: {
      obj.Add(new BulletBehavior(obj, 1, 1, 1));
      break;
    }
    case HUDCOIN: {
      obj.Add(new HUDBehavior(obj, static_cast<HUDTYPE>(0)));
      break; }
    case HUDLIFE: {
      obj.Add(new HUDBehavior(obj, static_cast<HUDTYPE>(1)));
      break; }
    case enemychase: {
      Factory::EnemyChase & chase = behavior.param.chase;
      Factory::EnemyShoot & shoot = chase.shoot;
      obj.Add(new EnemyChaseBehavior{ obj, chase.movespeed, shoot.fires, shoot.shootsPlayer, shoot.shootDir, shoot.fireRate, shoot.bulletSpeed });
    }break;

    case enemypace: {
      Factory::EnemyPace & pace = behavior.param.pace;
      Factory::EnemyShoot & shoot = pace.shoot;
      obj.Add(new EnemyPaceBehavior(obj,
        shoot.fires, shoot.shootsPlayer, shoot.shootDir, shoot.fireRate, shoot.bulletSpeed,
        std::move(pace.reverses), std::move(pace.movespeed), std::move(pace.offset))
      );
    }break;

    case coinlocate: {
      obj.Add(new CoinBehavior(obj));
    }break;

    case ChangeLevel: {
      Factory::ChangeLevel & level = behavior.param.levelswitch;
      obj.Add(new GoalBehavior(obj, level.level));
    }break;
    case Hole:
      obj.Add(new HoleBehavior(obj));
      break;
    case RoomSwitch: {
      Factory::RoomSwitch & roomswitch = behavior.param.roomswitch;
      obj.Add(new RoomSwitchBehavior(obj, roomswitch.pos, roomswitch._room, std::move(levelobjects)));
    }break;
     case EnemyCountDoor:
       obj.Add(new EnemyDoorBehavior(obj) );
     break;
    }

    // move on to the next object
    ++object;
  });

  DelayedBehavior.clear();

  // update the manager so everything will be rendered where it should be
  //manager.Update(0); //
}

std::vector<std::reference_wrapper<GameObject>> ObjectFactory::Create(const char* filename, GameObjectManager& manager)
{
  // create a jason value
  Json::Value obj;

  // Parse the filename into the Json Value
  ReadBuffer(filename, &obj);

  // create a vector if GameObject* and of size NumObj
  static std::vector<GameObject> AssemblyLine;
  static std::vector<std::vector<GameObject> > Tiles;
  std::vector< std::vector< ObjectId > > LEVELOBJECTS;
  static std::vector<std::reference_wrapper<GameObject>> output;
  // loop through the vector and construct objects based on archetypes replacing
  // values that aren't default (this will be read in from the file)
  unsigned size = obj["Object"].size();

  bool tilemapexists = false;

  for (unsigned int i = 0; i < size; ++i)
  {
    if (obj["Object"][i].isMember("Archetype"))
    {
      AssemblyLine.emplace_back(DeSerializeArchetype(obj["Object"][i]));
    }
    else {
      tilemapexists = true;
      GameObject bebe("SwitchListener","SwitchListener");
      BehaviorParameters param{ bebe.GetName(), behaviors::RoomSwitch };
      param.param.roomswitch._room = 0;
      param.param.roomswitch.pos = glm::vec2{};
      DelayedBehavior.emplace_back(std::move(param));
      AssemblyLine.emplace_back(std::move(bebe));
      DeSerializeTileMap(obj["Object"][i], Tiles);
  }
  }

  // loop through the GameObject Vector and add the GameObjects to the GameObjectManager


  for (auto && i : AssemblyLine) // move level data
    output.emplace_back(manager.Add(std::move(i)));
  
  if (tilemapexists) {
    for (unsigned i = 0; i < Tiles.size(); ++i)
    {
      std::vector<ObjectId> levelkeys;

      for (unsigned j = 0; j < Tiles[i].size(); ++j)
      {
        GameObject&  game_object = manager.Add(std::move(Tiles[i][j])); // add to manager
        levelkeys.push_back(game_object.GetKey());//get key
        output.emplace_back(game_object); // get behavior
      }

      LEVELOBJECTS.push_back(levelkeys);
    }

    
    
  }


   if (tilemapexists == false)
     AddBehaviors(output, manager);
   else
     Add_Behaviors(output, manager, LEVELOBJECTS);

  LEVELOBJECTS.clear();
  AssemblyLine.clear();
  Tiles.clear();

  return std::move(output);
}

GameObject ObjectFactory::Archetype(Json::Value Archetype)
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // File IO STUFF
  // create a jason reader to parse thring from the file
  // Open the json file so we can serialize it
  // create a jason value
  Json::Value obj;
  ReadBuffer(Archetype.asCString(), &obj);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // construct a new object with the specified components
  GameObject NewObj(obj["Type"].asCString());
  // if the Object has a movement value set its value to that in the json file, else set it to zero
  obj.isMember("MovVal") ? NewObj.MoveVal(obj["MovVal"].asFloat()) : NewObj.MoveVal(0);
  if (NewObj.MoveVal())
    NewObj.PInput(true);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if the archetype has a BackGround give the object a BackGround
  if (obj.isMember("BackgroundImage"))
  {
    Graphics::BackgroundImage* bg = new Graphics::BackgroundImage(
      // Set the Parent of the object
      NewObj,
      // Set the img file for the sprite to load from
      obj["BackgroundImage"]["FileName"].asCString(),
      // set the depth of the sprite
      obj["BackgroundImage"]["Depth"].asFloat(),
      // set the Parallax factor of the backgroud
      obj["BackgroundImage"]["ParallaxFactor"].asFloat(),
      // set the alpha value of the sprite
      obj["BackgroundImage"]["Alpha"].asFloat(),
      // set the collor of the sprite
      // Set the Red value of the Color
      obj["BackgroundImage"]["Red"].asFloat(),
      // Set the Red value of the Color
      obj["BackgroundImage"]["Green"].asFloat(),
      // Set the Red value of the Color
      obj["BackgroundImage"]["Blue"].asFloat(),
      obj["BackgroundImage"]["BlendAlpha"].asFloat(),
      // Set the Blend mode of the Sprite
      static_cast<Graphics::BlendMode>(obj["BackgroundImage"]["BlendMode"].asInt()));
    bg->setRepeat(static_cast<Graphics::RepeatMode>(obj["BackgroundImage"]["HorizontalRepeatMode"].asInt()),
      static_cast<Graphics::RepeatMode>(obj["BackgroundImage"]["VerticalRepeatMode"].asInt()));
    NewObj.Add(bg);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // TODO INTEGRATE BEHAVIORS ASAP
  //if (obj.isMember("Behaviors"))
  //{
  //	
  //}
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  ///////////////////////////////
  //DelayedDestruction
  ///////////////////////////////
  if (obj.isMember("DelayedDestruction"))
  {
    auto && inner = obj["DelayedDestruction"];
    NewObj.Add(new DelayedDestruction{ NewObj, inner.get("Lifetime", 1.0f).asFloat() });
  }

  ///////////////////////////////
  //FadeOut
  ///////////////////////////////
  if (obj.isMember("FadeOut"))
  {
    auto && inner = obj["FadeOut"];
    NewObj.Add(new FadeOut{ NewObj, inner.get("Interval", 1.0f).asFloat() });
  }


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if the archetype has a Camera give the object a Camera
  if (obj.isMember("Camera"))
  {

    Graphics::Camera* component = new Graphics::Camera(NewObj);

    auto && cam = obj["Camera"];

    if (cam["isActive"].asBool())
      component->activate();

    component->setHeight(cam["Height"].asFloat());

    {
      auto && vec = cam["Center"];

      component->setCenter(glm::vec2(vec["X"].asFloat(), vec["Y"].asFloat()));
    }

    {
      auto && vec = cam["Up"];

      component->setUp(glm::vec2(vec["X"].asFloat(), vec["Y"].asFloat()));
    }

    NewObj.Add(component);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //TODO CHECK IF ANYTHING IS MISSINNG HERE
  // if the archetype has a RigidBody give the object a RigidBody (THE RIGIDBODY MUST BE ADDED BEFORE THE COLLIDER OR
  //																 PHYSICS WONT WORK CORECTLY)
  if (obj.isMember("RigidBody"))
  {
    NewObj.Add(new KPE::RigidBody(NewObj));
    //if (obj["RigidBody"]["Force"].asBool())
   // {
      //determine type of force
      //engine.GetPhysicsEngine()->forceRegistry_.add(NewObj.Find<KPE::RigidBody>(), new KPE::ForceGravity);
    //}

    if (obj["RigidBody"].isMember("Mass"))
      NewObj.Find<KPE::RigidBody>()->setMass(obj["RigidBody"]["Mass"].asFloat());

    if (obj["RigidBody"].isMember("Force"))
    {
      if (obj["RigidBody"]["Force"].asBool())
      {
        NewObj.Find<KPE::RigidBody>()->forcesActivate();
      }
      else
      {
        NewObj.Find<KPE::RigidBody>()->forcesDeactivate();
      }
    }
    //NewObj.Find<KPE::RigidBody>()->setMass(0);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //TODO CHECK IF ANYTHING IS MISSINNG HERE
  // if the archetype has a RigidBody give the object a RigidBody (THE RIGIDBODY MUST BE ADDED BEFORE THE COLLIDER OR
  //																 PHYSICS WONT WORK CORECTLY)
  if (obj.isMember("Force"))
  {
    for (unsigned i = 0; i < obj["Force"].size(); ++i) {
      std::string type;
      glm::vec2 vec1(0);
      glm::vec2 vec2(0);
      float val1 = 0;
      float val2 = 0;

      type = obj["Force"][i]["Type"].asString();
      vec1.x = obj["Force"][i]["Vec1X"].asFloat();
      vec1.y = obj["Force"][i]["Vec1Y"].asFloat();
      vec2.x = obj["Force"][i]["Vec2X"].asFloat();
      vec2.y = obj["Force"][i]["Vec2Y"].asFloat();

      val1 = obj["Force"][i]["Val1"].asFloat();
      val2 = obj["Force"][i]["Val2"].asFloat();

      engine.GetPhysicsEngine()->forceRegistry_.add(NewObj.Find<KPE::RigidBody>(),
        KPE::makeForceGenerator(type, vec1, vec2, val1, val2));
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //TODO CHECK IF ANYTHING IS MISSINNG HERE
  // if the archetype has a Collider give the object a Collider
  if (obj.isMember("Colliders"))
  {

    Transform * transform = NewObj.Find<Transform>();

    for (auto && colObj : obj["Colliders"])
    {
      KPE::Colliders* collider = new KPE::Colliders(NewObj);
      collider->add(
        colObj.isMember("BoundType") ? static_cast<KPE::BoundType>(colObj["BoundType"].asInt()) : static_cast<KPE::BoundType>(0),
        transform,
        colObj["xHalfSize"].asFloat(), colObj["yHalfSize"].asFloat(),

        // float getFloat(const Json::Value& obj, const char *key); 
        // conditionally returns obj[key].asFloat() or 0.0f if the member exists

        colObj.isMember("xOffset") ? colObj["xOffset"].asFloat() : 0.0f,
        colObj.isMember("yOffset") ? colObj["yOffset"].asFloat() : 0.0f
      );

      NewObj.Add(collider);
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //TODO PARSE A Circle FORCE EMITTER
  // if the archetype has a Circle ForceEmitter give the object a Circle ForceEmitter
  if (obj.isMember("ForceEmitterCircle"))
  {
    KPE::ForceEmitterCircle* force = new KPE::ForceEmitterCircle(NewObj);
    std::string type;
    glm::vec2 vec1(0);
    glm::vec2 vec2(0);
    float val1 = 0;
    float val2 = 0;

    type = obj["ForceEmitterCircle"]["Force"]["Type"].asString();
    vec1.x = obj["ForceEmitterCircle"]["Force"]["Vec1X"].asFloat();
    vec1.y = obj["ForceEmitterCircle"]["Force"]["Vec1Y"].asFloat();
    vec2.x = obj["ForceEmitterCircle"]["Force"]["Vec2X"].asFloat();
    vec2.y = obj["ForceEmitterCircle"]["Force"]["Vec2Y"].asFloat();

    val1 = obj["ForceEmitterCircle"]["Force"]["Val1"].asFloat();
    val2 = obj["ForceEmitterCircle"]["Force"]["Val2"].asFloat();

    force->setForce(KPE::stringToType(type),
      KPE::makeForceGenerator(type, vec1, vec2, val1, val2));
    force->setRadius(obj["ForceEmitterCircle"]["Radius"].asFloat());
    NewObj.Add(force);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //TODO PARSE A Box FORCE EMITTER
  // if the archetype has a Box ForceEmitter give the object a Box ForceEmitter
  if (obj.isMember("ForceEmitterBox"))
  {
    KPE::ForceEmitterBox* force = new KPE::ForceEmitterBox(NewObj);
    std::string type;
    glm::vec2 vec1(0);
    glm::vec2 vec2(0);
    float val1 = 0;
    float val2 = 0;

    type = obj["ForceEmitterBox"]["Force"]["Type"].asString();
    vec1.x = obj["ForceEmitterBox"]["Force"]["Vec1X"].asFloat();
    vec1.y = obj["ForceEmitterBox"]["Force"]["Vec1Y"].asFloat();
    vec2.x = obj["ForceEmitterBox"]["Force"]["Vec2X"].asFloat();
    vec2.y = obj["ForceEmitterBox"]["Force"]["Vec2Y"].asFloat();

    val1 = obj["ForceEmitterBox"]["Force"]["Val1"].asFloat();
    val2 = obj["ForceEmitterBox"]["Force"]["Val2"].asFloat();

    force->setForce(KPE::stringToType(type),
      KPE::makeForceGenerator(type, vec1, vec2, val1, val2));
    force->setHalfsize(glm::vec2(obj["ForceEmitterBox"]["xHalfSize"].asFloat(),
      obj["ForceEmitterBox"]["yHalfSize"].asFloat()));
    NewObj.Add(force);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if the archetype has a MenuComponent give the object a MenuComponent
  if (obj.isMember("MenuComponent"))
  {
    std::vector<std::pair<Graphics::UISprite, int>> vec;
    if (obj.isMember("UISprite"))
    {
      for (unsigned i = 0; i < obj["MenuComponent"]["UISprite"].size(); ++i)
      {

        vec.push_back(std::make_pair(Graphics::UISprite(
          // Set the Parent of the object
          NewObj,
          // Set the img file for the sprite to load from
          obj["MenuComponent"]["UISprite"][i]["FileName"].asCString(),
          // set the size of the sprite
          Graphics::Size{ obj["MenuComponent"]["UISprite"][i]["SpriteWidth"].asFloat(),
          obj["MenuComponent"]["UISprite"][i]["SpriteHeight"].asFloat() },
          // set the depth of the sprite
          obj["MenuComponent"]["UISprite"][i]["Depth"].asFloat(),
          // tell the sprite which frame in the sprite sheet it is using
          obj["MenuComponent"]["UISprite"][i]["Frame"].asUInt(),
          // set the alpha value of the sprite
          obj["MenuComponent"]["UISprite"][i]["Alpha"].asFloat(),
          // set the collor of the sprite
          Graphics::Color{
            // Set the Red value of the Color
            obj["MenuComponent"]["UISprite"][i]["Red"].asFloat(),
            // Set the Green value of the Color
            obj["MenuComponent"]["UISprite"][i]["Green"].asFloat(),
            // Set the Blue value of the Color
            obj["MenuComponent"]["UISprite"][i]["Blue"].asFloat(),
            // Set the Blend Alpha of the Color
            obj["MenuComponent"]["UISprite"][i]["BlendAlpha"].asFloat() },
            // Set the Blend mode of the Sprite
            static_cast<Graphics::BlendMode>(obj["MenuComponent"]["UISprite"][i]["BlendMode"].asInt())),
            obj["MenuComponent"]["UISprite  "][i]["level"].asInt()));
        vec.back().first.setOffset(obj["MenuComponent"]["UISprite"][i]["xOffset"].asFloat(), obj["MenuComponent"]["UISprite"][i]["yOffset"].asFloat());

      }
    }
    NewObj.Add(new MenuComponent(NewObj, std::move(vec)));
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (obj.isMember("MusicController"))
  {
    MusicController* music = new MusicController(NewObj);
    for (unsigned i = 0; i < obj["MusicController"]["Music"].size(); ++i)
    {
      music->AddMusic(obj["MusicController"]["Music"][i]["Name"].asCString(),
        obj["MusicController"]["Music"][i]["Delay"].asUInt(),
        obj["MusicController"]["Music"][i]["Probability"].asFloat());
    }
    NewObj.Add(music);
  }

  DeSerializeParticle(obj, NewObj);
  // if the archetype has a sprite give the object a sprite
  if (obj.isMember("Sprite"))
    NewObj.Add(new Graphics::Sprite(
      // Set the Parent of the object
      NewObj,
      // Set the img file for the sprite to load from
      obj["Sprite"]["FileName"].asCString(),
      // set the size of the sprite
      Graphics::Size{ obj["Sprite"]["SpriteWidth"].asFloat(), obj["Sprite"]["SpriteHeight"].asFloat() },
      // set the depth of the sprite
      obj["Sprite"]["Depth"].asFloat(),
      // tell the sprite which frame in the sprite sheet it is using
      obj["Sprite"]["Frame"].asUInt(),
      // set the alpha value of the sprite
      obj["Sprite"]["Alpha"].asFloat(),
      // set the collor of the sprite
      Graphics::Color{
        // Set the Red value of the Color
        obj["Sprite"]["Red"].asFloat(),
        // Set the Green value of the Color
        obj["Sprite"]["Green"].asFloat(),
        // Set the Blue value of the Color
        obj["Sprite"]["Blue"].asFloat(),
        // Set the Blend Alpha of the Color
        obj["Sprite"]["BlendAlpha"].asFloat() },
        // Set the Blend mode of the Sprite
        static_cast<Graphics::BlendMode>(obj["Sprite"]["BlendMode"].asInt())));
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if the archetype has a CircleSprite give the object a CircleSprite

  DeSerializeCircleSprite(obj, NewObj);


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if the archetype has a UI give the object a UIs
  if (obj.isMember("UISprite"))
  {
    ArchetypeUISprite(NewObj, obj);
    
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (obj.isMember("TextSprite"))
  {
    auto textObj = obj["TextSprite"];
    const char * filename = textObj["FileName"].asCString();
    const Graphics::Size size{ textObj["SpriteWidth"].asFloat(), textObj["SpriteHeight"].asFloat() };
    const float depth = textObj["Depth"].asFloat();
    const size_t frame = textObj["Frame"].asUInt();
    const float alpha = textObj["Alpha"].asFloat();
    const Graphics::Color color{
      // Set the Red value of the Color
      textObj["Red"].asFloat(),
      // Set the Green value of the Color
      textObj["Green"].asFloat(),
      // Set the Blue value of the Color
      textObj["Blue"].asFloat(),
      // Set the Blend Alpha of the Color
      textObj["BlendAlpha"].asFloat() 
    };
    const Graphics::BlendMode mode = static_cast<Graphics::BlendMode>(textObj["BlendMode"].asInt());

    if (textObj.get("inWorldSpace", false).asBool())
    {
      NewObj.Add(
        new Graphics::WorldTextSprite(
          NewObj, filename, size, depth,
          frame, alpha, color,  mode
      ));
    }
    else
    {
      NewObj.Add(
        new Graphics::TextSprite(
          NewObj, filename, size, depth,
          frame, alpha, color, mode
      ));
    }

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if the archetype has a UI give the object a UIs
  if (obj.isMember("UICircleSprite"))
    NewObj.Add(new Graphics::UICircleSprite(
      // Set the Parent of the object
      NewObj,
      // Set the img file for the sprite to load from
      obj["UICircleSprite"]["FileName"].asCString(),
      // set the size of the sprite
      Graphics::Size{ obj["UICircleSprite"]["SpriteWidth"].asFloat(), obj["UICircleSprite"]["SpriteHeight"].asFloat() },
      // set the depth of the sprite
      obj["UICircleSprite"]["Depth"].asFloat(),
      // tell the sprite which frame in the sprite sheet it is using
      obj["UICircleSprite"]["Frame"].asUInt(),
      // set the alpha value of the sprite
      obj["UICircleSprite"]["Alpha"].asFloat(),
      // set the collor of the sprite
      Graphics::Color{
        // Set the Red value of the Color
        obj["UICircleSprite"]["Red"].asFloat(),
        // Set the Green value of the Color
        obj["UICircleSprite"]["Green"].asFloat(),
        // Set the Blue value of the Color
        obj["UICircleSprite"]["Blue"].asFloat(),
        // Set the Blend Alpha of the Color
        obj["UICircleSprite"]["BlendAlpha"].asFloat() },
        // Set the Blend mode of the Sprite
        static_cast<Graphics::BlendMode>(obj["UICircleSprite"]["BlendMode"].asInt())));
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // set the transform things
  NewObj.Find<Transform>()->setOrientation(obj["Transform"]["Orientation"].asFloat());
  NewObj.Find<Transform>()->setPosition(obj["Transform"]["xPosition"].asFloat(), obj["Transform"]["yPosition"].asFloat());
  NewObj.Find<Transform>()->setXScale(obj["Transform"]["xScale"].asFloat());
  NewObj.Find<Transform>()->setYScale(obj["Transform"]["yScale"].asFloat());
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  return NewObj;
}

GameObject ObjectFactory::DeSerializeArchetype(Json::Value::const_iterator::reference obj)
{
  // create an object from the json folder
  GameObject object = Archetype(obj["Archetype"]);

  // check if the object has screen wrap if so give it a screen wrap

  // check if the object has a lifetime if so give it the lifetime
  obj.isMember("LifeTime") ? object.LifeTime(obj["LifeTime"].asFloat()) : object.LifeTime(0.0f);

  // set the name of the object
  if (obj.isMember("Name"))
    object.SetName(obj["Name"].asCString());

  // set the movment value of the object (if it has one)
  if (obj.isMember("MovVal"))
    object.MoveVal(obj["MovVal"].asFloat());


  // Read in non default values for a BackgroundImage if the object has any
  if (obj.isMember("BackgroundImage"))
  {
    Graphics::BackgroundImage* bg = object.Find<Graphics::BackgroundImage>();
    ReadGraphicsComponents<Graphics::BackgroundImage>(object, obj, "BackgroundImage");
    if (obj["BackgroundImage"].isMember("ParallaxFactor"))
      bg->setParallaxFactor(obj["BackgroundImage"]["ParallaxFactor"].asFloat());
    if (obj["BackgroundImage"].isMember("HorizontalRepeatMode") && obj["BackgroundImage"].isMember("VerticalRepeatMode"))
      bg->setRepeat(static_cast<Graphics::RepeatMode>(obj["BackgroundImage"]["HorizontalRepeatMode"].asInt()),
        static_cast<Graphics::RepeatMode>(obj["BackgroundImage"]["VerticalRepeatMode"].asInt()));
  }

  if (obj.isMember("Behaviors"))
  {
    auto && behavior = obj["Behaviors"];
    BehaviorParameters param{ object.GetName(), static_cast<behaviors>(behavior["BehaviorsType"].asInt()) };
    DeSerializeParameters(behavior, param);
    DelayedBehavior.emplace_back(std::move(param));
  }

  // Read in non default values for a Camera if the object has any
  if (obj.isMember("Camera"))
  {
    Graphics::Camera * component = object.Find<Graphics::Camera>();
    auto && cam = obj["Camera"];

    if (cam.get("isActive", false).asBool())
      component->activate();

    if (cam.isMember("Height"))
      component->setHeight(cam["Height"].asFloat());

    if (cam.isMember("Center"))
    {
      auto && vec = cam["Center"];

      glm::vec2 center = component->getCenter();

      if (vec.isMember("X"))
        center.x = vec["X"].asFloat();
      if (vec.isMember("Y"))
        center.y = vec["Y"].asFloat();

      component->setCenter(center);
    }

    if (cam.isMember("Up"))
    {
      auto && vec = cam["Up"];

      glm::vec2 up = component->getUp();

      if (vec.isMember("X"))
        up.x = vec["X"].asFloat();
      if (vec.isMember("Y"))
        up.y = vec["Y"].asFloat();

      component->setUp(up);
    }

  }

  // check if the object has a collider with non-default value
  DeSerializeCollider(obj, object);

  // check if the box force emitter has a non-default half size
  if (obj.isMember("ForceEmitterBox")) {
    std::string type;
    glm::vec2 vec1(0, 0);
    glm::vec2 vec2; (0, 0);
    float val1 = 0;
    float val2 = 0;

    type = obj["ForceEmitterBox"]["Force"]["Type"].asString();
    vec1.x = obj["ForceEmitterBox"]["Force"]["Vec1X"].asFloat();
    vec1.y = obj["ForceEmitterBox"]["Force"]["Vec1Y"].asFloat();
    vec2.x = obj["ForceEmitterBox"]["Force"]["Vec2X"].asFloat();
    vec2.y = obj["ForceEmitterBox"]["Force"]["Vec2Y"].asFloat();

    val1 = obj["ForceEmitterBox"]["Force"]["Val1"].asFloat();
    val2 = obj["ForceEmitterBox"]["Force"]["Val2"].asFloat();

    object.Find<KPE::ForceEmitterBox>()->setForce(KPE::stringToType(type),
      KPE::makeForceGenerator(type, vec1, vec2, val1, val2));

    object.Find<KPE::ForceEmitterBox>()->setHalfsize(
      glm::vec2(obj["ForceEmitterBox"]["xHalfSize"].asFloat(),
        obj["ForceEmitterBox"]["yHalfSize"].asFloat()));
  }

  // check if the Radius of the Circular force emiter has a non-default value
  if (obj.isMember("ForceEmitterCircle"))
    object.Find<KPE::ForceEmitterCircle>()->setRadius(obj["ForceEmitter"]["Radius"].asFloat());

  if (obj.isMember("MenuComponent"))
  {
    MenuComponent* menu = object.Find<MenuComponent>();

    if (obj["MenuComponent"].isMember("UISprite"))
    {
      for (unsigned i = 0; i < obj["MenuComponent"]["UISprite"].size(); ++i)
      {
        Graphics::UISprite sprite(
          // Set the Parent of the object
          object,
          // Set the img file for the sprite to load from
          obj["MenuComponent"]["UISprite"][i]["FileName"].asCString(),
          // set the size of the sprite
          Graphics::Size{ obj["MenuComponent"]["UISprite"][i]["SpriteWidth"].asFloat(),
          obj["MenuComponent"]["UISprite"][i]["SpriteHeight"].asFloat() },
          // set the depth of the sprite
          obj["MenuComponent"]["UISprite"][i]["Depth"].asFloat(),
          // tell the sprite which frame in the sprite sheet it is using
          obj["MenuComponent"]["UISprite"][i]["Frame"].asUInt(),
          // set the alpha value of the sprite
          obj["MenuComponent"]["UISprite"][i]["Alpha"].asFloat(),
          // set the collor of the sprite
          Graphics::Color{
            // Set the Red value of the Color
            obj["MenuComponent"]["UISprite"][i]["Red"].asFloat(),
            // Set the Green value of the Color
            obj["MenuComponent"]["UISprite"][i]["Green"].asFloat(),
            // Set the Blue value of the Color
            obj["MenuComponent"]["UISprite"][i]["Blue"].asFloat(),
            // Set the Blend Alpha of the Color
            obj["MenuComponent"]["UISprite"][i]["BlendAlpha"].asFloat() },
            // Set the Blend mode of the Sprite
            static_cast<Graphics::BlendMode>(obj["MenuComponent"]["UISprite"][i]["BlendMode"].asInt()));
        sprite.setOffset(obj["MenuComponent"]["UISprite"][i]["xOffset"].asFloat(), obj["MenuComponent"]["UISprite"][i]["yOffset"].asFloat());
        menu->Add(std::move(std::make_pair(sprite, obj["MenuComponent"]["UISprite"][i]["level"].asInt())));
      }
    }
  }

  if (obj.isMember("MusicController"))
  {
    MusicController* music = object.Find<MusicController>();
    for (unsigned i = 0; i < obj["MusicController"]["Music"].size(); ++i)
    {
      music->AddMusic(obj["MusicController"]["Music"][i]["Name"].asCString(),
        obj["MusicController"]["Music"][i]["Delay"].asUInt(),
        obj["MusicController"]["Music"][i]["Probability"].asFloat());
    }
    music->PlayMusic();
  }
#if 0
  if (obj.isMember("ParticleEmitter"))
  {

    KPE::ParticleEmitter* emitter = object.Find<KPE::ParticleEmitter>();

    // check if the Particle emitter has a non-default angle if so read it in
    if (obj["ParticleEmitter"].isMember("Angle"))
      emitter->setAngle(obj["ParticleEmitter"]["Angle"].asFloat());

    // check if the ParticleEmitter has a non-default counter if so read it in
    if (obj["ParticleEmitter"].isMember("Angle Variance"))
      emitter->getAngleVariance(obj["ParticleEmitter"]["Angle Variance"].asFloat());

    // 	check if the ParticleEmitter has a non-default Count if so read it in
    if (obj["ParticleEmitter"].isMember("Count"))
      emitter->setMaxCount(obj["ParticleEmitter"]["Count"].asUInt());

    // check if the ParticleEmitter has a non-default PLifetime if so read it in
    if (obj["ParticleEmitter"].isMember("PLifetime"))
      emitter->setPLifetime(obj["ParticleEmitter"]["PLifetime"].asFloat());

    // check if the ParticleEmitter has a non-default Power if so read it in
    if (obj["ParticleEmitter"].isMember("Rate"))
      emitter->setPower(obj["ParticleEmitter"]["Power"].asFloat());

    // check if the ParticleEmitter has a non-default Rate if so read it in
    emitter->setRate(obj["ParticleEmitter"]["Rate"].asFloat());
  }
#endif
  //TODO FIGURE OUT IF WE NEED THIS
  //if (obj.isMember("RigidBody"))
  //{
  //}

  // check if the object has a sprite with non-default values, if so read it in
  if (obj.isMember("Sprite"))
  {
    // read the frame from the json file and store it in the component
    if (obj["Sprite"].isMember("Frame"))
      object.Find<Graphics::Sprite>()->setFrame(obj["Sprite"]["Frame"].asUInt());

    //read the sprite size from the json file and store it in the component
    if (obj["Sprite"].isMember("SpriteWidth") && obj["Sprite"].isMember("SpriteHeight"))
      object.Find<Graphics::Sprite>()->setSize(Graphics::Size{ obj["Sprite"]["SpriteWidth"].asFloat(),
        obj["Sprite"]["SpriteHeight"].asFloat() });

    ReadGraphicsComponents<Graphics::Sprite>(object, obj, "Sprite");
  }

  // check if the object has a CircleSprite with non-default values, if so read it in
  if (obj.isMember("CircleSprite"))
  {
    // read the frame from the json file and store it in the component
    if (obj["CircleSprite"].isMember("Frame"))
      object.Find<Graphics::CircleSprite>()->setFrame(obj["CircleSprite"]["Frame"].asUInt());

    //read the sprite size from the json file and store it in the component
    if (obj["CircleSprite"].isMember("SpriteWidth") && obj["CircleSprite"].isMember("SpriteHeight"))
      object.Find<Graphics::CircleSprite>()->setSize(Graphics::Size{ obj["CircleSprite"]["SpriteWidth"].asFloat(),
        obj["CircleSprite"]["SpriteHeight"].asFloat() });

    ReadGraphicsComponents<Graphics::CircleSprite>(object, obj, "CircleSprite");
  }

  // check if the object has a sprite with non-default values, if so read it in
  if (obj.isMember("UISprite"))
  {
    DeSerializeUISprite(obj, object);
  }


  if (obj.isMember("TextSprite"))
  {
    DeSerializeTextSprite(obj, object);
  }


  // check if the object has a sprite with non-default values, if so read it in
  if (obj.isMember("UICircleSprite"))
  {
    // read the frame from the json file and store it in the component
    if (obj["UICircleSprite"].isMember("Frame"))
      object.Find<Graphics::UICircleSprite>()->setFrame(obj["UISprite"]["Frame"].asUInt());

    //read the sprite size from the json file and store it in the component
    if (obj["UICircleSprite"].isMember("SpriteWidth") && obj["UICircleSprite"].isMember("SpriteHeight"))
      object.Find<Graphics::UICircleSprite>()->setSize(Graphics::Size{ obj["UICircleSprite"]["SpriteWidth"].asFloat(),
        obj["UICircleSprite"]["SpriteHeight"].asFloat() });

    ReadGraphicsComponents<Graphics::UICircleSprite>(object, obj, "UICircleSprite");
  }


  // check whether the transform has any non-default values, if so read them in
  if (obj.isMember("Transform"))
  {
    // check if the transform has a non-default orientation if so read it in
    if (obj["Transform"].isMember("Orientation"))
      object.Find<Transform>()->setOrientation(obj["Transform"]["Orientation"].asFloat());

    // check if the object has a non-default Posistion
    if (obj["Transform"].isMember("xPosition") && obj["Transform"].isMember("yPosition"))
      object.Find<Transform>()->setPosition(obj["Transform"]["xPosition"].asFloat(), obj["Transform"]["yPosition"].asFloat());

    // check if the object has a non-default scaling in the x direction
    if (obj["Transform"].isMember("xScale"))
      object.Find<Transform>()->setXScale(obj["Transform"]["xScale"].asFloat());

    // check if the object has a non-default scaling in the y direction
    if (obj["Transform"].isMember("yScale"))
      object.Find<Transform>()->setYScale(obj["Transform"]["yScale"].asFloat());

  }

  if (obj.isMember("Health"))
  {
    DeSerializeHealth(obj, object);
  }

  return object;
}

void ObjectFactory::DeSerializeWorldText(Json::Value::const_iterator::reference obj, GameObject& object)
{
  object.Add(new Graphics::WorldTextSprite(object));

  object.Add(new DoorTextComponent(object));
}

void ObjectFactory::DeSerializeParameters(Json::Value behavior, BehaviorParameters & param)
{
  switch (param.id)
  {

  case behaviors::player1:
  {
    param.param.player.canShoot = behavior.get("CanShoot", false).asBool();
  }break;

  case behaviors::ChangeLevel: {
    param.param.levelswitch.level = behavior.get("NextLevel", 2).asInt();
  } break;


  case behaviors::enemychase: {
    glm::vec2 dir{ -1, 0 };
    if (behavior.isMember("FiringDirection"))
      DeSerializeVec2(behavior["FiringDirection"], dir);


    param.param.chase = Factory::EnemyChase{
      Factory::EnemyShoot{
        behavior.get("Fires", true).asBool(),
        behavior.get("FiresAtPlayer", false).asBool(),
        dir,
        1.0f / behavior.get("FireRate", 1.5f).asFloat(),
        behavior.get("BulletSpeed", 5).asFloat()
      },
      behavior.get("Speed", 0.0f).asFloat()
    };
  }break;

  case behaviors::enemypace: {

    glm::vec2 dir{ -1, 0 };
    if (behavior.isMember("FiringDirection"))
      DeSerializeVec2(behavior["FiringDirection"], dir);

    std::vector<glm::vec2> offsets;

    if (behavior.isMember("Offset"))
    {
      auto && offset = behavior["Offset"];
      // Offset: [
      //   [2, 0],
      //   [1, 1]
      // ]
      if (offset.isArray())
      {
        offsets.reserve(offsets.size());
        
        for (auto && vec2 : offset)
        {
          glm::vec2 pos{ 0, 0 };
          DeSerializeVec2(vec2, pos);
          offsets.emplace_back(std::move(pos));
        }
      }
      else
      {
        glm::vec2 pos{ 0, 0 };
        DeSerializeVec2(offset, pos);
        offsets.emplace_back(pos);
      }
    }

    param.param.pace = Factory::EnemyPace{
      Factory::EnemyShoot{
        behavior.get("Fires", true).asBool(),
        behavior.get("FiresAtPlayer", false).asBool(),
        dir,
        1.0f / behavior.get("FireRate", 1.5f).asFloat(),
        behavior.get("BulletSpeed", 5).asFloat()
      },
      behavior.get("Reverses", false).asBool(),
      behavior.get("Speed", 0.0f).asFloat(),
      std::move(offsets)
    };

  }break;

  case behaviors::RoomSwitch: {
    glm::vec2 pos{};
    if (behavior.isMember("Position"))
      DeSerializeVec2(behavior["Position"], pos);

    param.param.roomswitch = Factory::RoomSwitch{
      pos,
      behavior.get("Room", 0u).asUInt()
    };
  }break;

  default: {

  }break;
  }
}

void ObjectFactory::DeSerializeTileMap(Json::Value::const_iterator::reference obj, std::vector<std::vector<GameObject> >& manager)
{

  if (obj["TileMap"].isMember("Filename"))
  {

    //std::array<  obj["Key"].size(), std::vector<GameObject> > levelmaplist;
#if 0
    TemplateLevel level(obj["TileMap"]["Filename"].asCString());
    if (obj["TileMap"].isMember("xPos") && obj["TileMap"].isMember("yPos"))
      level.SetPos(obj["TileMap"]["xPos"].asFloat(), obj["TileMap"]["yPos"].asFloat());
    if (obj["TileMap"].isMember("xScale") && obj["TileMap"].isMember("yScale"))
      level.SetScale(obj["TileMap"]["xScale"].asFloat(), obj["TileMap"]["yScale"].asFloat());
    level.constructlevel(manager);
#endif

    std::vector<GameObject> levelmaparray;
    //level.construct_ui(manager); //not needed bruh handled in the json
    MapTemplate map(obj["TileMap"]["Filename"].asCString(), levelmaparray);
    if (obj["TileMap"].isMember("xPos") && obj["TileMap"].isMember("yPos"))
    {
      glm::vec2 pos(obj["TileMap"]["xPos"].asFloat(), obj["TileMap"]["yPos"].asFloat());
      map.SetPos(pos);
    }
    if (obj["TileMap"].isMember("xScale") && obj["TileMap"].isMember("yScale"))
    {
      glm::vec2 scale(obj["TileMap"]["xScale"].asFloat(), obj["TileMap"]["yScale"].asFloat());
      map.SetScale(scale);
    }



    map.buildArray(manager);
    //map.constructmap();  

    for (unsigned int i = 0; i < manager.size(); ++i) {
        for (unsigned int j = 0; j < manager[i].size(); ++j) {
          manager[i][j].DeActivate();
        }
    }


    for (unsigned int i = 0; i < manager[0].size(); ++i) {
        manager[0][i].Activate();
    }
  }

}


void ReadBuffer(const char * filename, Json::Value * obj)
{
  std::ifstream inFile(filename);
  if (!inFile.is_open())
  {
    std::string err = filename;
    err + " not found";
    throw std::exception(err.c_str());
  }

  // create a Json Char Reader to read the filebuffer into a json value
  Json::Reader Parser;
  // create a string to store errors from parse
  // read the file into the jason value
  Parser.parse(inFile, *obj);
}


bool ObjectFactory::Serialize(GameObject const& gameObject, const char* filename)
{
  return true;
}

void ObjectFactory::DeSerializeBehaviorsAG(Json::Value::const_iterator::reference obj, const GameObject& s_game_object)
{
  auto && behavior = obj["Behaviors"];
  BehaviorParameters param{ s_game_object.GetName(), static_cast<behaviors>(behavior["BehaviorsType"].asInt()) };
  DeSerializeParameters(behavior, param);
  DelayedBehavior.emplace_back(std::move(param));
}


GameObject ObjectFactory::ArchetypeGameObject(const char*filename, glm::vec2 pos)
{
  std::ifstream inFile(filename);
  if (!inFile.is_open())
  {
    std::string err = filename;
    err + " not found";
    throw std::exception(err.c_str());
  }
  Json::Value obj;

  // create a Json Char Reader to read the filebuffer into a json value
  Json::Reader Parser;
  // create a string to store errors from parse
  // read the file into the jason value
  Parser.parse(inFile, obj);

  std::vector<std::string> components = obj.getMemberNames();
  Json::Value Type = obj.get("Type", Json::nullValue);
  Json::Value Sprite = obj.get("Sprite", Json::nullValue);
  Json::Value CSprite = obj.get("CircleSprite", Json::nullValue);
  Json::Value RigidBody = obj.get("RigidBody", Json::nullValue);
  Json::Value Collider = obj.get("Colliders", Json::nullValue);
  Json::Value Behave = obj.get("Behaviors", Json::nullValue);
  Json::Value Particles = obj.get("ParticleEmitter", Json::nullValue);
  Json::Value Health = obj.get("Health", Json::nullValue);
  Json::Value UISprite = obj.get("UISprite", Json::nullValue);
  Json::Value Fade = obj.get("FadeOut", Json::nullValue);
  Json::Value ParticleFade = obj.get("ParticleFadeOut", Json::nullValue);
  Json::Value DelayDestroy = obj.get("DelayedDestruction", Json::nullValue);
  Json::Value Rswitch = obj.get("RoomSwitch", Json::nullValue);
  Json::Value HPickup = obj.get("HealthPickup", Json::nullValue);
  Json::Value WorldText = obj.get("WorldText", Json::nullValue);

  std::string type = (Type != Json::nullValue) ? obj["Type"].asString() : "";

  std::string name =  obj.isMember("Name") ?   obj["Name"].asCString() :  type;
  GameObject s_game_object = GameObject(type, name);//adds transform



  Transform *T = s_game_object.Find<Transform>();

  T->setPosition(pos);
  obj["Transform"].isMember("xScale") ? T->setXScale(obj["Transform"]["xScale"].asFloat()) : T->setYScale(1.0f);
  obj["Transform"].isMember("yScale") ? T->setYScale(obj["Transform"]["yScale"].asFloat()) : T->setYScale(1.0f);
  // check if the object has a lifetime if so give it the lifetime
  // check if the object has a lifetime if so give it the lifetime
  //obj.isMember("LifeTime") ? s_game_object.LifeTime(obj["LifeTime"].asFloat()) : s_game_object.LifeTime(0.0f);// set the name of the object

  if (obj.isMember("Name"))
    s_game_object.SetName(obj["Name"].asCString());
  // set the movment value of the object (if it has one)
  s_game_object.MoveVal(0);

  //add components
  if (Sprite != Json::nullValue)
  {

    std::string file = obj["Sprite"]["FileName"].asCString();
    glm::vec2 size(obj["Sprite"]["SpriteWidth"].asFloat(), obj["Sprite"]["SpriteHeight"].asFloat());
    float a = obj["Sprite"]["Alpha"].asFloat();
    float r = obj["Sprite"]["Red"].asFloat();
    float g = obj["Sprite"]["Green"].asFloat();
    float b = obj["Sprite"]["Blue"].asFloat();
    Graphics::BlendMode bam = static_cast<Graphics::BlendMode>(obj["Sprite"]["BlendMode"].asUInt());
    float ba = obj["Sprite"]["BlendAlpha"].asFloat();
    size_t frame = static_cast<size_t>(obj["Sprite"]["Frame"].asLargestUInt());
    float depth = obj["Sprite"]["Depth"].asFloat();
    s_game_object.Add(new Graphics::Sprite(s_game_object, file.c_str(), size, depth, frame, ba, glm::vec4(r, g, b, a), bam));
  }

  if (WorldText != Json::nullValue)
  {
    DeSerializeWorldText(obj, s_game_object);
  }


  if (CSprite != Json::nullValue)
    DeSerializeCircleSprite(obj, s_game_object);

  if (UISprite != Json::nullValue) {
    ArchetypeUISprite(s_game_object, obj);
    DeSerializeUISprite(obj, s_game_object);
  }
  if (RigidBody != Json::nullValue)
  {
    s_game_object.Add(new KPE::RigidBody(s_game_object));

    if (obj["RigidBody"].isMember("Mass"))
      s_game_object.Find<KPE::RigidBody>()->setMass(obj["RigidBody"]["Mass"].asFloat());

    if (obj["RigidBody"].isMember("Force"))
    {
      if (obj["RigidBody"]["Force"].asBool())
      {
        s_game_object.Find<KPE::RigidBody>()->forcesActivate();
      }
      else
      {
        s_game_object.Find<KPE::RigidBody>()->forcesDeactivate();
      }
    }
  }


  if (Collider != Json::nullValue)
  {
    KPE::Colliders* collider = new KPE::Colliders(s_game_object);
    Transform * trans = s_game_object.Find<Transform>();

    if (Collider.isArray())
    {

      for (auto && box : obj["Colliders"])
      {
        collider->add(
          static_cast<KPE::BoundType>(box["BoundType"].asInt()),
          trans,
          box["xHalfSize"].asFloat(), box["yHalfSize"].asFloat(),
          box.get("xOffset", 0.0f).asFloat(), box.get("yOffset", 0.0f).asFloat()
        );
      }

    }
    else {
      auto && box = obj["Colliders"];
      collider->add(
        static_cast<KPE::BoundType>(box["BoundType"].asInt()),
        trans,
        box["xHalfSize"].asFloat(), box["yHalfSize"].asFloat(),
        box.get("xOffset", 0.0f).asFloat(), box.get("yOffset", 0.0f).asFloat()
      );
    }
    
    s_game_object.Add(collider);
  }

  if (HPickup != Json::nullValue)
  {
    int amt = HPickup.isMember("amt") ? obj["HealthPickup"]["amt"].asInt() : 0 ;
    s_game_object.Add(new HealthPickupComponent(s_game_object, amt));
  }

  if (Behave != Json::nullValue)
  {
    DeSerializeBehaviorsAG(obj, s_game_object);
  }

  if (Particles != Json::nullValue)
    DeSerializeParticle(obj, s_game_object);

  if (Health != Json::nullValue)
  {
    DeSerializeHealth(obj, s_game_object);
  }
  if (DelayDestroy != Json::nullValue)
  {
    s_game_object.Add(new DelayedDestruction{ s_game_object, DelayDestroy.get("Lifetime", 1.0f).asFloat() });
  }

  if (Fade != Json::nullValue)
  {
    s_game_object.Add(new FadeOut{ s_game_object, Fade.get("Interval", 1.0f).asFloat() });
  }

  if (ParticleFade != Json::nullValue)
  {
    s_game_object.Add(new ParticleFadeOut{ s_game_object, ParticleFade.get("Interval", 1.0f).asFloat() });
  }

  if (Rswitch != Json::nullValue)
  {
    int room = obj["RoomSwitch"].isMember("LoadLevel") ? obj["RoomSwitch"]["LoadLevel"].asInt() : 0;
    float x = obj["RoomSwitch"].isMember("Start") ? obj["RoomSwitch"]["Start"][0].asFloat() : 0;
    float y = obj["RoomSwitch"].isMember("Start") ? obj["RoomSwitch"]["Start"][1].asFloat() : 0;
    glm::vec2 _pos(x,y);
    s_game_object.Add(new RSwitchComponent(s_game_object,room, _pos ) );

  }

   return s_game_object;
}

glm::vec2 & ObjectFactory::DeSerializeVec2(Json::Value obj, glm::vec2 & vec)
{
  if (!obj.isArray())
    return vec;

  const size_t length = std::min(static_cast<size_t>(vec.length()), obj.size());
  for (size_t i = 0; i != length; ++i)
  {
    vec[i] = obj[i].asFloat();
  }

  return vec;
}

void ObjectFactory::DeSerializeTextSprite(Json::ValueConstIterator::reference obj, GameObject& object)
{
  auto textObj = obj["TextSprite"];
  const bool inWorldSpace = textObj.get("inWorldSpace", false).asBool();
  Graphics::TextSprite *spr =
    (inWorldSpace)
      ? object.Find<Graphics::WorldTextSprite>()
      : object.Find<Graphics::TextSprite>();


  if (inWorldSpace)
  {
    Graphics::WorldTextSprite *worldSpr = dynamic_cast<Graphics::WorldTextSprite *>(spr);

    if (worldSpr != nullptr)
    {
      if (textObj.isMember("Offset"))
      {
        glm::vec2 pos{worldSpr->getOffset()};
        DeSerializeVec2(textObj["Offset"], pos);
        worldSpr->setOffset(std::move(pos));
      }

      if (textObj.isMember("FontSize"))
        worldSpr->setFontSize(textObj["FontSize"].asFloat());
    }
  }

  // read the frame from the json file and store it in the component

  if (textObj.isMember("Frame"))
    spr->setFrame(textObj["Frame"].asUInt());

  //read the sprite size from the json file and store it in the component
  if (textObj.isMember("SpriteWidth") && textObj.isMember("SpriteHeight"))
    spr->setSize(Graphics::Size{ textObj["SpriteWidth"].asFloat(),
      textObj["SpriteHeight"].asFloat() });

  if (inWorldSpace)
    ReadGraphicsComponents<Graphics::WorldTextSprite>(object, obj, "TextSprite");
  else
    ReadGraphicsComponents<Graphics::TextSprite>(object, obj, "TextSprite");
}

void ObjectFactory::DeSerializeParticle(Json::Value::const_iterator::reference obj, GameObject& NewObj)
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if the archetype has a ParticleEmitter give the object a ParticleEmitter
  if (obj.isMember("ParticleEmitter"))
  {
    KPE::ParticleEmitter* emitter = new KPE::ParticleEmitter(NewObj);
    auto && inner = obj["ParticleEmitter"];

    // transform 
    emitter->setEmitterSize(glm::vec2(
      inner["Emitter Size X"].asFloat(),
      inner["Emitter Size Y"].asFloat()
    ));

    emitter->setRandomAngle(inner["Random Angle"].asBool());
    emitter->setAngle(inner["Angle"].asFloat());
    emitter->setAngleVariance(inner["Angle Variance"].asFloat());

    // miscellaneous 
    emitter->setWarmuptime(inner["Warmup Time"].asFloat());
    emitter->setMaxCount(inner["Count"].asUInt());

    emitter->setPower(glm::vec2(
      inner["Force X"].asFloat(),
      inner["Force Y"].asFloat()
    ));

    emitter->setPowerVariance(glm::vec2(
      inner["Force Variance X"].asFloat(),
      inner["Force Variance Y"].asFloat()
    ));

    emitter->setParticleSize(inner["Size"].asFloat());
    emitter->setSizeVariance(inner["Size Variance"].asFloat());

    emitter->setRate(inner["Rate"].asFloat());
    emitter->setRateVariance(inner["Rate Variance"].asFloat());



    emitter->setPLifetime(inner["Lifetime"].asFloat());
    emitter->setLifetimeVariance(inner["Lifetime Variance"].asFloat());

    NewObj.Add(emitter);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if the archetype has a ParticleAnimator give the object a ParticleAnimator
  if (obj.isMember("ParticleAnimator"))
  {
    auto && anim = obj["ParticleAnimator"];

    KPE::ParticleAnimator* animator = new KPE::ParticleAnimator(NewObj);

    glm::vec2 temp(0);
    temp.x = anim["RandForceX"].asFloat();
    temp.y = anim["RandForceY"].asFloat();
    animator->setRandomForce(temp);

    temp = glm::vec2(0);
    temp.x = anim["ForceX"].asFloat();
    temp.y = anim["ForceY"].asFloat();
    animator->setForce(temp);

    animator->setTorque(anim["Torque"].asFloat());
    animator->setGravity(anim["Gravity"].asBool());
    animator->setGrowth(anim["Growth"].asFloat());
    animator->setDampening(anim["Dampening"].asFloat());

    NewObj.Add(animator);
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if the archetype has a ParticleSystem
  if (obj.isMember("ParticleSystem"))
  {
    auto && objinner = obj["ParticleSystem"];

    KPE::ParticleSystem* system = new KPE::ParticleSystem(NewObj);

    system->loadTexture(objinner["Texture"].asCString());

    system->setSize(glm::vec2(objinner["Width"].asFloat(), objinner["Height"].asFloat()));

    system->setAlpha(objinner["Fill"].asFloat());

    system->setDepth(objinner["Depth"].asFloat());

    system->setAlpha(objinner["Alpha"].asFloat());

    system->setBlendColor(
      Graphics::Color(
        objinner["Red"].asFloat(),
        objinner["Green"].asFloat(),
        objinner["Blue"].asFloat(),
        objinner["BlendAlpha"].asFloat()
      )
    );

    system->setBlendMode(static_cast<Graphics::BlendMode>(objinner["BlendMode"].asInt()));

    system->setSystemSpace(
      static_cast<KPE::ParticleSystem::SystemSpace>(objinner["Space"].asInt())
    );

    system->setGeometry(
      static_cast<Graphics::Geometry>(objinner["Shape"].asInt())
    );

    NewObj.Add(system);
  }


}

void ObjectFactory::DeSerializeCircleSprite(Json::Value::const_iterator::reference obj, GameObject& NewObj)
{

  if (obj.isMember("CircleSprite"))
    NewObj.Add(new Graphics::CircleSprite(
      // Set the Parent of the object
      NewObj,
      // Set the img file for the sprite to load from
      obj["CircleSprite"]["FileName"].asCString(),
      // set the size of the sprite
      Graphics::Size{ obj["CircleSprite"]["SpriteWidth"].asFloat(), obj["CircleSprite"]["SpriteHeight"].asFloat() },
      // set the depth of the sprite
      obj["CircleSprite"]["Depth"].asFloat(),
      // tell the sprite which frame in the sprite sheet it is using
      obj["CircleSprite"]["Frame"].asUInt(),
      // set the alpha value of the sprite
      obj["CircleSprite"]["Alpha"].asFloat(),
      // set the collor of the sprite
      Graphics::Color{
        // Set the Red value of the Color
        obj["CircleSprite"]["Red"].asFloat(),
        // Set the Green value of the Color
        obj["CircleSprite"]["Green"].asFloat(),
        // Set the Blue value of the Color
        obj["CircleSprite"]["Blue"].asFloat(),
        // Set the Blend Alpha of the Color
        obj["CircleSprite"]["BlendAlpha"].asFloat() },
        // Set the Blend mode of the Sprite
        static_cast<Graphics::BlendMode>(obj["CircleSprite"]["BlendMode"].asInt())));

}

void ObjectFactory::ArchetypeUISprite(GameObject& NewObj, Json::Value::const_iterator::reference obj)
{
  NewObj.Add(new Graphics::UISprite(
    // Set the Parent of the object
    NewObj,
    // Set the img file for the sprite to load from
    obj["UISprite"]["FileName"].asCString(),
    // set the size of the sprite
    Graphics::Size{ obj["UISprite"]["SpriteWidth"].asFloat(), obj["UISprite"]["SpriteHeight"].asFloat() },
    // set the depth of the sprite
    obj["UISprite"]["Depth"].asFloat(),
    // tell the sprite which frame in the sprite sheet it is using
    obj["UISprite"]["Frame"].asUInt(),
    // set the alpha value of the sprite
    obj["UISprite"]["Alpha"].asFloat(),
    // set the collor of the sprite
    Graphics::Color{
      // Set the Red value of the Color
      obj["UISprite"]["Red"].asFloat(),
      // Set the Green value of the Color
      obj["UISprite"]["Green"].asFloat(),
      // Set the Blue value of the Color
      obj["UISprite"]["Blue"].asFloat(),
      // Set the Blend Alpha of the Color
      obj["UISprite"]["BlendAlpha"].asFloat() },
      // Set the Blend mode of the Sprite
      static_cast<Graphics::BlendMode>(obj["UISprite"]["BlendMode"].asInt())));
}


void ObjectFactory::DeSerializeCollider(Json::ValueConstIterator::reference obj, GameObject& object)
{
  if (obj.isMember("Colliders"))
  {
    for (unsigned i = 0; i < obj["Colliders"].size(); ++i)
    {
      KPE::Colliders* c = object.Find<KPE::Colliders>();
      c->add(static_cast<KPE::BoundType>(obj["Colliders"][i]["BoundType"].asInt()),
        object.Find<Transform>(),
        obj["Colliders"][i]["xHalfSize"].asFloat(),
        obj["Colliders"][i]["yHalfSize"].asFloat());
    }
  }
}

void ObjectFactory::DeSerializeUISprite(Json::ValueConstIterator::reference obj, GameObject& object)
{

  //object.Add(new Graphics::UISprite(object));

  // read the frame from the json file and store it in the component
  if (obj["UISprite"].isMember("Frame"))
    object.Find<Graphics::UISprite>()->setFrame(obj["UISprite"]["Frame"].asUInt());

  //read the sprite size from the json file and store it in the component
  if (obj["UISprite"].isMember("SpriteWidth") && obj["UISprite"].isMember("SpriteHeight"))
    object.Find<Graphics::UISprite>()->setSize(Graphics::Size{ obj["UISprite"]["SpriteWidth"].asFloat(),
      obj["UISprite"]["SpriteHeight"].asFloat() });

  ReadGraphicsComponents<Graphics::UISprite>(object, obj, "UISprite");

  
}


void ObjectFactory::DeSerializeHealth(Json::Value::const_iterator::reference obj, GameObject& game_object)
{
  int health;
  if (obj["Health"].isMember("Total"))
    health = obj["Health"]["Total"].asInt();
  else
    health = 0;

  unsigned int type;
  if (obj["Health"].isMember("Type"))
    type = obj["Health"]["Type"].asUInt();
  else
    type = HealthComponentType::HCTNONE;

  /*TODO: ADD COINS*/
  BulletType c = BulletType::NONE;
  if (obj["Health"].isMember("Color"))
     c = static_cast<BulletType>(obj["Health"]["Color"].asUInt());



  HealthComponent* h = new HealthComponent(game_object, static_cast<HealthComponentType>(type), health, c);
  game_object.Add(h);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LevelSerializer::LevelSerializer()
{
  // create a jason value
  Json::Value obj;

  ReadBuffer("data/json/Manifest.json", &obj);

  // determine the number of elements that need to be in an array
  Levels_.reserve(obj["Levels"].size());

  // loop through and store each level in the vector so we don't have to do more file io than required
  for (unsigned i = 0; i < obj["Levels"].size(); ++i)
    Levels_.push_back(obj["Levels"][i].asCString());
  currentLevel_ = Levels_.begin();
}

std::string LevelSerializer::CurrentLevel() {
  return *currentLevel_;
}

std::string LevelSerializer::SetNextLevel(const int level)
{
  currentLevel_ = Levels_.begin() + level;
  return *currentLevel_;
}


std::string LevelSerializer::SetNextLevel() {
  return *(++currentLevel_);
}


void LevelSerializer::clear() {
  this->~LevelSerializer();
  new(this) LevelSerializer;
}
