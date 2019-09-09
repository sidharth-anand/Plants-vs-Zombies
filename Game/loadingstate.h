#pragma once

#include "state.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class LoadingState : public State
{
public:
						LoadingState(StateStack& stack, Context context);

	virtual void		draw();
	virtual bool		update(sf::Time dt);
	virtual bool		handleEvent(const sf::Event& event);


private:
	void				loadTextures();

private:
	sf::Sprite			mBackgroundSprite;
	sf::Sprite			mGrass;
	sf::Sprite			mGrassRoll;

	sf::Text			mText;
	bool				mShowText;

	sf::Time			mTextEffectTime;
	sf::Time			mElapsedTime;
};