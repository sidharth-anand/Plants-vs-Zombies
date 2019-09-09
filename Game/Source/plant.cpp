#include "plant.h"
#include "datatables.h"
#include "utitlty.h"
#include "commandqueue.h"
#include "resourceholder.h"
#include "soundnode.h"
#include "sun.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <cmath>
#include <iostream>

namespace
{
	const std::vector<PlantData> Table = initializePlantData();
}

Plant::Plant(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mFireCommand()
	, mFireCountdown(Table[type].fireInterval)
	, mFireInterval(Table[type].fireInterval)
	, mIsFiring(false)
	, mIsZombieWithinRange(false)
	, mPrice(Table[mType].price)
	, mRange(Table[mType].range)
	, mAnimation(textures.get(Table[type].animation))
{
	mAnimation.setFrameSize(Table[type].animationRect);
	mAnimation.setNumFrames(Table[type].animationFrames);
	mAnimation.setDuration(Table[type].animationDuration);
	mAnimation.setRepeating(true);

	centerOrigin(mSprite);
	centerOrigin(mAnimation);

	mFireCommand.category = Category::SceneLayer;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};

	if (type == Sunflower)
		mFireInterval = sf::seconds(randomInt(5, 14));

	updateTexts();
}

void Plant::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mAnimation, states);
}

void Plant::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateTexts();

	checkProjectileLaunch(dt, commands);
	mAnimation.update(dt);

	Entity::updateCurrent(dt, commands);
}

unsigned int Plant::getCategory() const
{
	return Category::Plant;
}

sf::FloatRect Plant::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Plant::isMarkedForRemoval() const
{
	return isDestroyed();
}

void Plant::remove()
{
	Entity::remove();
}

bool Plant::isAllied() const
{
	return true;
}

float Plant::getMaxSpeed() const
{
	return 0.f;
}

int Plant::getRange() const
{
	return mRange;
}

Plant::Type Plant::getType() const
{
	return mType;
}

void Plant::setZombieInRange(bool flag)
{
	mIsZombieWithinRange = flag;
}

int Plant::getPrice() const
{
	return mPrice;
}

void Plant::fire()
{
	if (mFireInterval != sf::Time::Zero)
		mIsFiring = true;
}

void Plant::playSound(CommandQueue & commands, Sounds::ID sound)
{
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = Category::Sounds;
	command.action = derivedAction<SoundNode>([sound, worldPosition] (SoundNode& node, sf::Time)
	{
		node.playSound(sound, worldPosition);
	});

	commands.push(command);
}

void Plant::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	if (mIsZombieWithinRange)
		fire();

	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		commands.push(mFireCommand);
		playSound(commands, Table[mType].fire);
		mFireCountdown += mFireInterval;
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		mFireCountdown -= dt;
		mIsFiring = false;
	}
}

void Plant::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	if (mType != Sunflower)
	{
		Projectile::Type type = Projectile::AlliedBullet;
		createProjectile(node, type, 0.f, -0.25f, textures);
	}
	else
	{
		createSun(node, -0.75f, -1.25f, textures);
	}
}

void Plant::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);

	projectile->setPosition(getWorldPosition() + offset);
	projectile->setVelocity(projectile->getMaxSpeed(), 0.f);
	node.attachChild(std::move(projectile));
}

void Plant::createSun(SceneNode& node, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Sun> sun(new Sun(textures, mSprite.getGlobalBounds().height));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);

	sun->setPosition(getWorldPosition() + offset);
	node.attachChild(std::move(sun));

}

void Plant::updateTexts()
{

}