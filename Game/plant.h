#pragma once

#include "entity.h"
#include "command.h"
#include "resourceidentifiers.h"
#include "projectile.h"
#include "textnode.h"
#include "animation.h"

#include <SFML/Graphics/Sprite.hpp>


class Plant : public Entity
{
public:
	enum Type
	{
		Basic,
		Sunflower,
		TypeCount,
	};


public:
							Plant(Type type, const TextureHolder& textures, const FontHolder& fonts);

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;
	virtual void			remove();
	virtual bool 			isMarkedForRemoval() const;
	bool					isAllied() const;
	float					getMaxSpeed() const;
	int						getRange() const;
	Plant::Type				getType() const;
	void					setZombieInRange(bool flag);
	int						getPrice() const;

	void 					fire();

	void					playSound(CommandQueue& commands, Sounds::ID sound);


private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
	void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	void					createBullets(SceneNode& node, const TextureHolder& textures) const;
	void					createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
	void					createSun(SceneNode& node, float xOffset, float yOffset, const TextureHolder& textures) const;

	void					updateTexts();


private:
	Type					mType;
	sf::Sprite				mSprite;
	Animation				mAnimation;
	Command 				mFireCommand;
	sf::Time				mFireCountdown;
	sf::Time				mFireInterval;
	bool 					mIsFiring;
	bool					mIsZombieWithinRange;
	int						mPrice;
	float					mRange;
};