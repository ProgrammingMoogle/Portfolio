/******************************************************************************/
/*!
  \file    GameObject.cpp
  \author  Zayd Gaudet
  \par     zayd.gaudet\@digipen.edu
  \par     09/07/2017

  \brief
	definition of the GameObject class
  
  All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <algorithm>
#include <Transform/Transform.hpp>
#include <stdexcept>
#include "GameObject.h"
#include "Engine/Engine.h"

 //default constructor
GameObject::GameObject()
{
	throw(std::runtime_error("Don't call the dctor it isn't real"));
}

GameObject::GameObject(std::string type, std::string name) : type_(type), name_(name), Active(true) {
	Add(new Transform(*this));
}

// copy ctor makes a copy of a gameObject with an empty string for a name
GameObject::GameObject(const GameObject& rhs) : type_(rhs.type_), name_(rhs.name_), isDestroyed_(isDestroyed_), PInput_(rhs.PInput_), moveVal_(rhs.moveVal_), lifetime_(rhs.lifetime_), key_(rhs.key_), Active(rhs.Active) {
	//throw std::runtime_error("Don't be stupid 2.0");
	for (auto i : rhs.components_)
	{
		components_.insert(std::make_pair(i.first, i.second->Clone_()));
		components_.find(i.first)->second->Parent(this);
	}
}

GameObject::GameObject(GameObject&& o) noexcept :
	type_(std::move(o.type_)), name_(std::move(o.name_)), PInput_(std::move(o.PInput_)),
		moveVal_(std::move(o.moveVal_)), components_(std::move(o.components_)), isDestroyed_(std::move(o.isDestroyed_)), lifetime_(std::move(o.lifetime_)), key_(std::move(o.key_)), Active(std::move(o.Active)) {
	for (auto i : components_)
		i.second->Parent(this);
}


// equality operator, for the time being only returns false
bool GameObject::operator==(const GameObject& rhs) const
{
	if (&rhs == this)
		return true;
	return false;
}

// assignment operator utalizing the copy swap patern
GameObject& GameObject::operator=(GameObject const& rhs)
{
	swap(GameObject(rhs));
	return *this;
}

// swap function used in in the copy assignment operator
void GameObject::swap(GameObject& rhs)
{
	// swap the isDestroyed flag
	std::swap(isDestroyed_, rhs.isDestroyed_);
	std::swap(PInput_, rhs.PInput_);
	std::swap(lifetime_, rhs.lifetime_);
	std::swap(moveVal_, rhs.moveVal_);
	std::swap(key_, rhs.key_);
	// swap the name
	name_.swap(rhs.name_);
	// swap the type
	type_.swap(rhs.type_);
	//swap the unorderd_map of components
	components_.swap(rhs.components_);
  std::swap(Active, rhs.Active);
	for (auto it : components_)
	{
		it.second->Parent(this);
	}
	for (auto it : rhs.components_)
	{
		it.second->Parent(&rhs);
	}
}
// move assignment operator 
GameObject& GameObject::operator=(GameObject&& rhs) noexcept
{
	components_ = std::move(rhs.components_);
	isDestroyed_ = std::move(rhs.isDestroyed_);
	PInput_ = std::move(rhs.PInput_);
	lifetime_ = std::move(lifetime_);
	key_ = std::move(rhs.key_);
	name_ = std::move(rhs.name_);
	moveVal_ = std::move(rhs.moveVal_);
	type_ = std::move(rhs.type_);
  Active = std::move(rhs.Active);
	for (auto i : components_)
		i.second->Parent_ = this;
	return *this;
}

// flag the GameObject to be destroyed
void GameObject::Destroy() {
	isDestroyed_ = true;
}

// returns true if the object has been flaged to be destroyed else it false
bool GameObject::IsDestroyed() {
	return isDestroyed_;
}

void GameObject::Start()
{
  for (auto && component : components_)
    component.second->Start();
}

// loops through the vector of components and calls the updat function
void GameObject::Update(float dt)
{
  if (!IsActive()) return;

	// loop through the unordered map of components and update each one
	std::for_each(components_.begin(), components_.end(),
		[&dt](std::pair<std::type_index, Component*> o)
	{
		o.second->Update(dt);
	});
	if (lifetime_) { LifeTime_(dt); }
}

// returns the objects name
std::string& GameObject::GetName() {
	return name_;
}

std::string& GameObject::SetName(std::string name) {
	return name_ = name;
}


std::string const& GameObject::GetName() const {
	return name_;
}

std::string const& GameObject::GetType() const {
	return type_;
}

GameObject::~GameObject() noexcept {
	for (auto i : components_)
		delete i.second;
}

// returns a copy so users can't change the key value
ObjectId const& GameObject::SetObjectId(ObjectId& key) {
	for (auto i : components_)
		i.second->SetKey(key);
	key_ = key;
	return key;
}

/*
void GameObject::Wrap_()
{
	Transform * transform = Find<Transform>();
	glm::vec2 pos = transform->getPosition();
	if (pos.y <= ScreenWrap_)
	{
		pos.y = -1.7f * ScreenWrap_;
		transform->setPosition(pos);
		Find<KPE::RigidBody>()->setVelocity(glm::vec2{ 0,0 });
	}
}
*/

void GameObject::LifeTime_(float dt)
{
	lifetime_ -= dt;
	if (lifetime_ <= 0)
		engine.Find<GameStateManager>()->ChangeState();
}