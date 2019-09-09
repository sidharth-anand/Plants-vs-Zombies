#include "settingsstate.h"
#include "button.h"
#include "utitlty.h"
#include "resourceholder.h"
#include "slider.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\View.hpp>

#include <iostream>

SettingsState::SettingsState(StateStack &stack, Context context)
	:State(stack, context)
	, mGUIContainer()
{
	sf::Texture& texture = context.textures->get(Textures::OptionsBG);
	mBackgroundSprite.setTexture(texture);
	centerOrigin(mBackgroundSprite);
	mBackgroundSprite.setPosition(context.window->getSize().x / 2, context.window->getSize().y / 2);

	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures, *context.sounds, GUI::Button::OptionsBack);
	backButton->setPosition(400.f, 492.f);
	backButton->setText("");
	backButton->setCallback([this]()
	{
		requestStackPop();
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
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	mGUIContainer.setMousePosition(sf::Mouse::getPosition(*getContext().window));
	mGUIContainer.handleEvent(event);
	return false;
}