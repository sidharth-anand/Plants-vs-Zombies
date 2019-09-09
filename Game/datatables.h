#pragma once

#include "resourceidentifiers.h"
#include "utitlty.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>


class Plant;
class Zombie;
class Projectile;
class Button;
class Slider;

struct PlantData
{
	float							hitpoints;
	Textures::ID					texture;
	sf::IntRect						textureRect;
	sf::Time						fireInterval;
	int								price;
	int								range;
	Textures::ID					animation;
	sf::Time						animationDuration;
	sf::Vector2i					animationRect;
	int								animationFrames;
	Sounds::ID						fire;
};

struct ProjectileData
{
	float							damage;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
	Textures::ID					splatter;
	Sounds::ID						hit;
};

struct ZombieData
{
	float							hitpoints;
	float							speed;
	Textures::ID					texture;
	sf::IntRect						textureRect;
};

struct ButtonData
{
	Textures::ID					texture;
	sf::IntRect						textureRect;
	Sounds::ID						hover;
	Sounds::ID						click;
};

struct SliderData
{
	Textures::ID					barTexture;
	Textures::ID					handleTexture;
	sf::IntRect						barRect;
	sf::IntRect						handleRect;
};

std::vector<PlantData>		initializePlantData();
std::vector<ProjectileData>	initializeProjectileData();
std::vector<ZombieData>		initializeZombieData();
std::vector<ButtonData>		initializeButtonData();
std::vector<SliderData>		initializeSliderData();