/**
  \file   AnimationController.cpp
  \author Samuel Cook

  \brief Animation controller implementation

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#include "AnimationController.hpp"
#include "Sprite.hpp"
#include <json\json.h>
#include <fstream>
#include <Messaging/Messaging.hpp>
#include <Engine/Engine.h>
#include <Trace/Trace.hpp>

namespace Graphics {
	AnimationController::AnimationController(Sprite & sprite) : spr_(&sprite), current_animation_(nullptr) {
	}

	void AnimationController::load(const char * filename) {
		Json::Value root;
		std::ifstream fp(filename);
		if (fp.is_open()) {
			// create a Json Char Reader to read the filebuffer into a json value
			Json::Reader Parser;
			// create a string to store errors from parse
			// read the file into the jason value
			Parser.parse(fp, root);
      // get the number of animations
			unsigned ani_count = root["animations"].size();
			for (unsigned i = 0; i < ani_count; ++i) {
        // get the animation data
				std::string name = root["animations"][i]["name"].asCString();
				bool loop = root["animations"][i]["loop"].asBool();
				std::vector<std::pair<unsigned, float>> framedata;
				unsigned frame_count = root["animations"][i]["frame-data"].size();
        // read in frame data
				for (unsigned j = 0; j < frame_count; ++j) {
					Json::Value value = root["animations"][i]["frame-data"][j];
					framedata.push_back(std::make_pair(value["index"].asUInt(), value["duration"].asFloat()));
				}
        // add the animation to the table
				animations_.insert(std::make_pair(name, Animation(name.c_str(), framedata, loop)));
			}
		}
	}

	void AnimationController::update(float dt) {
    // check if an animation is playing
		if (current_animation_ && current_animation_->isPlaying()) {
      // update the animation
			if (current_animation_->update(dt)) {
        // set the frame
				spr_->setFrame(current_animation_->getIndex());

        // broadcast information about the animation
				Messaging::Message msg;
				msg.id = Messaging::ID::AnimationUpdate;
				msg.message.animation_update.sprite = spr_;
				msg.message.animation_update.animation = current_animation_->getName();
				msg.message.animation_update.frame = current_animation_->getFrame();
				msg.message.animation_update.is_playing = current_animation_->isPlaying();
				engine.Find<Messaging::MessageHandler>()->Post(msg);
			}
		}
	}

	unsigned AnimationController::getFrame() const {
		return current_animation_->getFrame();
	}

	void AnimationController::play(const char * animation) {
    // check if the animation exists
		if (animations_.find(animation) != animations_.end()) {
      // stop the current animation
			if (current_animation_) current_animation_->stop();
      // make the new animation the current one
			current_animation_ = &animations_.find(animation)->second;
      // restart the animation
			current_animation_->stop();
      // play the animation
			current_animation_->play();
			spr_->setFrame(current_animation_->getIndex());

      // broadcast information about the animation
			Messaging::Message msg;
			msg.id = Messaging::ID::AnimationUpdate;
			msg.message.animation_update.sprite = spr_;
			msg.message.animation_update.animation = current_animation_->getName();
			msg.message.animation_update.frame = current_animation_->getFrame();
			msg.message.animation_update.is_playing = current_animation_->isPlaying();
			engine.Find<Messaging::MessageHandler>()->Post(msg);
    } else {
      engine.Find<Trace::TraceHandler>()->TraceMessage( TraceLevel::WARNING, "Animation %s does not exist.", animation );
    }
	}
	void AnimationController::play() {
    // check if there is a currently set animation
		if (current_animation_) {
      // resume the animation
			current_animation_->play();

			spr_->setFrame(current_animation_->getIndex());

      // broadcast information about the animation
			Messaging::Message msg;
			msg.id = Messaging::ID::AnimationUpdate;
			msg.message.animation_update.sprite = spr_;
			msg.message.animation_update.animation = current_animation_->getName();
			msg.message.animation_update.frame = current_animation_->getFrame();
			msg.message.animation_update.is_playing = current_animation_->isPlaying();
			engine.Find<Messaging::MessageHandler>()->Post(msg);
		}
	}
	void AnimationController::pause() {
		if (current_animation_) current_animation_->pause();
	}
	void AnimationController::stop() {
		if (current_animation_) current_animation_->stop();
	}

	Animation::Animation(const char *name, const std::vector<std::pair<unsigned, float>>& framedata, bool loop) : name_(name), frame_data_(framedata), frame_count_(frame_data_.size()), time_to_next_frame_(frame_data_[0].second), loop_(loop) {
	}
	void Animation::play() {
    // mark the animation as playing
		playing_ = true;
	}
	void Animation::pause() {
    // mark the animation as not playing
		playing_ = false;
	}
	void Animation::stop() {
    // reset the animation and mark it as not playing
		frame_ = 0;
		time_to_next_frame_ = frame_data_[0].second;
		playing_ = false;
	}
	bool Animation::update(float dt) {
		bool _update = false; // < check if the animation changed frames this update
    // check if the animation is playing
		if (playing_) {
			time_to_next_frame_ -= dt;
      // advance the animation
			while (time_to_next_frame_ < 0) {
				++frame_;
				_update = true;
        // reset or loop if the animation reached the end
				if (frame_ == frame_count_) {
					frame_ = 0;
					if (!loop_) {
						stop();
						frame_ = frame_count_ - 1;
						return true;
					}
				}
				time_to_next_frame_ += frame_data_[frame_].second;
			}
		}
		return _update;
	}
	unsigned Animation::getIndex() {
		return frame_data_[frame_].first;
	}
	unsigned Animation::getFrame() {
		return frame_;
	}
	const char * Animation::getName() {
		return name_.c_str();
	}
	bool Animation::isPlaying() {
		return playing_;
	}
}
