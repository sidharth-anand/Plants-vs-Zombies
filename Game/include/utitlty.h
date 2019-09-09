#pragma once


#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

#include <sstream>


namespace sf
{
	class Sprite;
	class Text;
}

class Animation;

template <typename T>
std::string		toString(const T& value);

std::string		toString(sf::Keyboard::Key key);

void			centerOrigin(sf::Sprite& sprite);
void			centerOrigin(sf::Text& text);
void			centerOrigin(Animation& animation);

float			toDegree(float radian);
float			toRadian(float degree);

int				randomInt(int min, int max);

float			length(sf::Vector2f vector);
sf::Vector2f	unitVector(sf::Vector2f vector);

struct Direction
{
	Direction()
		: angle()
		, distance()
	{
	}

	Direction(float angle, float distance)
		: angle(angle)
		, distance(distance)
	{
	}

	float angle;
	float distance;
};

#include "utility.inl"