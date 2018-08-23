/**
  \file   CameraBehavior.hpp
  \author Samuel Cook

  \brief Behavior component for camera

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <GameObject/Component.h>
#include <string>
  
/**
 * \brief 
 */
class CameraBehavior : public Component {
public:
	CameraBehavior(GameObject& parent);

	virtual Component* Clone_() const { return new CameraBehavior(*this); }

	virtual void Update(float dt);

	bool scrollLeft() const { return scroll_left; }
	bool scrollRight() const { return scroll_right; }

private:
	bool scroll_left = false, scroll_right = false, isDone = false;
	ObjectId boundry1, boundry2;

	ObjectId CreateBoundry(const std::string &name, const std::string &player);
};