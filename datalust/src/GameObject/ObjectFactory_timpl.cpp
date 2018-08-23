/******************************************************************************/
/*!
\file    ObjectFactory_timple.cpp
\author  Zayd Gaudet
\par     zayd.gaudet\@digipen.edu
\par     10/25/2017

\brief
	template implementation for the ObjectFactory class

All content © 2017 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "ObjectFactory.h"

// colors and blend mode and depth
template<typename T>
void ObjectFactory::WriteFN_A_BA_C_BM_D(GameObject const& gameObject, Json::Value Root, unsigned int NumObj, const char* name)
{
	//TODO write the filename into the json file once the functionality exists
	//object.Find<Graphics::BackgroundImage>()->loadFromFile(obj["BackgroundImage"]["FileName"].asCString());

	// write the alpha to the json file
	float Alpha;
	gameObject.Find<T>()->getAlpha(&Alpha);
	Root[name]["Alpha"] = Alpha;

	// write the various colors to the json file
	Graphics::Color color;
	gameObject.Find<T>()->getBlendColor(&color);
	// store the red
	Root["Object"][NumObj - 1][name]["Red"] = color.r;
	// store the green
	Root["Object"][NumObj - 1][name]["Green"] = color.g;
	// store the blue
	Root["Object"][NumObj - 1][name]["Blue"] = color.b;
	// store the Blend alpha
	Root["Object"][NumObj - 1][name]["BlendAlpha"] = color.a;

	// write the blend mode to the json file, we are storing it as an int since it is an enum
	Graphics::BlendMode blendmode;
	gameObject.Find<T>()->getBlendMode(&blendmode);
	Root["Object"][NumObj - 1][name]["BlendMode"] = static_cast<int>(blendmode);

	// write the depth of the BackgroundImage to the json file
	float depth;
	gameObject.Find<T>()->getDepth(&depth);
	Root["Object"][NumObj - 1][name]["Depth"] = depth;
}

template <typename T>
void ObjectFactory::WriteS_F(GameObject const& gameObject, Json::Value Root, unsigned int NumObj, const char* name)
{
	// write the size of the UISprite to the json file
	Graphics::Size size;
	gameObject.Find<T>()->getSize(&size);
	Root["Object"][NumObj - 1][name]["SpriteWidth"] = size.x;
	Root["Object"][NumObj - 1][name]["SpriteHeight"] = size.y;


	// write the frame of the UIsprite to the json file
	size_t frame;
	gameObject.Find<T>()->getFrame(&frame);
	Root["Object"][NumObj - 1][name]["Frame"] = frame;
}

template <typename T>
void ObjectFactory::ReadGraphicsComponents(GameObject& object, Json::Value obj, const char* name)
{
	// read the name of the file of the sprite sheet and store it in the component
	if (obj[name].isMember("FileName"))
		object.Find<T>()->loadFromFile(obj[name]["FileName"].asCString());

	// read in the Alpha value and store it in the component
	if (obj[name].isMember("Alpha"))
		object.Find<T>()->setAlpha(obj[name]["Alpha"].asFloat());

	// read in the BlendAlpha and store it for the component
	if (obj[name].isMember("BlendAlpha"))
		object.Find<T>()->setBlendAlpha(obj[name]["BlendAlpha"].asFloat());

	// read in the blend color for the component and store it
	if (obj[name].isMember("Red") && obj[name].isMember("Green")
		&& obj[name].isMember("Blue") && obj[name].isMember("BlendAlpha"))
		object.Find<T>()->setBlendColor({ obj[name]["Red"].asFloat(),
											obj[name]["Green"].asFloat(),
											obj[name]["Blue"].asFloat(),
											obj[name]["BlendAlpha"].asFloat() });

	// read in the blendmode from the json file and stor it in the component
	if (obj[name].isMember("BlendMode"))
		object.Find<T>()->setBlendMode(static_cast<Graphics::BlendMode>(obj[name]["BlendMode"].asInt()));

	// read in the Depth from the json file and store in component
	if (obj[name].isMember("Depth"))
		object.Find<T>()->setDepth(obj[name]["Depth"].asFloat());
}