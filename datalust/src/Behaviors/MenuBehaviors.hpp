/**
  \file   MenuBehavior.hpp
  \author Zayd Gaudet

  \brief Behavior component for Menus

  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <GameObject\Component.h>
#include <Messaging\Messaging.hpp>

/**
 * \brief 
 */
class MenuBehavior : public Component {
public:
	MenuBehavior(GameObject &parent);
	~MenuBehavior();
	virtual Component* Clone_() const { return new MenuBehavior(*this); }
	virtual void Update(float dt);
	static void InputMenuUp(ObjectId, const Messaging::Message&);
	static void InputMenuDown(ObjectId, const Messaging::Message&);
	static void InputMenuSelect(ObjectId, const Messaging::Message&);
private:
	void onInputMenuUp(const Messaging::Message&);
	void onInputMenuDown(const Messaging::Message&);
	void onInputMenuSelect(const Messaging::Message&);
	float delay = 0.25f;
};