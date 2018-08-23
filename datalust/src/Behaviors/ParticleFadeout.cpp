/*************************************************************************/
/*!
\file   ParticleFadeout.cpp
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
#include "GameObject/GameObject.h"
#include "Physics/ParticleSystem.hpp"
#include "Behaviors/ParticleFadeout.hpp"

/**
 * \brief 
 * \param parent 
 * \param interval 
 */
ParticleFadeOut::ParticleFadeOut(GameObject& parent, float interval) : FadeOut(parent, interval) {}

/**
 * \brief 
 * \param dt 
 */
void ParticleFadeOut::Update(float dt)
{
  fade.fade(dt);
  GetParent().Find<KPE::ParticleSystem>()->setBlendColor(fade.interp());
}