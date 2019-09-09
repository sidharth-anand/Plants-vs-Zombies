#include "menustate.h"
#include "button.h"
#include "utitlty.h"
#include "resourceholder.h"
#include "textbox.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(Textures::MainMenu);
	mBackgroundSprite.setTexture(texture);

	auto playButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures, *context.sounds, GUI::Button::AdventureButton);
	playButton->setPosition(570.f, 135.5);
	playButton->setText("");
	playButton->setCallback([this]()
	{
		requestStackPop();
		requestStackPush(States::Game);
	});

	auto optionsButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures, *context.sounds, GUI::Button::Options);
	optionsButton->setPosition(602.5, 496.f);
	optionsButton->setText("");
	optionsButton->setCallback([this]()
	{
		requestStackPush(States::Settings);
	});

	auto quitButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures, *context.sounds, GUI::Button::Quit);
	quitButton->setPosition(742.5, 518.5);
	quitButton->setText("");
	quitButton->setCallback([this]()
	{
		requestStateClear();
	});

	*context.playerName = "Player";

	auto nameBox = std::make_shared<GUI::TextBox>(220, *context.playerName, *context.fonts);
	nameBox->setPosition(150.f, 107.5f);
	nameBox->setCallback([this, context, nameBox]()
	{
			*context.playerName = nameBox->getText();
	});

	auto nameButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures, *context.sounds, GUI::Button::Player);
	nameButton->setPosition(145.5f, 165.5f);
	nameButton->setText("");
	nameButton->setCallback([this, context, nameBox]()
	{
			*context.playerName = nameBox->getText();
	});

	auto scoreButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures, *context.sounds, GUI::Button::Score);
	scoreButton->setPosition(550.5, 230.5);
	scoreButton->setText("");
	scoreButton->setCallback([this]()
	{
			requestStackPush(States::Score);
	});

	mGUIContainer.pack(playButton);
	mGUIContainer.pack(optionsButton);
	mGUIContainer.pack(quitButton);
	mGUIContainer.pack(nameButton);
	mGUIContainer.pack(nameBox);
	mGUIContainer.pack(scoreButton);
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	float scale = (float)getContext().window->getSize().x / 800;
	sf::Vector2f pos((float)(sf::Mouse::getPosition(*getContext().window).x / scale), (float)sf::Mouse::getPosition(*getContext().window).y / scale);
	mGUIContainer.setMousePosition(sf::Vector2i(int(pos.x), int(pos.y)));
	mGUIContainer.handleEvent(event);
	return false;
}