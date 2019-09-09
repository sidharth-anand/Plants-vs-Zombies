#pragma once

#include "resourceholder.h"
#include "resourceidentifiers.h"
#include "statestack.h"
#include "musicplayer.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <string.h>

class Application
{
public:
							Application();
	void					run();


private:
	void					processInput();
	void					update(sf::Time dt);
	void					render();

	void					updateStatistics(sf::Time dt);
	void					registerStates();


private:
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	TextureHolder			mTextures;
	FontHolder				mFonts;
	MusicPlayer				mMusic;
	SoundPlayer				mSounds;
	std::string				mPlayerName;
	int						mScore;

	StateStack				mStateStack;

	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;
};