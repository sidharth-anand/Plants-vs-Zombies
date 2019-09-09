#include "gamestate.h"
#include "button.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

GameState::GameState(StateStack& stack, Context context)
	: State(stack, context)
	, mWorld(*context.window, *context.textures, *context.fonts, *context.sounds)
	, mGUIContainer()
	, mPlayerStatus(*(context.hasPlayerWon))
	, mPushTime(sf::seconds(0.f))
{
	auto menuButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures, *context.sounds, GUI::Button::PauseMenu);
	menuButton->setPosition(733.5f, 15.f);
	menuButton->setText("");
	menuButton->setCallback([this, context]()
	{
		requestStackPush(States::Pause);
		context.sounds->play(Sounds::Pause);
	});

	mGUIContainer.pack(menuButton);

	context.music->play(Music::Grasswalk);
}

void GameState::draw()
{
	mWorld.draw();

	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mGUIContainer);
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);

	if (mWorld.hasPlayerFinished())
	{
		mPlayerStatus = true;
		mPushTime += dt;
		*getContext().playaerScore = 60000 - mWorld.getElapsedTime();
	}
	else if (mWorld.hasZombiesReachedEnd())
	{
		mPlayerStatus = false;
		mPushTime += dt;
	}

	if (mPushTime > sf::seconds(2.f))
		requestStackPush(States::GameOver);

	CommandQueue& commands = mWorld.getCommandQueue();

	return true;
}

bool GameState::handleEvent(const sf::Event& event)
{
	CommandQueue& commands = mWorld.getCommandQueue();

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		requestStackPush(States::Pause);
		getContext().sounds->play(Sounds::Pause);
	}
	else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
	{
		mWorld.screenshot();
	}

	mWorld.setMousePosition(sf::Mouse::getPosition(*getContext().window));
	mWorld.handleEvent(event);

	mGUIContainer.setMousePosition(sf::Mouse::getPosition(*getContext().window));
	mGUIContainer.handleEvent(event);

	return true;
}