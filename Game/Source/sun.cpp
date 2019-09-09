#include "sun.h"
#include "resourceholder.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

Sun::Sun(const TextureHolder& textures, const float distance)
	: Entity(1)
	, mSprite(textures.get(Textures::Sun))
	, mDistance(distance)
	, mTravelledDistance(0.f)
	, mFadeTime(sf::seconds(0))
{
	setVelocity(0.f, 35.f);
}

unsigned int Sun::getCategory() const
{
	return Category::Sun;
}

sf::FloatRect Sun::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Sun::isMarkedForRemoval() const
{
	return isDestroyed();
}

void Sun::remove()
{
	Entity::remove();
}

void Sun::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Sun::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	mTravelledDistance += dt.asSeconds() * getVelocity().y;
	if (mTravelledDistance >= mDistance)
	{
		setVelocity(0.f, 0.f);
		mFadeTime += dt;
	}

	if (mFadeTime > sf::seconds(5))
		remove();

	if (mFadeTime > sf::seconds(2.5f))
		mSprite.setColor(sf::Color(255, 255, 255, (255 - ((mFadeTime.asMilliseconds() - 2500) / 10))));

	Entity::updateCurrent(dt, commands);
}