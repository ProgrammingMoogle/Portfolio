#include "PauseState.h"

void PauseState::Initialize()
{
	//Unsure if this is correct
	
	///////////////////////////////////////////////////////////////////////
	//
	//  1. Create GameObjects for Each object in the state.
	//
	//  2. Add the uisprite to object for ui objects with correct sprite.
	//
	//  3. Set the transform
	//
	//	4. Add to the manager
	//


	GameObject* pauseTitle = new GameObject("Title", "PauseTitle");
	//	Add: Places an image on a normal sprite or ui sprite
	//
	//	1.	takes a new sprite/uisprite base on the object
	//	2.	takes the pointer to the gameobject it is being applied to
	//	3.	the file path to the ui image
	//	4.	sets the size of the object on screen
	//	5.	more input when hovered over

	pauseTitle->Add(new Graphics::UISprite(*pauseTitle, "data/img/spr/Character.png", Graphics::Size{ 128, 128 }));

	//	Find: Used to find the transform or the animation frame for an animation
	//
	//	1.	Takes the x coordinate
	//	2.	Takes the y coordinate
	pauseTitle->Find<Transform>()->setPosition(-200, 0);

	//	1.	Scales the X value 4 is a temp value
	pauseTitle->Find<Transform>()->setXScale(4);

	//	1.	Scales the Y value 4 is a temp value
	pauseTitle->Find<Transform>()->setYScale(4);

	//Get the GameObject Manager and add it to the obj stacks
	Manager().Add(*pauseTitle);

	//Create Button Objects based on steps from above
	// This could be hacked...

	GameObject* button = new GameObject("Button", "Resume");
	button->Add(new Graphics::Sprite(*button, "data/img/spr/menu_versus.png", Graphics::Size{ 128,128 }));
	button->Find<Transform>()->setPosition(-100, 0);
	button->Find<Transform>()->setXScale(5);
	button->Find<Transform>()->setYScale(5);
	Manager().Add(*button);

	button = new GameObject("Button", "Quit");
	button->Add(new Graphics::Sprite(*button, "data/img/spr/menu_option.png", Graphics::Size{ 128,128 }));
	button->Find<Transform>()->setPosition(-100, 0);
	button->Find<Transform>()->setXScale(5);
	button->Find<Transform>()->setYScale(5);
	Manager().Add(*button);

	Component* resume = Manager().Find("Resume").Find<Transform>();
	mMsg->Register(Messaging::ID::MenuDown, resume->GetKey(), MenuBehavior::InputMenuDown);



}
