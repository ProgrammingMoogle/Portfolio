/*************************************************************************/
/*!
\file   ParticleFadeout.hpp
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
#include "Behaviors/FadeOut.hpp"

/**
 * \brief 
 */
class ParticleFadeOut : public FadeOut
{
public:
  ParticleFadeOut(GameObject & parent, float interval = 0);

  virtual void Update(float dt);
};