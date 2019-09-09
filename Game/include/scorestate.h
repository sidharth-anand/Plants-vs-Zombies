#pragma once

#include "state.h"
#include "container.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

class ScoreState : public State
{
public:
						ScoreState(StateStack& stack, Context context);
						
	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);

private:
	sf::Text			mText;
	sf::Sprite			mBackground;
	GUI::Container		mGUIContainer;

	std::vector<std::pair<std::string, int>>	mScores;
};