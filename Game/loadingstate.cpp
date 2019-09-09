#include "loadingstate.h"
#include "utitlty.h"
#include "resourceidentifiers.h"

#include <SFML\Graphics\RenderWindow.hpp>

LoadingState::LoadingState(StateStack& stack, Context context)
	: State(stack, context)
	, mText()
	, mShowText(true)
	, mTextEffectTime(sf::Time::Zero)
	, mElapsedTime(sf::Time::Zero)
	, mBackgroundSprite()
	, mGrass()
	, mGrassRoll()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

	mGrass.setTexture(context.textures->get(Textures::LoadingGrass));
	mGrass.setPosition(237.f, 514.f);
	mGrass.setTextureRect(sf::IntRect(0, 0, 0, 0));

	mGrassRoll.setTexture(context.textures->get(Textures::LoadingGrassRoll));
	centerOrigin(mGrassRoll);
	mGrassRoll.setPosition(250.f, 503.f);

	mText.setFont(context.fonts->get(Fonts::BrianneTod));
	mText.setFillColor(sf::Color(243, 206, 0));
	mText.setString("Press any key to start");
	mText.setPosition(sf::Vector2f(283.f, 534.f));
	mText.setCharacterSize(25u);

	loadTextures();
}

void LoadingState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
	window.draw(mGrass);
	window.draw(mGrassRoll);

	if (mShowText && mElapsedTime > sf::seconds(6.8))
		window.draw(mText);
}

bool LoadingState::update(sf::Time dt)
{
	mTextEffectTime += dt;
	mElapsedTime += dt;

	if (mTextEffectTime >= sf::seconds(0.5f))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	if (mElapsedTime <= sf::seconds(6.28))
	{
		mGrass.setTextureRect(sf::IntRect(0, 0, mElapsedTime.asSeconds() * 50, 33));

		mGrassRoll.setScale(sf::Vector2f(1.25 * (1 - (mElapsedTime.asSeconds() / 6.28)), 1.25 * (1 - (mElapsedTime.asSeconds() / 6.28))));
		mGrassRoll.setPosition(48.197 * mElapsedTime.asSeconds() + 250.f, 5.75 * mElapsedTime.asSeconds() + 503);
		mGrassRoll.rotate(dt.asSeconds() * 50);
	}

	return true;
}

bool LoadingState::handleEvent(const sf::Event& event)
{
	if ((event.type == sf::Event::KeyReleased || event.type == sf::Event::MouseButtonReleased) && mElapsedTime > sf::seconds(6.8f))
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}

	return true;
}

void LoadingState::loadTextures()
{
	getContext().textures->load(Textures::Bullet, "Resources/Textures/projectilepea.png");
	getContext().textures->load(Textures::Zombie, "Resources/Textures/zombie.png");
	getContext().textures->load(Textures::Plant, "Resources/Textures/peashooter.png");
	getContext().textures->load(Textures::Map, "Resources/Textures/background1.png");
	getContext().textures->load(Textures::Buttons, "Resources/Textures/seedpacket.png");
	getContext().textures->load(Textures::PlantIcon, "Resources/Textures/icon1.png");
	getContext().textures->load(Textures::ZombieDeath, "Resources/Textures/zombie dying.png");
	getContext().textures->load(Textures::ZombieNormal, "Resources/Textures/zombie walking1.png");
	getContext().textures->load(Textures::Sun, "Resources/Textures/sun.png");
	getContext().textures->load(Textures::PeaHit, "Resources/Textures/peasplatter.png");
	getContext().textures->load(Textures::Words, "Resources/Textures/readysetplant.png");
	getContext().textures->load(Textures::Eating, "Resources/Textures/zombie eating.png");
	getContext().textures->load(Textures::ProgressBar, "Resources/Textures/levelprogressbar.png");
	getContext().textures->load(Textures::ProgressMeter, "Resources/Textures/levelprogressmeter.png");
	getContext().textures->load(Textures::ProgressParts, "Resources/Textures/levelprogressparts.png");
	getContext().textures->load(Textures::HugeWave, "Resources/Textures/hugewave.png");
	getContext().textures->load(Textures::MainMenu, "Resources/Textures/mainmenubg.png");
	getContext().textures->load(Textures::AdventureButton, "Resources/Textures/adventurebutton.png");
	getContext().textures->load(Textures::OptionsButton, "Resources/Textures/optionsbutton.png");
	getContext().textures->load(Textures::QuitButton, "Resources/Textures/quitbutton.png");
	getContext().textures->load(Textures::OptionsBG, "Resources/Textures/settingsbg.png");
	getContext().textures->load(Textures::OptionsBack, "Resources/Textures/optionsok.png");
	getContext().textures->load(Textures::SliderBar, "Resources/Textures/bar.png");
	getContext().textures->load(Textures::SliderKnob, "Resources/Textures/knob.png");
	getContext().textures->load(Textures::Pause, "Resources/Textures/menubutton.png");
	getContext().textures->load(Textures::Back, "Resources/Textures/back.png");
	getContext().textures->load(Textures::Restart, "Resources/Textures/mainmenu.png");
	getContext().textures->load(Textures::Win, "Resources/Textures/win.png");
	getContext().textures->load(Textures::Lose, "Resources/Textures/lose.png");
	getContext().textures->load(Textures::Sunflower, "Resources/Textures/sunflower.png");
	getContext().textures->load(Textures::SunflowerIcon, "Resources/Textures/sunflowericon.png");
	getContext().textures->load(Textures::PeashooterAnimation, "Resources/Textures/peashooteranimation.png");
	getContext().textures->load(Textures::SunflowerAnimation, "Resources/Textures/sunfloweranimation.png");
	getContext().textures->load(Textures::PlayerNameButton, "Resources/Textures/namebutton.png");
	getContext().textures->load(Textures::ScoreButton, "Resources/Textures/scorebutton.png");
	getContext().textures->load(Textures::ScoreScreen, "Resources/Textures/scorescreen.png");
}