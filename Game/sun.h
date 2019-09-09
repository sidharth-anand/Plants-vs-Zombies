#pragma once

#include "entity.h"
#include "command.h"
#include "resourceidentifiers.h"
#include "animation.h"

#include <SFML\Graphics\Sprite.hpp>

class Sun : public Entity
{
public:
							Sun(const TextureHolder& textures, const float distance);

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;
	virtual void			remove();
	virtual bool 			isMarkedForRemoval() const;

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);

private:
	sf::Sprite				mSprite;

	float					mDistance;
	float					mTravelledDistance;
	sf::Time				mFadeTime;
};