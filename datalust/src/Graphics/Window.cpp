/**
  \file   GraphicBase.hpp
  \author Samuel Cook

  \brief



  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/


#include "Messaging/Messaging.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics.hpp"
#include "Transform/Transform.hpp"
#include "Camera.hpp"
#include <Engine/Engine.h>
#include <iostream>

#include "Window.hpp"


#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics.hpp>
#include <GameObject/GameObject.h>

#if (WIN32 || _WIN32 || _WIN64)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#error "Unsupported OS"
#endif

namespace Graphics {
  bool paused_;
  bool cheatsActive_;
  GameObject* cheatConsole_;

  /*
    OpenGL debugging code from learnopengl.com

    copyright JoeyDeVries
  */
#ifdef _DEBUG
  void __stdcall glDebugOutput(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
  }
#endif
  /*
    End of OpenGL debugging code from learnopengl.com
  */
  //std::string file( "data\\img\\spr\\Reticle.png" );
 void Window::set_cursor(const std::string& file) const
{
      HCURSOR mouse = LoadCursorFromFile(file.c_str());
      SetCursor(mouse);
      SetClassLongPtr(handle_.getSystemHandle(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(mouse));
 }

 Window::Window(const char * title, size_t width, size_t height, bool fullscreen) 
   : fullscreen_{ fullscreen }, xRes_{ width }, yRes_{ height }
 {
    createWindow(
      sf::VideoMode{ width, height },
      title,
      sf::Style::Default
    );
    GLenum err = glewInit();
    if (GLEW_OK != err) throw std::runtime_error("Failed to initialize GLEW");



#ifndef _DEBUG
    {
      glClearColor(0, 0, 0, 0);
      glClear(GL_COLOR_BUFFER_BIT);
      handle_.display();
      toggleFullscreen();
    }
#endif

    /*
      OpenGL debugging code from learnopengl.com

      copyright JoeyDeVries
    */
#ifdef _DEBUG
    GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      //glDebugMessageCallback(glDebugOutput, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif
    /*
      End of OpenGL debugging code from learnopengl.com
    */

    handle_.setVerticalSyncEnabled(true);
    
    //handle hide mouse cursor FOR ccustomcursor
    handle_.setMouseCursorVisible(false);


    //might need this: set window icon
    //handle_.setIcon();
    set_cursor("data\\img\\spr\\Reticle.cur");
  }

  bool Window::isOpen() const {
    return handle_.isOpen();
  }

  void Window::process() 
  {
	sf::Event e;
	//bool pressed = false;
    while (handle_.pollEvent(e)) {
      switch (e.type) {
      case sf::Event::Closed:
        handle_.close();
        break;
      case sf::Event::Resized:
        activate();
        glViewport(0, 0, e.size.width, e.size.height);
        //Camera::WindowResize(e.size.width, e.size.height);
        break;

      case sf::Event::LostFocus: {
        Messaging::Message m{};
        m.id = Messaging::ID::LostFocus;
        engine.Find<Messaging::MessageHandler>()->Post(m);
      } break;

      case sf::Event::GainedFocus: {
        //handle hide mouse cursor FOR ccustomcursor
        handle_.setMouseCursorVisible(false);


        //might need this: set window icon
        //handle_.setIcon();
        set_cursor("data\\img\\spr\\Reticle.cur");
      }break;

      case sf::Event::KeyReleased:
        switch (e.key.code)
        {
        case sf::Keyboard::F11:
          //Toggles Fullscreen
          toggleFullscreen();
          break;

        case sf::Keyboard::Escape:
          //Activates the Pause Menu
          //paused_ = !paused_;
          //if (paused_)
          //{
          //  engine.Find<GameStateManager>()->PauseState(7);
          //  engine.pausePhysics();
          //}
          //else
          //{
          //  engine.Find<GameStateManager>()->UnpauseState(7);
          //  engine.unpausePhysics();
          //}
          break;

        //case sf::Keyboard::Tab:
        //  //Activates the Pause Menu
        //  //engine.Find<GameStateManager>()->ChangeState(8);
        //  break;

        default:
          break;
        }
        // intentional fall-through

        //Cheat mode activation
        if (e.key.shift) {
          
          if (e.key.code == sf::Keyboard::Tilde) {
            //Cannot create cheat console in these states
            if (engine.Find<GameStateManager>()->CurrentState()->getFilename_() == "data/json/MainMenu.json" ||
                engine.Find<GameStateManager>()->CurrentState()->getFilename_() == "data/json/HowToMenu.json" ||
                engine.Find<GameStateManager>()->CurrentState()->getFilename_() == "data/json/PauseMenu.json" ||
                engine.Find<GameStateManager>()->CurrentState()->getFilename_() == "data/json/SplashScreen.json" ||
                engine.Find<GameStateManager>()->CurrentState()->getFilename_() == "data/json/QuitScreen.json" ||
                engine.Find<GameStateManager>()->CurrentState()->getFilename_() == "data/json/CreditMenu.json") {
              continue;
            }
            //Cannot create cheat console in pause state
            if (paused_) {
              continue;
            }


            if (!cheatsActive_) {
              //Activate Cheat Mode
              cheatsActive_ = true;
              ObjectFactory::Create("data/json/CheatConsole.json",
                engine.Find<GameStateManager>()->CurrentState()->Manager());
            }
            else {
              cheatsActive_ = false;
              engine.Find<GameStateManager>()->CurrentState()->Manager().Find("CheatOptions").Destroy();
            }
          }
        }
      default:
        engine.Find<Input::InputHandler>()->GetInput(e);
        break;
      }
    }
  }

  void Window::activate() {
    handle_.setActive(true);
  }

  void Window::swapBuffers() {
    handle_.display();
  }

  size_t Window::width() const {
    unsigned x = handle_.getSize().x;
    return x;
  }

  size_t Window::height() const {
    unsigned y = handle_.getSize().y;
    return y;
  }

  void Window::setTitle(const char * title) {
    handle_.setTitle(title);
  }
  static glm::vec2 vectoritof(sf::Vector2i const& vec)
  {
    return glm::vec2(static_cast<float>(vec.x), static_cast<float>(vec.y));
  }


  glm::vec2 Window::GetMousePos() const
  {
    return vectoritof(sf::Mouse::getPosition(handle_));
  }

  void Window::toggleFullscreen()
  {
    fullscreen_ ^= 1;

    auto OSHandle = handle_.getSystemHandle(); // for OS-specific solution
#if (WIN32 || _WIN32 || _WIN64)
    if(fullscreen_)
    {
      // store last windowed mode setting
      lastPos_ = handle_.getPosition();
      xRes_ = handle_.getSize().x; yRes_ = handle_.getSize().y;

      // set position to 0, scale to entire screen
      handle_.setPosition(sf::Vector2i{ 0, 0 });
      const sf::VideoMode desktopDim =  sf::VideoMode::getDesktopMode();
      handle_.setSize(sf::Vector2u{ desktopDim.width, desktopDim.height });

      // remove borders
      LONG_PTR style = GetWindowLong(OSHandle, GWL_STYLE);
      style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
      SetWindowLong(OSHandle, GWL_STYLE, style);
    }
    else 
    {
      // restore windowed mode settings
      handle_.setPosition(lastPos_);
      handle_.setSize(sf::Vector2u{ xRes_, yRes_ });
      
      // Restore window borders
      LONG_PTR style = GetWindowLong(OSHandle, GWL_STYLE);
      style |= (WS_CAPTION | WS_THICKFRAME | WS_SYSMENU);
      SetWindowLong(OSHandle, GWL_STYLE, style);
    }

    // redraw the window
    SetWindowPos(OSHandle, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
    handle_.requestFocus();
#endif // WIN32 || _WIN32 || _WIN64

    //auto settings_ =  handle_.getSettings();
    //handle_.setActive(false);
    //handle_.close();

    //if (fullscreen_)
    //{
    //  handle_.create(
    //    sf::VideoMode::getDesktopMode(),
    //    title_,
    //    sf::Style::Default | sf::Style::Fullscreen,
    //    settings_
    //  );
    //}
    //else
    //{
    //  handle_.create(
    //    sf::VideoMode{ xRes_, yRes_ },
    //    title_,
    //    sf::Style::Default,
    //    settings_
    //  );
    //}

    //handle_.requestFocus();
    //handle_.setActive();

    //Graphics::SquareTexturedMesh::regenerateBuffers();
    //Graphics::Renderer::getCurrentRenderer()->regenerateCircleBuffer();
  }

  void Window::createWindow(sf::VideoMode const& mode, const char * title, sf::Uint32 style)
  {
    handle_.create(
      mode,
      sf::String{ title },
      style,
      sf::ContextSettings{ 24, 8, 0, 4, 5, sf::ContextSettings::Attribute::Debug }
    );
  }

  void Window::setFullscreen() {
    toggleFullscreen();
  }

  void Window::deactivateCheats() const {
    cheatsActive_ = false;
  }

  void Window::togglePause() const {
    if (!paused_) {
      paused_ = true;
    }
    else {
      paused_ = false;
    }
  }

  bool Window::isPaused() const {
    return paused_;
  }

}
