#pragma once
/**
  \file   Player1Behavior.cpp
  \author Zayd Gaudet

  \brief Behavior component for player 1

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Player1Behavior.hpp"
#include "PunchBehaviors.hpp"
#include <Engine/Engine.h>
#include <Messaging/Messaging.hpp>
#include <Transform/Transform.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/Renderer.hpp>
#include <Physics/RigidBody.hpp>
#include <Behaviors/HUDBehavior.hpp>
#include <Audio/Audio.hpp>
#include <Behaviors/BulletBehavior.h>
#include "Graphics\Camera.hpp"
#include "Physics\ParticleSystem.hpp"
#include "Physics\ParticleEmitter.hpp"
#include "BulletType.h"
#include "Graphics/Window.hpp"
#include "CoinBehavior.h"
#include "HealthPickupComponent.h"
#include <GameStateManager/GameStateManager.h>


/**
 * \brief 
 * \param parent 
 * \param canshoot 
 */
Player1Behavior::Player1Behavior(GameObject &parent, bool canshoot) : Component(parent), CanShootRed(canshoot), CanShootBlue(canshoot) {

  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Register(Messaging::ID::MoveLeft, key, Player1Behavior::InputLeft);
  m_handler->Register(Messaging::ID::MoveRight, key, Player1Behavior::InputRight);
  m_handler->Register(Messaging::ID::MoveUp, key, Player1Behavior::InputUp);
  m_handler->Register(Messaging::ID::MoveDown, key, Player1Behavior::InputDown);
  m_handler->Register(Messaging::ID::Attack, key, Player1Behavior::InputShoot);
  m_handler->Register(Messaging::ID::AnimationUpdate, key, Player1Behavior::AnimationUpdate);
  //m_handler->Register(Messaging::ID::P1Win, key, Player1Behavior::InputF1);
  //	m_handler->Register(Messaging::ID::Jump, key, Player1Behavior::InputJump);
  m_handler->Register(Messaging::ID::Collision, key, Player1Behavior::CollisionHandle);

  m_handler->Register(Messaging::ID::Pause, key, Player1Behavior::Pause);
  m_handler->Register(Messaging::ID::LostFocus, key, Player1Behavior::FocusLost);

  m_handler->Register(Messaging::ID::Menu, key, Player1Behavior::PMenu);
  m_handler->Register(Messaging::ID::Quit, key, Player1Behavior::PQuit);
  m_handler->Register(Messaging::ID::Reset, key, Player1Behavior::PReset);
  m_handler->Register(Messaging::ID::Confirm, key, Player1Behavior::PConfirm);
  m_handler->Register(Messaging::ID::Deny, key, Player1Behavior::PDeny);

  dmg = 1;
}

/**
 * \brief 
 */
Player1Behavior::~Player1Behavior() {
  Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  ObjectId key = GetParent().GetKey();
  m_handler->Unregister(Messaging::ID::MoveLeft, key);
  m_handler->Unregister(Messaging::ID::MoveRight, key);
  m_handler->Unregister(Messaging::ID::MoveUp, key);
  m_handler->Unregister(Messaging::ID::MoveDown, key);
  //m_handler->Unregister(Messaging::ID::Jump, key);
  m_handler->Unregister(Messaging::ID::Attack, key);
  m_handler->Unregister(Messaging::ID::AnimationUpdate, key);
  //m_handler->Unregister(Messaging::ID::P1Win, key);
  m_handler->Unregister(Messaging::ID::Pause, key);
  m_handler->Unregister(Messaging::ID::LostFocus, key);

  //Pause Options
  m_handler->Unregister(Messaging::ID::Menu, key);
  m_handler->Unregister(Messaging::ID::Quit, key);
  m_handler->Unregister(Messaging::ID::Reset, key);
  m_handler->Unregister(Messaging::ID::Confirm, key);
  m_handler->Unregister(Messaging::ID::Deny, key);
}

/**
 * \brief 
 * \param ID 
 */
void Player1Behavior::RemoveInput(ObjectId ID)
{
	Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
  m_handler->Unregister(Messaging::ID::MoveLeft, ID);
  m_handler->Unregister(Messaging::ID::MoveRight, ID);
  m_handler->Unregister(Messaging::ID::MoveUp, ID);
  m_handler->Unregister(Messaging::ID::MoveDown, ID);
  m_handler->Unregister(Messaging::ID::Attack, ID);
}

