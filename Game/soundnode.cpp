#include "soundnode.h"
#include "soundplayer.h"


SoundNode::SoundNode(SoundPlayer& player)
	: SceneNode()
	, mSounds(player)
{
}

void SoundNode::playSound(Sounds::ID sound, sf::Vector2f position)
{
	mSounds.play(sound, position);
}

unsigned int SoundNode::getCategory() const
{
	return Category::Sounds;
}