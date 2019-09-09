#pragma once

#include "entity.h"
#include "command.h"
#include "resourceidentifiers.h"
#include "projectile.h"
#include "textnode.h"
#include "animation.h"
#include "utitlty.h"

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

class Zombie : public Entity
{
public:
	enum Type
	{
		Basic,
		TypeCount,
	};

public:
							Zombie(Type type, const TextureHolder& textures, const FontHolder& fonts);
	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;
	virtual void			remove();
	virtual bool 			isMarkedForRemoval() const;
	float					getMaxSpeed() const;
	void					setIsEating(bool flag);

private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
	void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);


private:
	Type					mType;
	sf::Sprite				mSprite;
	Animation				mMovement;
	Animation				mDeath;
	Animation				mEating;
	bool 					mShowDeath;
	bool					mIsEating;
	bool					mPlayedFallSound;
};