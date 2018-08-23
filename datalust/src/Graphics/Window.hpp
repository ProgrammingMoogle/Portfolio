/**
  \file   Window.hpp
  \author Samuel Cook

  \brief 

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics/Sprite.hpp>


namespace Graphics {
  constexpr const char * const WINDOW_TITLE = "\tColorBlaster\t";

  /**
  \brief A Window.
  */
  class Window {
  public:
    /// Constructor
    Window(const char * title, size_t width = 1280, size_t height = 720, bool fullscreen = false);
    /// Check if the window is still open.
    bool isOpen() const;
    /// Process Window events.
    void process();
    /// Activate the windows OpenGL Context for rendering.
    void activate();
    /// Present the backbuffer to the screen.
    void swapBuffers();
    /// Get the width of the window.
    size_t width() const;
    /// Get the height of the window.
    size_t height() const;

    void setTitle( const char * title );

    void set_cursor(const std::string& file) const;

     glm::vec2 GetMousePos() const;
    // void attachInputHandler(Input::InputHandler * input) {input_ = input;}

     void setFullscreen();

     void deactivateCheats() const;

     void togglePause() const;

     bool isPaused() const;

  private:
    void toggleFullscreen();
    void createWindow(sf::VideoMode const& mode, const char * title, sf::Uint32 style);
    std::string title_;
    sf::Window handle_;
    bool fullscreen_;
    size_t xRes_, yRes_;
    sf::Vector2i lastPos_{ 0, 0 };
    // Input::InputHandler *input_;
  };
}