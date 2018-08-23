/******************************************************************************/
/*!
	\file    MenuComponent.cpp
	\author  Zayd Gaudet
	\par     zayd.gaudet\@digipen.edu
	\par     12/03/2017
	
	\brief
		Deffinition of the GameState class
	
	All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <algorithm>
#include <Engine/Engine.h>
#include "MenuComponent.h"

MenuComponent::MenuComponent(GameObject& parent, std::vector<std::pair<Graphics::UISprite, int>>&& other) : Component(parent), choices_(other)
{
	currentSelection_ = choices_.begin();
}

MenuComponent::MenuComponent(MenuComponent const& other) : Component(other), choices_(other.choices_)
{
	size_t index = other.currentSelection_ - other.choices_.begin();
	currentSelection_ = choices_.begin() + index;
}

void MenuComponent::Update(float dt)
{
	std::for_each(choices_.begin(), choices_.end(), [&dt](std::pair<Graphics::UISprite, int> sprite)
	{
		sprite.first.Update(dt);
	});
}

Component* MenuComponent::Clone_() const
{
	return new MenuComponent(*this);
}

MenuComponent& MenuComponent::operator--()
{
	currentSelection_->first.setFrame(0);
	if (currentSelection_ == choices_.begin())
	{
		currentSelection_ = choices_.end() -1;
	}
	else
	{
		--currentSelection_;
	}
	currentSelection_->first.setFrame(1);  
	return *this;
}
MenuComponent& MenuComponent::operator++()
{
	currentSelection_->first.setFrame(0);
	++currentSelection_;
	if (currentSelection_ == choices_.end())
	{
		currentSelection_ = choices_.begin();
	}
	currentSelection_->first.setFrame(1);
	return *this;
}

void MenuComponent::PushState() const
{
	engine.Find<GameStateManager>()->CurrentState()->NewState(std::make_pair(true, currentSelection_->second));
}

void MenuComponent::Add(std::pair<Graphics::UISprite, int>&& o)
{
	size_t index = currentSelection_ - choices_.begin();
	choices_.push_back(o);
	if (!currentSelection_._Ptr)
	{
		currentSelection_ = choices_.begin();
	}
	else
		currentSelection_ = choices_.begin() + index;

}

void MenuComponent::Parent(GameObject* Parent)
{
	Parent_ = Parent;
	for (auto i = choices_.begin(); i != choices_.end(); ++i)
	{
		i->first.Parent(Parent);
	}
}
