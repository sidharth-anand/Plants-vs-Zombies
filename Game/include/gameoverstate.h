#pragma once

#include "state.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameOverState : public State
{
public:
						GameOverState(StateStack& stack, Context context);
						~GameOverState();

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	sf::Sprite			mSprite;
	sf::Time			mElapsedTime;
};