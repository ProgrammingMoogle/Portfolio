/******************************************************************************/
/*!
  \file    Component.h
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/18/2017

  \brief
	Component.h contains the Pure virtual class to be used by the GameObject
	class
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#ifndef COMPONENT_H
#define COMPONENT_H
#include <Util/Util.hpp>
/**
 * \brief 
 */
class GameObject;

//Base component class
/**
 * \brief 
 */
class Component
{
	friend class GameObject;
	friend class MenuComponent;
public:
	//! default constructor stores the parent information in the component
	Component(GameObject& parent) : Parent_(&parent) { }
	//! copy ctor for the Base Component class
	Component(Component const& rhs) : Parent_(rhs.Parent_) {}
	//! virtual destuctor
	virtual ~Component() {}
	//! clone function MUST BE IMPLEMENTED BY ALL CLASSES (recomended: implement using a copy ctor)
	virtual Component* Clone_() const = 0;

  virtual void Start() {}

	//! virtual update function SHOULD be implemented by all derived classes
	virtual void Update(float dt) { UNREFERENCED_PARAMETER(dt); }
	//! virtual draw function should be implemented by classes that need it
	virtual void Draw() const {}
	//! base class get parent function const version
	const GameObject& GetParent() const { return *Parent_; }
	//! base class get parent function 
	GameObject& GetParent() { return *Parent_; }
	//! function to get the key from a component
	ObjectId const GetKey() const { return key_; }
	//! function to set the key of a component
	ObjectId const& SetKey(ObjectId& key) { return key_ = key; }
private:
	//! GameObject the component belongs to
	GameObject* Parent_;
	ObjectId key_ = nullobj;
	//! Private function to be used to change the parent (this is used by very limitedly)
	virtual void Parent(GameObject* GameObject) { Parent_ = GameObject; }
};

#endif
