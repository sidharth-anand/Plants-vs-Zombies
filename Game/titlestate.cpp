#include "titlestate.h"
#include "utitlty.h"
#include "resourceholder.h"

#include <SFML/Graphics/RenderWindow.hpp>


TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mElapsedTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::Logo));

	context.music->play(Music::Menu);
}

void TitleState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
}

bool TitleState::update(sf::Time dt)
{
	mElapsedTime += dt;

	float alpha;
	alpha = 255 * std::sinf(4 / 3.14 * mElapsedTime.asSeconds());
	alpha > 0 ? alpha = alpha : alpha = 0;

	mBackgroundSprite.setColor(sf::Color(255, 255, 255, alpha));

	if(mElapsedTime > sf::seconds(3.f))
	{
		requestStackPop();
		requestStackPush(States::Loading);
	}

	return true;
}

bool TitleState::handleEvent(const sf::Event& event)
{

	return true;
}