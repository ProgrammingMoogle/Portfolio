/******************************************************************************/
/*!
  \file    MenuComponent.h
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     12/02/2017

  \brief
  Menu Component.h is contains the deffinition for a derived class of component 
  used to facilitate menu's
  class
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef MENUCOMPONENT_H
#define MENUCOMPONENT_H
#include <vector>
#include <Graphics/Sprite.hpp>
#include <GameObject/Component.h>
class GameObject;

//Base component class
class MenuComponent : public Component
{
	friend class GameObject;
public:
	// default constructor stores the parent information in the component
	MenuComponent(GameObject& parent, std::vector<std::pair<Graphics::UISprite, int>>&& other);
	MenuComponent(MenuComponent const& o);
	// virtual destuctor
	virtual ~MenuComponent() = default;
	// update function for the MenuComponent class, updates all of the sprites
	virtual void Update(float dt);
	// clone function MUST BE IMPLEMENTED BY ALL CLASSES (recomended: implement using a copy ctor)
	virtual Component* Clone_() const;
	// decrement the iterator into the vector of menu boxes, move it down and change which box is active
	MenuComponent& operator--();
	// increment the iterator into the vector of menu boxes, move it up and change which box is active
	MenuComponent& operator++();
	// change the state based on whatever box the player selected
	void PushState() const;
	// add more sprites to the menu component
	void Add(std::pair<Graphics::UISprite, int>&& o);
private:
	virtual void Parent(GameObject* GameObject);
	// vector containing all of the choices in the menu
	std::vector<std::pair<Graphics::UISprite, int>> choices_;
	std::vector<std::pair<Graphics::UISprite, int>>::iterator currentSelection_;
};

#endif //MENUCOMPONENT_H
