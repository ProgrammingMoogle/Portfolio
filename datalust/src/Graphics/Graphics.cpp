/**
  \file   Graphics.cpp
  \author Samuel Cook

  \brief Manages the graphics system (poorly, but it works)

  

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include <stdexcept>

#include <SFML/System.hpp>
#include <GL/glew.h>

#include "Mesh.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"
#include "Sprite.hpp"

#include <GameObject/GameObject.h>
#include <Transform/Transform.hpp>
#include <Engine/Engine.h>
#include <Messaging/Messaging.hpp>

#include <iostream>

#include "Renderer.hpp"
#include "Window.hpp"

namespace Graphics {
  Manager::Manager() : window_(new Window{ WINDOW_TITLE }),
    renderer_{ new Renderer{*window_} }
    { 
      renderer_->activate(); 
    }
  Manager::Manager(Window * w)
    : window_(w),
    renderer_(new Renderer{ *window_ })
  { 
    renderer_->activate();  
  }

    bool Manager::Init() {
    // register a handler for the debug draw key
    Messaging::MessageHandler *messaging = engine.Find<Messaging::MessageHandler>();
    messaging->Register( Messaging::ID::Debug, nullobj, Renderer::onDebugCommand );
    messaging->Register(Messaging::ID::PauseRender, nullobj, Renderer::onPauseCommand);
    return true;
  }

  void Manager::Update(float dt) {
    if(window_->isOpen()) {
      // process window messages
      window_->process();
      try {
        // render scene
        renderer_->draw();
      } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
      }
    } else {
      // send a window close message and tell the GSM to quit
      Messaging::Message msg;
      msg.id = Messaging::ID::WindowClose;
      engine.Find<Messaging::MessageHandler>()->Post(msg);
      engine.Find<GameStateManager>()->Quit();
    }
  }

  void Manager::Shutdown() {    
    // unregister the message handler and delete the window and renderer
    Messaging::MessageHandler *messaging = engine.Find<Messaging::MessageHandler>();
    messaging->Unregister( Messaging::ID::Debug, nullobj );
    delete window_;
    delete renderer_;
  }

  // DEPRECATED Test Code

 // namespace {
 //   
 // }
 // void testGraphics() {
 //   float y = 0;
 //   float x = 0;
 //   float angle = 0;
 //   float s = 0;
 //   Window window("GAME", 800, 600);
 //   Renderer renderer(window);
 //   renderer.activate();

 //   GameObject obj("Object", "obj");    
 //   //auto graphic = std::make_shared<Graphic>(obj, Graphics::Size{32, 32}, "data/img/spr/test.png");
	////new Graphics::Sprite( obj, "data/img/spr/test.png", Graphics::Size{ 16, 16 } );
 //   auto sprite = new Sprite(obj, "data/img/spr/test.png", ::Graphics::Size{16,16});
 //   auto transform = new Transform(obj);
 //   obj.Add(sprite);
 //   obj.Add(transform);

 //   GameObject cam("Camera", "camera");
 //   auto camera = new Camera(cam);
 //   auto transform2 = new Transform(cam);
 //   cam.Add(camera);
 //   cam.Add(transform2);
 //   //cam.Find(Camera)->activate();

	//  obj.Find<Sprite>()->setFrame(0xEF);
 //   while (window.isOpen()) {
 //     window.process();
 //     //physics update
 //     // angle += 0.05;
 //     // y = 100 * sin(angle);
 //     // x = 100 * cos(angle);
 //     // cam.Find(Transform)->setPosition(x, y);
 //     // cam.Find(Transform)->integrate(0);
 //     renderer.draw();
 //   }
 // }
}