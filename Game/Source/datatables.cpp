#include "datatables.h"
#include "plant.h"
#include "zombie.h"
#include "projectile.h"
#include "button.h"
#include "slider.h"

using namespace std::placeholders;

std::vector<PlantData> initializePlantData()
{
	std::vector<PlantData> data(Plant::TypeCount);

	data[Plant::Basic].hitpoints = 300;
	data[Plant::Basic].texture = Textures::Plant;
	data[Plant::Basic].textureRect = sf::IntRect(0, 0, 63, 71);
	data[Plant::Basic].fireInterval = sf::seconds(1.75f);
	data[Plant::Basic].price = 100;
	data[Plant::Basic].range = 600;
	data[Plant::Basic].fire = Sounds::PeashooterFire;
	data[Plant::Basic].animation = Textures::PeashooterAnimation;
	data[Plant::Basic].animationRect = sf::Vector2i(91, 86);
	data[Plant::Basic].animationDuration = sf::seconds(1.75f);
	data[Plant::Basic].animationFrames = 25;

	data[Plant::Sunflower].hitpoints = 300;
	data[Plant::Sunflower].texture = Textures::Sunflower;
	data[Plant::Sunflower].textureRect = sf::IntRect(0, 0, 76, 74);
	data[Plant::Sunflower].fireInterval = sf::seconds(15.575);
	data[Plant::Sunflower].price = 50;
	data[Plant::Sunflower].range = 600;
	data[Plant::Sunflower].fire = Sounds::None;
	data[Plant::Sunflower].animation = Textures::SunflowerAnimation;
	data[Plant::Sunflower].animationRect = sf::Vector2i(84, 78);
	data[Plant::Sunflower].animationDuration = sf::seconds(1.75f);
	data[Plant::Sunflower].animationFrames = 25;

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(Projectile::TypeCount);

	data[Projectile::AlliedBullet].damage = 5.f;
	data[Projectile::AlliedBullet].speed = 300.f;
	data[Projectile::AlliedBullet].texture = Textures::Bullet;
	data[Projectile::AlliedBullet].textureRect = sf::IntRect(0, 0, 24, 24);
	data[Projectile::AlliedBullet].splatter = Textures::PeaHit;
	data[Projectile::AlliedBullet].hit = Sounds::PeaSplat;

	return data;
}

std::vector<ZombieData> initializeZombieData()
{
	std::vector<ZombieData> data(Zombie::TypeCount);

	data[Zombie::Basic].hitpoints = 25;
	data[Zombie::Basic].speed = 15.f;
	data[Zombie::Basic].texture = Textures::Zombie;
	data[Zombie::Basic].textureRect = sf::IntRect(0, 0, 65, 108);

	return data;
}

std::vector<ButtonData> initializeButtonData()
{
	std::vector<ButtonData> data(GUI::Button::TypeCount);

	data[GUI::Button::PlantButton].texture = Textures::Buttons;
	data[GUI::Button::PlantButton].textureRect = sf::IntRect(0, 0, 52, 74);
	data[GUI::Button::PlantButton].hover = Sounds::None;
	data[GUI::Button::PlantButton].click = Sounds::ButtonClickPlant;

	data[GUI::Button::AdventureButton].texture = Textures::AdventureButton;
	data[GUI::Button::AdventureButton].textureRect = sf::IntRect(0, 0, 330, 143);
	data[GUI::Button::AdventureButton].hover = Sounds::ButtonHover;
	data[GUI::Button::AdventureButton].click = Sounds::ButtonClick;

	data[GUI::Button::Options].texture = Textures::OptionsButton;
	data[GUI::Button::Options].textureRect = sf::IntRect(0, 0, 79, 28);
	data[GUI::Button::Options].hover = Sounds::ButtonHover;
	data[GUI::Button::Options].click = Sounds::ButtonClickSmall;

	data[GUI::Button::Quit].texture = Textures::QuitButton;
	data[GUI::Button::Quit].textureRect = sf::IntRect(0, 0, 45, 25);
	data[GUI::Button::Quit].hover = Sounds::ButtonHover;
	data[GUI::Button::Quit].click = Sounds::ButtonClickSmall;

	data[GUI::Button::OptionsBack].texture = Textures::OptionsBack;
	data[GUI::Button::OptionsBack].textureRect = sf::IntRect(0, 0, 76, 42);
	data[GUI::Button::OptionsBack].hover = Sounds::ButtonHover;
	data[GUI::Button::OptionsBack].click = Sounds::ButtonClick;

	data[GUI::Button::PauseMenu].texture = Textures::Pause;
	data[GUI::Button::PauseMenu].textureRect = sf::IntRect(0, 0, 113, 30);
	data[GUI::Button::PauseMenu].hover = Sounds::ButtonHover;
	data[GUI::Button::PauseMenu].click = Sounds::ButtonClickSmall;

	data[GUI::Button::Back].texture = Textures::Back;
	data[GUI::Button::Back].textureRect = sf::IntRect(0, 0, 142, 43);
	data[GUI::Button::Back].hover = Sounds::ButtonHover;
	data[GUI::Button::Back].click = Sounds::ButtonClick;

	data[GUI::Button::MainMenu].texture = Textures::Restart;
	data[GUI::Button::MainMenu].textureRect = sf::IntRect(0, 0, 206, 41);
	data[GUI::Button::MainMenu].hover = Sounds::ButtonHover;
	data[GUI::Button::MainMenu].click = Sounds::ButtonClickSmall;

	data[GUI::Button::Player].texture = Textures::PlayerNameButton;
	data[GUI::Button::Player].textureRect = sf::IntRect(0, 0, 291, 53);
	data[GUI::Button::MainMenu].hover = Sounds::ButtonHover;
	data[GUI::Button::MainMenu].click = Sounds::ButtonClickSmall;

	data[GUI::Button::Score].texture = Textures::ScoreButton;
	data[GUI::Button::Score].textureRect = sf::IntRect(0, 0, 295, 125);
	data[GUI::Button::Score].hover = Sounds::ButtonHover;
	data[GUI::Button::Score].click = Sounds::ButtonClick;

	return data;
}

std::vector<SliderData> initializeSliderData()
{
	std::vector<SliderData> data(GUI::Slider::TypeCount);

	data[GUI::Slider::SettingsMenu].barTexture = Textures::SliderBar;
	data[GUI::Slider::SettingsMenu].barRect = sf::IntRect(0, 0, 135, 9);
	data[GUI::Slider::SettingsMenu].handleTexture = Textures::SliderKnob;
	data[GUI::Slider::SettingsMenu].handleRect = sf::IntRect(0, 0, 22, 29);

	return data;
}