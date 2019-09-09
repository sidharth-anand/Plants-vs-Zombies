#pragma once

#include "scenenode.h"
#include "resourceidentifiers.h"

class SoundPlayer;

class SoundNode : public SceneNode
{
public:
	explicit				SoundNode(SoundPlayer& player);
	void					playSound(Sounds::ID sound, sf::Vector2f position);

	virtual unsigned int	getCategory() const;


private:
	SoundPlayer&			mSounds;
};