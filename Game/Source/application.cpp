#include "application.h"
#include "utitlty.h"
#include "state.h"
#include "stateidentifiers.h"
#include "gamestate.h"
#include "menustate.h"
#include "settingsstate.h"
#include "pausestate.h"
#include "gameoverstate.h"
#include "loadingstate.h"
#include "titlestate.h"
#include "scorestate.h"

#include <iostream>

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(800, 600), "Plants vs Zombies", sf::Style::Resize | sf::Style::Close)
	, mTextures()
	, mFonts()
	, mMusic()
	, mSounds()
	, mPlayerName("Player")
	, mScore(0)
	, mStateStack(State::Context(mWindow, mTextures, mFonts, mMusic, mSounds, mPlayerName, mScore))
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
{
	mWindow.setKeyRepeatEnabled(false);

	mFonts.load(Fonts::Main, "Resources/Fonts/Sansation.ttf");
	mFonts.load(Fonts::CSMS, "Resources/Fonts/Comic Sans ms.ttf");
	mFonts.load(Fonts::Dwarven, "Resources/Fonts/DWARVESC.ttf");
	mFonts.load(Fonts::BrianneTod, "Resources/Fonts/BrianneTod.ttf");


	mTextures.load(Textures::TitleScreen, "Resources/Textures/titlescreen.png");
	mTextures.load(Textures::LoadingGrass, "Resources/Textures/loadinggrass.png");
	mTextures.load(Textures::LoadingGrassRoll, "Resources/Textures/grassroll.png");
	mTextures.load(Textures::Logo, "Resources/Textures/logo.png");

	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

	mMusic.setVolume(75.f);

	registerStates();
	mStateStack.pushState(States::Title);
}

void Application::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (mStateStack.isEmpty())
				mWindow.close();
		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
		else if (event.type == sf::Event::Resized)
		{
			int x = ((mWindow.getSize().x + 200) / 400) * 400;
			int y = x * 3;
			y /= 4;
			mWindow.setSize(sf::Vector2u(x, y));
		}
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<SettingsState>(States::Settings);
	mStateStack.registerState<PauseState>(States::Pause);
	mStateStack.registerState<GameOverState>(States::GameOver);
	mStateStack.registerState<LoadingState>(States::Loading);
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<ScoreState>(States::Score);
}