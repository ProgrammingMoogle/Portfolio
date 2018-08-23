#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <Util/Util.hpp>
#include <string>
#include <GameObject/GameObjectManager.h>
#include <GameObject/ObjectFactory.h>
#include <Messaging/Messaging.hpp>
#include <Transform/Transform.hpp>	
#include "GameState.h"
#include <Behaviors/MenuBehaviors.hpp>

class PauseState : public GameState
{
public:
	PauseState() = default;
	~PauseState() = default;

	void Initialize();
	void Update();
	void Shutdown();
private:
	Messaging::MessageHandler* mMsg;
};

#endif //PAUSESTATE