/**
 * \brief 
 * \param ID 
 */
void Player1Behavior::ReturnInput(ObjectId ID)
{
	Messaging::MessageHandler* m_handler = engine.Find<Messaging::MessageHandler>();
	m_handler->Register(Messaging::ID::MoveLeft, ID, Player1Behavior::InputLeft);
	m_handler->Register(Messaging::ID::MoveRight, ID, Player1Behavior::InputRight);
	m_handler->Register(Messaging::ID::MoveUp, ID, Player1Behavior::InputUp);
	m_handler->Register(Messaging::ID::MoveDown, ID, Player1Behavior::InputDown);
	m_handler->Register(Messaging::ID::Attack, ID, Player1Behavior::InputShoot);
}

/**
 * \brief 
 * \param dt 
 */
void Player1Behavior::Update(float dt) {
  //suspect 1 p1hud
#if 0
  try {
    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find("P1Hud");
    HUDBehavior* b = object.Find<HUDBehavior>();
    if (b)
      b->setPlayer(GetParent().GetKey());
  }
  catch (...) {}
#endif

  if (!GetParent().IsDestroyed())
  {
    if (_timer > 0)
      _timer -= dt;
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::InputLeft(ObjectId obj, const Messaging::Message & message) {
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  if (object.GetName() == "Player1") {
    object.Find<Player1Behavior>()->onInputLeft(message);
  }
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::onInputLeft(const Messaging::Message & message) {


  KPE::RigidBody* body = GetParent().Find<KPE::RigidBody>();
  Transform* transform = GetParent().Find<Transform>();
  Graphics::Sprite* sprite = GetParent().Find<Graphics::Sprite>();
  glm::vec2 velocity = body->getVelocity();

  if (message.message.move_left.key_state == Messaging::KeyState::Pressed ||
    message.message.move_left.key_state == Messaging::KeyState::Held) {
    if (facing_ == right) {
      facing_ = left;
      transform->setXScale(-GetParent().Find<Transform>()->getXScale());
    }

    if (state_ != walking) {
      if (state_ != jumping) {
        sprite->play("walk");
        state_ = walking;
      }
    }

    velocity.x = -GetParent().MoveVal();
    body->setVelocity(velocity);

  }
  if (message.message.move_left.key_state == Messaging::KeyState::Released) {
    if (state_ = walking) {
      velocity.x = 0;
      body->setVelocity(velocity);
      sprite->play("idle");
      state_ = idle;
    }
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::InputRight(ObjectId obj, const Messaging::Message & message) {
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  if (object.GetName() == "Player1") {
    object.Find<Player1Behavior>()->onInputRight(message);
  }
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::onInputRight(const Messaging::Message & message) {

  KPE::RigidBody* body = GetParent().Find<KPE::RigidBody>();
  Transform* transform = GetParent().Find<Transform>();
  Graphics::Sprite* sprite = GetParent().Find<Graphics::Sprite>();
  glm::vec2 velocity = body->getVelocity();

  if (message.message.move_right.key_state == Messaging::KeyState::Pressed ||
    message.message.move_right.key_state == Messaging::KeyState::Held) {
    if (facing_ == left) {
      facing_ = right;
      transform->setXScale(-GetParent().Find<Transform>()->getXScale());
    }

    if (state_ != walking) {
      if (state_ != jumping) {
        sprite->play("walk");
        state_ = walking;
      }
    }

    velocity.x = GetParent().MoveVal();
    body->setVelocity(velocity);

  }
  if (message.message.move_right.key_state == Messaging::KeyState::Released) {
    if (state_ = walking) {
      velocity.x = 0;
      body->setVelocity(velocity);
      sprite->play("idle");
      state_ = idle;
    }
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::InputUp(ObjectId obj, const Messaging::Message & message) {
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  if (object.GetName() == "Player1") {
    object.Find<Player1Behavior>()->onInputUp(message);
  }
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::onInputUp(const Messaging::Message & message) {

  KPE::RigidBody* body = GetParent().Find<KPE::RigidBody>();
  Transform* transform = GetParent().Find<Transform>();
  Graphics::Sprite* sprite = GetParent().Find<Graphics::Sprite>();
  glm::vec2 velocity = body->getVelocity();

  if (message.message.move_up.key_state == Messaging::KeyState::Pressed ||
    message.message.move_up.key_state == Messaging::KeyState::Held) {
    if (facing_ == right) {
      facing_ = left;
      transform->setXScale(-GetParent().Find<Transform>()->getXScale());
    }

    if (state_ != walking) {
      if (state_ != jumping) {
        sprite->play("walk");
        state_ = walking;
      }
    }

    velocity.y = GetParent().MoveVal();
    body->setVelocity(velocity);

  }
  if (message.message.move_up.key_state == Messaging::KeyState::Released) {
    if (state_ = walking) {
      velocity.y = 0;
      body->setVelocity(velocity);
      sprite->play("idle");
      state_ = idle;
    }
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::InputDown(ObjectId obj, const Messaging::Message & message) {
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  if (object.GetName() == "Player1") {
    object.Find<Player1Behavior>()->onInputDown(message);
  }
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::onInputDown(const Messaging::Message & message) {

  KPE::RigidBody* body = GetParent().Find<KPE::RigidBody>();
  Transform* transform = GetParent().Find<Transform>();
  Graphics::Sprite* sprite = GetParent().Find<Graphics::Sprite>();
  glm::vec2 velocity = body->getVelocity();

  KPE::ParticleEmitter * emitter = GetParent().Find<KPE::ParticleEmitter>();

  if (message.message.move_down.key_state == Messaging::KeyState::Pressed ||
    message.message.move_down.key_state == Messaging::KeyState::Held) {
    if (facing_ == right) {
      facing_ = left;
      transform->setXScale(-GetParent().Find<Transform>()->getXScale());
    }

    if (state_ != walking) {
      if (state_ != jumping) {
        sprite->play("walk");
        state_ = walking;
      }
    }

    velocity.y = -GetParent().MoveVal();
    body->setVelocity(velocity);
    /*
    emitter->setPower(
      -1.0f * glm::vec2(
        std::cos(std::atan(
            velocity.y / velocity.x)
        ) * emitter->getPower().x,

        std::sin(std::atan(
          velocity.y / velocity.x)
        ) * emitter->getPower().y)
      );
    emitter->setMaxCount(0);*/
  }
  if (message.message.move_down.key_state == Messaging::KeyState::Released) {
    if (state_ = walking) {
      velocity.y = 0;
      body->setVelocity(velocity);
      sprite->play("idle");
      state_ = idle;
    }
    /*emitter->setMaxCount(1);*/

  }
}
/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::InputShoot(ObjectId obj, const Messaging::Message& message)
{
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  if (object.GetName() == "Player1") {
    object.Find<Player1Behavior>()->onInputShoot(message);
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::AnimationUpdate(ObjectId obj, const Messaging::Message & message) {
#if 0 //dunno if this even needed
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  if (object.GetName() == "Player1") {
    object.Find<Player1Behavior>()->onAnimationUpdate(message);
  }
#endif
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::onAnimationUpdate(const Messaging::Message & message) {
  /*if (std::string(message.message.animation_update.animation) == "punch") {
    if (message.message.animation_update.is_playing == false)
      isPunching_ = false;
  }*/
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::CollisionHandle(ObjectId obj, const Messaging::Message& message) {
  /*GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  Player1Behavior* pb = object.Find<Player1Behavior>();
  pb->onCollisionHandle(message);*/

  KPE::RigidBody* KeyBody = nullptr;

  if (message.message.collision.body_->GetParent().GetKey() == obj) {
    KeyBody = message.message.collision.body_;
  }
  else if (message.message.collision.otherBody_->GetParent().GetKey() == obj) {
    KeyBody = message.message.collision.otherBody_;
  }



  if (message.message.collision.otherBody_->GetParent().GetType() == message.message.collision.body_->GetParent().GetType()) {
    //const_cast<Messaging::Message&>(message.message.collision.pass_ ) = false;
    *(message.message.collision.pass_) = false;
    return;
  }
  if (KeyBody == nullptr)
  {
    return;
  }


  //GameState* state = engine.Find<GameStateManager>()->CurrentState();
  //GameObjectManager& manager = state->Manager();
  GameObject& object = KeyBody->GetParent(); //manager.Find(obj);

                                             //if (object.GetKey() ==  obj)
                                             //Messaging::Message& temp = const_cast<Messaging::Message&>(message);
  object.Find<Player1Behavior>()->onCollisionHandle(message);
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::onCollisionHandle(const Messaging::Message& message) {

  KPE::RigidBody* collidedBody = nullptr;
  KPE::RigidBody* otherBody = nullptr;

  if (this == nullptr || message.message.collision.body_ == nullptr || message.message.collision.otherBody_ == nullptr) return;

  if (message.message.collision.body_ == GetParent().Find<KPE::RigidBody>()) 
  {
    collidedBody = message.message.collision.otherBody_;
    otherBody = message.message.collision.body_;
  }
  else
  {
    collidedBody = message.message.collision.body_;
    otherBody = message.message.collision.otherBody_;
  }

  if (!godMode_ && (collidedBody->GetParent().GetType() == "Enemy" || collidedBody->GetParent().GetType() == "Bullet"))
  {
    GetParent().Find<HealthComponent>()->DoDamage(1);//enemy damage
    return;
  }

  if (collidedBody->GetParent().GetType() == "HealthPickup")
  {
    int amt = collidedBody->GetParent().Find<HealthPickupComponent>()->GetAmt();
    AddHealthToPlayer(amt);
    collidedBody->GetParent().Destroy();
    return;
  }

  if (collidedBody->GetParent().GetType() == "RedGun")
  {
    EnableRedGun();
    collidedBody->GetParent().Destroy();
    return;
  }

  if (collidedBody->GetParent().GetType() == "BlueGun")
  {
    EnableBlueGun();
    collidedBody->GetParent().Destroy();
    return;
  }
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::Shoot(const Messaging::Message& message)
{
  GameObject& obj = GetParent();
  //Graphics::Sprite* sprite = obj.Find<Graphics::Sprite>();

  GameObjectManager& manager = engine.Find<GameStateManager>()->CurrentState()->Manager();


  std::vector<std::reference_wrapper<GameObject>> punches;

  switch (message.message.attack.color) {
  case BulletType::BLUE: { //shoot flag red & blue
    if (CanShootBlue)
      punches = ObjectFactory::Create("data/json/Punch.json", manager);
    else
      return;
    break;
  }
  case BulletType::RED: { //shoot flag red & blue
    if (CanShootRed)
      punches = ObjectFactory::Create("data/json/Punch.json", manager);
    else
      return;
    break;
  }
  case BulletType::COINS: { if (coins > 0) {
    punches = ObjectFactory::Create("data/json/Coin.json", manager);
    RemoveCoin();
  }
   else
     return; //do nothing
    break;
  }
  default: {//((message.message.attack.color == BulletType::COINS) && (coins <= 0)) //do not create
    return;
  }
  }
  // grab the punch object we just created
 
    //if ( == nullptr)
      //continue;

  auto && Punch = punches[0].get();
    //Punch.LifeTime(1.54);
   //replace with new behavior
   //Punch.Find<BulletBehavior>()->AttachPlayer(Punch.GetKey());
    BulletBehavior * b = nullptr;
    CoinBehavior * c = nullptr;

    if (!(message.message.attack.color == BulletType::COINS))
    {
      b = Punch.Find<BulletBehavior>();

      if (b == nullptr)
        return;
      b->SetDMG(dmg);
    }
    else
    {
      c = Punch.Find<CoinBehavior>();

      if (c == nullptr)
        return;
    }

    // player stuff
    glm::vec2 pos = glm::vec2(Graphics::Camera::getActiveCamera()->MouseToWorld());

    std::cout << pos.x << " " << pos.y << '\n';
    glm::vec2 const& origin = GetParent().Find<Transform>()->getPosition();
    glm::vec2 pos2 = pos - origin;
    pos2 = normalize(pos2);
    glm::vec2 newpos((pos2 * 80.0f) + origin + GetParent().Find<KPE::RigidBody>()->getVelocity() * (1.0f / 60.0f));

    pos2 *= BULLETSPEED;



    Punch.Find<Transform>()->setPosition(newpos);


    Punch.Find<KPE::RigidBody>()->addVelocity(pos2);

    Graphics::CircleSprite *_sprite = Punch.Find<Graphics::CircleSprite>();
    if (_sprite == nullptr)  return;


    KPE::ParticleSystem * _particleSys = Punch.Find<KPE::ParticleSystem>();
    if (_particleSys == nullptr) return;

    isShooting = true;

    Graphics::Window* w = engine.Find<Graphics::Manager>()->window();

    if (message.message.attack.color == BulletType::COINS)
    {
      _particleSys->setBlendColor(Graphics::Color{ 0.96f, 0.96f, 0.0f, 1.0f });
      _sprite->setBlendColor(Graphics::Color(1, 1, 0, 1));
      w->set_cursor("data\\img\\spr\\MiddleClickReticle.cur");
      c->SetBullet(BulletType::COINS);
      return;
    }
    else if (message.message.attack.color == BulletType::RED) // red
    {
      _particleSys->setBlendColor(Graphics::Color{ 0.5f, 0.01f, 0.01f, 1.0f });
      _sprite->setBlendColor(Graphics::Color(1, 0, 0, 1));
      w->set_cursor("data\\img\\spr\\LeftClickReticle.cur");
      b->SetBullet(BulletType::RED);
      return;
    }
    else if (message.message.attack.color == BulletType::BLUE) // blue
    {
      _particleSys->setBlendColor(Graphics::Color{ 0.01f, 0.01f, 0.5f, 1.0f });
      _sprite->setBlendColor(Graphics::Color(0, 0, 1, 1));
      w->set_cursor("data\\img\\spr\\RightClickReticle.cur");
      b->SetBullet(BulletType::BLUE);
      return;
    }
  
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::onInputShoot(const Messaging::Message& message)
{

  if (message.message.attack.Mouse_state == Messaging::MouseState::Pressed) {

    //collider->add(KPE::BoundType::BoundHitBox, transform, 10.5f * transform->getXScale(), 9.5f * transform->getYScale());
    //isPunching_ = true;
    //state_ = idle;

    //sprite->play("punch");
    Audio::AudioManager *audio_manager = engine.Find<Audio::AudioManager>();

    if (checktimer())
    {
      _timer = delay;
      Shoot(message);


      if (isShooting)
      {
        audio_manager->PlaySound("Magic_SpiritBlast_Ricochet4");
        audio_manager->SetSoundPitch(2.0f, "Magic_SpiritBlast_Ricochet4");
        audio_manager->SetSoundPitch(((rand() / (float)RAND_MAX)) + 1.0f, "Magic_SpiritBlast_Ricochet4");
      }
      isShooting = false;
    }
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::InputF1(ObjectId obj, const Messaging::Message& message)
{
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  object.Find<Player1Behavior>()->onInputF1(message);
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::onInputF1(const Messaging::Message& message) {
  if (message.message.win.key_state == Messaging::KeyState::Pressed)
    if (!isWinnning_)
    {
      isWinnning_ = true;
      ObjectFactory::Create("data/json/Player1Wins.json", engine.Find<GameStateManager>()->CurrentState()->Manager());
    }
}

/**
 * \brief 
 */
void Player1Behavior::activateGodMode() {
  godMode_ = true;
  AddToDMG(100);
  //HealthComponent::OnePunchMan = true;
}

/**
 * \brief 
 */
void Player1Behavior::deactivateGodMode() {
  godMode_ = false;
  dmg = 1;
  //HealthComponent::OnePunchMan = false;
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::Pause(ObjectId obj, const Messaging::Message& message) {

  if (message.message.pause.key_state == Messaging::KeyState::Released) {

    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "Player1")
    {
      object.Find<Player1Behavior>()->togglePause();
    }
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::FocusLost(ObjectId obj, const Messaging::Message & message)
{
  engine
    .Find< GameStateManager>()
      ->CurrentState()
        ->Manager()
          .Find(obj)
            .Find<Player1Behavior>()->onFocusLost(message);
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::onFocusLost(const Messaging::Message & message)
{
  if (!paused_)
    togglePause();
}

/**
 * \brief 
 */
void Player1Behavior::togglePause() {
  if (confirm_status_ == ConfirmStatus::None) {
    if (!paused_) {
      paused_ = true;
      engine.Find<GameStateManager>()->PauseState(0);
      Graphics::Renderer::getCurrentRenderer()->window()->togglePause();
      engine.GetPhysicsEngine()->togglePause();
    }
    else {
      paused_ = false;
      engine.Find<GameStateManager>()->UnpauseState(0);
      Graphics::Renderer::getCurrentRenderer()->window()->togglePause();
      engine.GetPhysicsEngine()->togglePause();
    }
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::PMenu(ObjectId obj, const Messaging::Message& message) {
  if (message.message.pause.key_state == Messaging::KeyState::Released) {

    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "Player1")
    {
      object.Find<Player1Behavior>()->PMenuHandle(message);
    }
  }
}
/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::PQuit(ObjectId obj, const Messaging::Message& message) {
  if (message.message.pause.key_state == Messaging::KeyState::Released) {

    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "Player1")
    {
      object.Find<Player1Behavior>()->PQuitHandle(message);
    }
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::PReset(ObjectId obj, const Messaging::Message & message)
{
  GameState* state = engine.Find<GameStateManager>()->CurrentState();
  GameObjectManager& manager = state->Manager();
  GameObject& object = manager.Find(obj);
  if (object.GetName() == "Player1")
  {
    object.Find<Player1Behavior>()->PResetHandle(message);
  }
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::PResetHandle(const Messaging::Message & message)
{
  if (paused_ && confirm_status_ == None) {
    confirm_status_ = ResetLevel;

    //Spawn a confirmation message
    confirmKey_ = ObjectFactory::Create("data/json/Confirm.json",
      engine.Find<GameStateManager>()->CurrentState()->Manager()).front().get().GetKey();
  }
}

/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::PConfirm(ObjectId obj, const Messaging::Message& message) {
  if (message.message.pause.key_state == Messaging::KeyState::Released) {

    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "Player1")
    {
      object.Find<Player1Behavior>()->PConfirmHandle(message);
    }
  }
}
/**
 * \brief 
 * \param obj 
 * \param message 
 */
void Player1Behavior::PDeny(ObjectId obj, const Messaging::Message& message) {
  if (message.message.pause.key_state == Messaging::KeyState::Released) {

    GameState* state = engine.Find<GameStateManager>()->CurrentState();
    GameObjectManager& manager = state->Manager();
    GameObject& object = manager.Find(obj);
    if (object.GetName() == "Player1")
    {
      object.Find<Player1Behavior>()->PDenyHandle(message);
    }
  }
}


/**
 * \brief 
 * \param message 
 */
void Player1Behavior::PMenuHandle(const Messaging::Message& message) {
  if (paused_ && confirm_status_ == None) {
    confirm_status_ = QuitMenu;

    //Spawn a confirmation message
    confirmKey_ = ObjectFactory::Create("data/json/Confirm.json",
      engine.Find<GameStateManager>()->CurrentState()->Manager()).front().get().GetKey();
  }
}

/**
 * \brief 
 * \param message 
 */
void Player1Behavior::PQuitHandle(const Messaging::Message& message) {
  if (paused_ && confirm_status_ == None) {
    confirm_status_ = QuitGame;

    //Spawn a confirmation message
    confirmKey_ = ObjectFactory::Create("data/json/Confirm.json",
      engine.Find<GameStateManager>()->CurrentState()->Manager()).front().get().GetKey();
  }
}
/**
 * \brief 
 * \param message 
 */
void Player1Behavior::PConfirmHandle(const Messaging::Message& message) {
  if (paused_) {
    switch (confirm_status_)
    {
    case QuitMenu: {
      backToMenu();
    }break;

    case QuitGame: {
      quit();
    }break;

    case ResetLevel: {
      reset();
    }break;

    default: {
    }break;
    }
  }
}
/**
 * \brief 
 * \param message 
 */
void Player1Behavior::PDenyHandle(const Messaging::Message& message) {
  if (paused_ && confirm_status_ != None) {
    //Destroy the confirmation message

    try
    {
      GameObject & confirm = engine.Find<GameStateManager>()->CurrentState()->Manager().Find(confirmKey_);
      confirm.DeActivate();
      confirm.Destroy();
      confirm_status_ = None;
    }
    catch (int)
    {
    }
  }
}

/**
 * \brief 
 */
void Player1Behavior::backToMenu() {
  GameStateManager *const& GSM = engine.Find<GameStateManager>();

  GSM->UnpauseState(
    GSM->CurrentState()->NewState().second
  );
  GSM->DelayedPopState();
  GSM->DelayedPushState(1);
  engine.GetPhysicsEngine()->togglePause();
  Graphics::Renderer::getCurrentRenderer()->window()->deactivateCheats();
}
/**
 * \brief 
 */
void Player1Behavior::quit() {
  engine.Quit();
}

/**
 * \brief 
 */
void Player1Behavior::reset()
{
  GameStateManager *const& GSM = engine.Find<GameStateManager>();
  GSM->UnpauseState(
    GSM->CurrentState()->NewState().second
  );
  GSM->DelayedResetState();
  engine.GetPhysicsEngine()->togglePause();
  Graphics::Renderer::getCurrentRenderer()->window()->deactivateCheats();
}
