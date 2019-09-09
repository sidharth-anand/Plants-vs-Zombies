#include "pausestate.h"
#include "button.h"
#include "utitlty.h"
#include "resourceholder.h"
#include "slider.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\View.hpp>

#include <iostream>

PauseState::PauseState(StateStack &stack, Context context)
	:State(stack, context)
	, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(Textures::OptionsBG);
	mBackgroundSprite.setTexture(texture);
	centerOrigin(mBackgroundSprite);
	mBackgroundSprite.setPosition(context.window->getSize().x / 2, context.window->getSize().y / 2);

	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures, *context.sounds, GUI::Button::Back);
	backButton->setPosition(400.f, 492.f);
	backButton->setText("");
	backButton->setCallback([this]()
	{
		requestStackPop();
	});

	auto restart = std::make_shared<GUI::Button>(*context.fonts, *context.textures, *context.sounds, GUI::Button::MainMenu);
	restart->setPosition(406.f, 344.5);
	restart->setTextColor(sf::Color(0, 255, 0, 150), GUI::Button::Normal);
	restart->setTextColor(sf::Color(0, 255, 0, 250), GUI::Button::Selected);
	restart->setText("Restart");
	restart->setFont(context.fonts->get(Fonts::Dwarven));
	restart->setCallback([this]()
	{
		requestStackPop();
		requestStackPop();
		requestStackPush(States::Game);
	});

	auto menu = std::make_shared<GUI::Button>(*context.fonts, *context.textures, *context.sounds, GUI::Button::MainMenu);
	menu->setPosition(406.f, 394.5);
	menu->setTextColor(sf::Color(0, 255, 0, 150), GUI::Button::Normal);
	menu->setTextColor(sf::Color(0, 255, 0, 250), GUI::Button::Selected);
	menu->setText("Main Menu");
	menu->setFont(context.fonts->get(Fonts::Dwarven));
	menu->setCharacterSize(20u);
	menu->setCallback([this, context]()
	{
		requestStateClear();
		requestStackPush(States::Menu);
		context.music->play(Music::Menu);
	});

	auto music = std::make_shared<GUI::Slider>(*context.textures, GUI::Slider::SettingsMenu);
	music->setPosition(453.5, 212.5);
	music->setValue(context.music->getVolume());
	music->setCallback([this, music, context]()
	{
		context.music->setVolume(music->getValue());
	});

	auto sfx = std::make_shared<GUI::Slider>(*context.textures, GUI::Slider::SettingsMenu);
	sfx->setPosition(453.5, 253.5);
	sfx->setValue(context.sounds->getVolume());
	sfx->setQuantum(10);
	sfx->setCallback([this, sfx, context]()
	{
		context.sounds->setVolume(sfx->getValue());
	});

	mGUIContainer.pack(backButton);
	mGUIContainer.pack(music);
	mGUIContainer.pack(sfx);
	mGUIContainer.pack(restart);
	mGUIContainer.pack(menu);

	context.music->setPaused(true);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool PauseState::update(sf::Time)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	mGUIContainer.setMousePosition(sf::Mouse::getPosition(*getContext().window));
	mGUIContainer.handleEvent(event);
	return false;
}

PauseState::~PauseState()
{
	getContext().music->setPaused(false);
}