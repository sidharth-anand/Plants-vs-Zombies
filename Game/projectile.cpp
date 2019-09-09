#include "projectile.h"
#include "datatables.h"
#include "utitlty.h"
#include "resourceholder.h"
#include "command.h"
#include "soundnode.h"
#include "commandqueue.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>
#include <iostream>

namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures)
	: Entity(1)
	, mType(type)
	, mSprite(textures.get(Table[type].texture), Table[type].textureRect)
	, mHit(textures.get(Table[mType].splatter))
	, mShowHit(true)
	, mPlayedHitSound(false)
{
	mHit.setFrameSize(sf::Vector2i(22, 24));
	mHit.setNumFrames(4);
	mHit.setDuration(sf::seconds(0.1));

	centerOrigin(mSprite);
	centerOrigin(mHit);
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isDestroyed())
	{
		mHit.update(dt);

		if (!mPlayedHitSound)
		{
			sf::Vector2f worldPosition = getWorldPosition();

			auto sound = Table[mType].hit;

			Command command;
			command.category = Category::Sounds;
			command.action = derivedAction<SoundNode>([sound, worldPosition](SoundNode& node, sf::Time)
			{
				node.playSound(sound, worldPosition);
			});

			commands.push(command);

			mPlayedHitSound = true;
		}

		return;
	}

	Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowHit)
		target.draw(mHit, states);
	else
		target.draw(mSprite, states);
}

unsigned int Projectile::getCategory() const
{
	return Category::Projectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
	return Table[mType].speed;
}

int Projectile::getDamage() const
{
	return Table[mType].damage;
}


bool Projectile::isMarkedForRemoval() const
{
	return isDestroyed() && (mHit.isFinished() || !mShowHit);
}

void Projectile::remove()
{
	Entity::remove();
	mShowHit = false;
}