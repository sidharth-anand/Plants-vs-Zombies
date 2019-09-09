#pragma once

#include "state.h"
#include "world.h"
#include "container.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State
{
public:
						GameState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	World				mWorld;
	GUI::Container		mGUIContainer;

	bool&				mPlayerStatus;
	sf::Time			mPushTime;
};