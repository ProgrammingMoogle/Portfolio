/**
  \file   AnimationController.hpp
  \author Samuel Cook

  \brief Class for handling Animations.

  Component of Graphic Objects that allows animation.

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#ifndef _ANIMATION_CONTROLLER_HPP_
#define _ANIMATION_CONTROLLER_HPP_

#include <unordered_map>
#include <vector>
#include <string>

namespace Graphics {
  class Sprite;

  class Animation {
    public:
      Animation(const char * name, const std::vector<std::pair<unsigned, float>>& framedata, bool loop = false);
      void play();
      void pause();
      void stop();
      bool update( float dt );
      unsigned getIndex();
      unsigned getFrame();
      const char * getName();
      bool isPlaying();
    private:
      std::string name_;
      std::vector<std::pair<unsigned, float>> frame_data_;
      unsigned frame_ = 0;
      unsigned frame_count_;
      float time_to_next_frame_;
      bool loop_ = false;
      bool playing_ = false;
  };

  class AnimationController {
    public:
      AnimationController(Sprite& sprite);
      ~AnimationController() = default;

      void load( const char *filename );

      void update( float dt );
      unsigned getFrame() const;

      void play( const char *animation );
      void play();
      void pause();
      void stop();

    private:
      Sprite *spr_;
      std::unordered_map<std::string, Animation> animations_;
      Animation* current_animation_;
  };
}

#endif