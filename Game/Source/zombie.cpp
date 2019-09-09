#include "zombie.h"
#include "datatables.h"
#include "utitlty.h"
#include "commandqueue.h"
#include "resourceholder.h"
#include "soundnode.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <cmath>
#include <iostream>

namespace
{
	const std::vector<ZombieData> Table = initializeZombieData();
}

Zombie::Zombie(Type type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mDeath(textures.get(Textures::ZombieDeath))
	, mMovement(textures.get(Textures::ZombieNormal))
	, mEating(textures.get(Textures::Eating))
	, mShowDeath(true)
	, mIsEating(false)
	, mPlayedFallSound(false)
{
	mDeath.setFrameSize(sf::Vector2i(359, 165));
	mDeath.setNumFrames(25);
	mDeath.setDuration(sf::seconds(1.5f));

	mMovement.setFrameSize(sf::Vector2i(126, 150));
	mMovement.setNumFrames(25);
	mMovement.setDuration(sf::seconds(1.5f));
	mMovement.setRepeating(true);

	mEating.setFrameSize(sf::Vector2i(101, 149));
	mEating.setNumFrames(25);
	mEating.setDuration(sf::seconds(1.5f));
	mEating.setRepeating(true);

	setVelocity(-getMaxSpeed(), 0.f);

	centerOrigin(mSprite);
	centerOrigin(mDeath);
	centerOrigin(mMovement);
	centerOrigin(mEating);

}

void Zombie::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowDeath)
		target.draw(mDeath, states);
	else if (mIsEating)
		target.draw(mEating, states);
	else
		target.draw(mMovement, states);
}

void Zombie::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isDestroyed())
	{
		mDeath.update(dt);

		if (!mPlayedFallSound)
		{
			sf::Vector2f worldPosition = getWorldPosition();

			auto sound = Sounds::ZombieFalling;

			Command command;
			command.category = Category::Sounds;
			command.action = derivedAction<SoundNode>([sound, worldPosition](SoundNode& node, sf::Time)
			{
				node.playSound(sound, worldPosition);
			});

			commands.push(command);

			mPlayedFallSound = true;
		}

		return;
	}

	if (mIsEating)
		mEating.update(dt);

	mMovement.update(dt);

	mIsEating = false;

	Entity::updateCurrent(dt, commands);
	setVelocity(-getMaxSpeed(), 0.f);
}

unsigned int Zombie::getCategory() const
{
	return Category::Zombie;
}

sf::FloatRect Zombie::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Zombie::isMarkedForRemoval() const
{
	return isDestroyed() && (mDeath.isFinished() || !mShowDeath);
}

void Zombie::remove()
{
	Entity::remove();
	mShowDeath = false;
}

float Zombie::getMaxSpeed() const
{
	return Table[mType].speed;
}

void Zombie::setIsEating(bool flag)
{
	mIsEating = true;
}