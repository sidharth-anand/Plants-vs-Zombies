#pragma once

#include "entity.h"
#include "resourceidentifiers.h"
#include "animation.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

class Projectile : public Entity
{
public:
	enum Type
	{
		AlliedBullet,
		TypeCount
	};


public:
							Projectile(Type type, const TextureHolder& textures);

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;
	float					getMaxSpeed() const;
	int						getDamage() const;

	virtual bool			isMarkedForRemoval() const;
	virtual void			remove();

private:
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	Type					mType;
	sf::Sprite				mSprite;
	Animation				mHit;
	bool					mShowHit;
	bool					mPlayedHitSound;
};
