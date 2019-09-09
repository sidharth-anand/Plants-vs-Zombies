#include "soundplayer.h"

#include <SFML/Audio/Listener.hpp>

#include <cmath>


namespace
{
	const float ListenerZ = 300.f;
	const float Attenuation = 8.f;
	const float MinDistance2D = 200.f;
	const float MinDistance3D = std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);
}

SoundPlayer::SoundPlayer()
	: mSoundBuffers()
	, mSounds()
	, mVolume(100.f)
{
	mSoundBuffers.load(Sounds::ButtonHover, "Resources/Sounds/buttonhover.ogg");
	mSoundBuffers.load(Sounds::ButtonClickSmall, "Resources/Sounds/buttonclicksmall.ogg");
	mSoundBuffers.load(Sounds::ButtonClick, "Resources/Sounds/buttonclick.ogg");
	mSoundBuffers.load(Sounds::ButtonClickPlant, "Resources/Sounds/plantclick.ogg");
	mSoundBuffers.load(Sounds::Plant, "Resources/Sounds/plant.ogg");
	mSoundBuffers.load(Sounds::SunClick, "Resources/Sounds/sun.ogg");
	mSoundBuffers.load(Sounds::Pause, "Resources/Sounds/pause.ogg");
	mSoundBuffers.load(Sounds::ReadySetPlant, "Resources/Sounds/readysetplant.ogg");
	mSoundBuffers.load(Sounds::PeashooterFire, "Resources/Sounds/peafire.ogg");
	mSoundBuffers.load(Sounds::PeaSplat, "Resources/Sounds/peahit.ogg");
	mSoundBuffers.load(Sounds::HugeWave, "Resources/Sounds/hugewave.ogg");
	mSoundBuffers.load(Sounds::Eating, "Resources/Sounds/chomp.ogg");
	mSoundBuffers.load(Sounds::ZombieFalling, "Resources/Sounds/zombiefalling.ogg");
	mSoundBuffers.load(Sounds::None, "Resources/Sounds/none.ogg");

	sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(Sounds::ID effect)
{
	play(effect, getListenerPosition());
}

void SoundPlayer::play(Sounds::ID effect, sf::Vector2f position)
{
	mSounds.push_back(sf::Sound());
	sf::Sound& sound = mSounds.back();

	sound.setBuffer(mSoundBuffers.get(effect));
	sound.setPosition(position.x, -position.y, 0.f);
	sound.setAttenuation(Attenuation);
	sound.setMinDistance(MinDistance3D);
	sound.setVolume(mVolume);

	sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
	mSounds.remove_if([](const sf::Sound& s)
	{
		return s.getStatus() == sf::Sound::Stopped;
	});
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
	sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
	sf::Vector3f position = sf::Listener::getPosition();
	return sf::Vector2f(position.x, -position.y);
}

void SoundPlayer::setVolume(int volume)
{
	mVolume = volume;
}

int SoundPlayer::getVolume() const
{
	return (int)mVolume;
}