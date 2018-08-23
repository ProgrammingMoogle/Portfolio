/*************************************************************************/
/*!
\file   EnemyBehavior.cpp
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
#include <glm/geometric.hpp>
#include "Engine/Engine.h"
#include "Physics/RigidBody.hpp"
#include "Transform/Transform.hpp"
#include "EnemyBehavior.hpp"
#include "Messaging/Messaging.hpp"

/**
 * \brief 
 * \param parent 
 * \param _movespeed 
 * \param _canShoot 
 * \param _shootplayer 
 * \param _shootDir 
 * \param _bulletSpeed 
 * \param _shootDelay 
 */
EnemyBehavior::EnemyBehavior(
  GameObject & parent,
  float _movespeed,
  bool _canShoot,
  bool _shootplayer,
  glm::vec2 _shootDir,
  float _bulletSpeed,
  float _shootDelay
) :
  Component{ parent },
  movespeed{_movespeed},
  canShoot{ _canShoot },
  shootPlayer{_shootplayer},
  shootDir{ glm::normalize(_shootDir) },
  bulletSpeed{_bulletSpeed},
  delay{ std::abs(_shootDelay) },
  timer{ std::abs(_shootDelay) }
{
}

/**
 * \brief 
 * \param dt 
 */
void EnemyBehavior::Update(float dt)
{
  if (canShoot)
  {
    timer -= dt;
    if (timer <= 0)
    {
      glm::vec2 const dir = ((shootPlayer) ? getPlayerDir() : shootDir);
      glm::vec2 const vel = bulletSpeed * dir;
      glm::vec2 const pos = GetParent().Find<Transform>()->getPosition();
      GameObjectManager& manager = engine.Find<GameStateManager>()->CurrentState()->Manager();
      do
      {
        auto && created = ObjectFactory::Create("data/json/EnemyBullet.json", manager);
          
        for (auto && bullet : created)
        {
          Transform * trans = bullet.get().Find<Transform>();
          if(trans == nullptr) continue;
          trans->setPosition(pos);
          trans->Update(0);

          KPE::RigidBody * body = bullet.get().Find<KPE::RigidBody>();
          if (body == nullptr) continue;
          body->setVelocity(vel.x, vel.y);
        }


      } while ((timer += delay) <= 0);

      
    }
  }
}

/**
 * \brief 
 * \param _shoots 
 */
void EnemyBehavior::setCanShoot(bool _shoots)
{
  canShoot = _shoots;
}

/**
 * \brief 
 * \param _targetPlayer 
 */
void EnemyBehavior::setShootsPlayer(bool _targetPlayer)
{
  shootPlayer = _targetPlayer;
}

/**
 * \brief 
 * \param _dir 
 */
void EnemyBehavior::setShootDir(glm::vec2 const & _dir)
{
  shootDir = glm::normalize(_dir);
}

/**
 * \brief 
 * \param _dir 
 */
void EnemyBehavior::setShootDir(glm::vec2 && _dir)
{
  shootDir = glm::normalize(std::move(_dir));
}

/**
 * \brief 
 * \param shootSpeed 
 */
void EnemyBehavior::setBulletSpeed(float shootSpeed)
{
  bulletSpeed = std::abs(shootSpeed);
}

/**
 * \brief 
 * \param _movespeed 
 */
void EnemyBehavior::setMoveSpeed(float _movespeed)
{
  movespeed = _movespeed;
}



/**
 * \brief 
 * \param us 
 * \param them 
 * \param collision 
 */
void EnemyBehavior::OnCollisionEnter(KPE::RigidBody& us, KPE::RigidBody & them, Messaging::Collision const& collision)
{
  if (them.GetParent().GetType() == "EnemyBullet" ||
      them.GetParent().GetType() == "Punch")
    *collision.pass_ = false;
}

/**
 * \brief 
 * \return 
 */
glm::vec2 const& EnemyBehavior::getPlayerDir() const
try
{
  Transform const * const eTrans = GetParent().Find<Transform>();
  if (eTrans == nullptr) return shootDir;

  Transform const * const pTrans = engine.Find<GameStateManager>()
    ->CurrentState()
      ->Manager()
        .Find("Player1")
          .Find<Transform>();
  if (pTrans == nullptr) return shootDir;
  
  static glm::vec2 retval;
  return retval = glm::vec2{ glm::normalize(pTrans->getPosition() - eTrans->getPosition()) };
}
catch (int)
{
  return shootDir;
}
