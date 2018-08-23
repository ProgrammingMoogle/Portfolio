#pragma once
/******************************************************************************/
/*!
\file    main.cpp
\author  Zayd Gaudet
\par     zayd.gaudet\@digipen.edu
\par     12/03/2017

\brief
	cointains the engine loop

All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <stdlib.h>  
#include <crtdbg.h>  
#include <iostream>
#include <Audio/Audio.hpp>
//#include <Behaviors/Behaviors.hpp>
#include <Engine/Engine.h>
#include <GameObject/GameObject.h>
#include <GameObject/GameObjectManager.h>
#include <Graphics/Graphics.hpp>
#include <GameStateManager/GameStateManager.h>
#include <Input/Input.hpp>
#include <Messaging/Messaging.hpp>
#include <Physics/Physics.hpp>
#include <Util/Util.hpp>
#include <chrono>
#include <iomanip>
#include "Graphics/Window.hpp"

/**
 * \brief 
 * \return 
 */
int main()
{    
	std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();
	// Change in time (in seconds) since the last game loop.
	// A framerate controller is not required for this assignment.
  double duration = 0;

	//Initialize the game engine
  engine.Init();

  //int i = 0; //sarah temp
  
  // MAIN LOOP: Do not exit until instructed to do so by the game state manager (GSM).
  while (engine.IsRunning())
  {
    //{
    //  if (i++ == 0)
    //  {
    //    //SarahSandbox::BehaviorsTest();
    //  }
    //} //sarah temp

	std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
	std::chrono::duration<double> dt = current - prev;
	prev = current;
  duration += dt.count();
  if ( duration > 0.1 ) {
    std::stringstream title;
    title << std::setprecision(1) << std::fixed << "FPS: " << 1.0 / dt.count() << " | " << Graphics::WINDOW_TITLE << std::endl;
    engine.Find<Graphics::Manager>()->window()->setTitle( title.str().c_str() );
    duration -= 0.1;
  }
  
	// Update the game engine.
  	engine.Update(1.0f / 60.0f);

  }
  // Shutdown the game engine.
  engine.Shutdown();
  return 0;
}