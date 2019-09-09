#include "textnode.h"
#include "utitlty.h"

#include <SFML/Graphics/RenderTarget.hpp>


TextNode::TextNode(const FontHolder& fonts, const std::string& text)
{
	mText.setFont(fonts.get(Fonts::Main));
	mText.setCharacterSize(20);
	setString(text);
}

TextNode::TextNode(const FontHolder& fonts, Fonts::ID font, const std::string& text)
{
	mText.setFont(fonts.get(font));
	mText.setCharacterSize(20);
	setString(text);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText, states);
}

void TextNode::setString(const std::string& text)
{
	mText.setString(text);
	centerOrigin(mText);
}

void TextNode::setSize(unsigned int size)
{
	mText.setCharacterSize(size);
}

void TextNode::setColor(sf::Color color)
{
	mText.setColor(color);
}

void TextNode::setFont(sf::Font& font)
{
	mText.setFont(font);
}