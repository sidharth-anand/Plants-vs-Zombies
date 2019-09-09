#include "spritenode.h"
#include "utitlty.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML\Graphics\Texture.hpp>

SpriteNode::SpriteNode(const sf::Texture& texture)
	: mSprite(texture)
{
	centerOrigin(mSprite);
}

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect)
	: mSprite(texture, textureRect)
{
	centerOrigin(mSprite);
}

void SpriteNode::setTexture(sf::Texture& texture)
{
	mSprite.setTexture(texture);
	centerOrigin(mSprite);
}

void SpriteNode::setTextureRect(sf::IntRect& textureRect)
{
	mSprite.setTextureRect(textureRect);
	centerOrigin(mSprite);
}

sf::FloatRect SpriteNode::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}