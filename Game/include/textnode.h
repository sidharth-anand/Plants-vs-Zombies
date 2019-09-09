#pragma once

#include "resourceholder.h"
#include "resourceidentifiers.h"
#include "scenenode.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML\Graphics\Color.hpp>

class TextNode : public SceneNode
{
public:
	explicit			TextNode(const FontHolder& fonts, const std::string& text);
						TextNode(const FontHolder& fonts, Fonts::ID font, const std::string& text);

	void				setString(const std::string& text);
	void				setSize(unsigned int size);
	void				setColor(sf::Color color);
	void				setFont(sf::Font& font);

private:
	virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	sf::Text			mText;
};