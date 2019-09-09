#pragma once

#include "scenenode.h"

#include <SFML/Graphics/Sprite.hpp>


class SpriteNode : public SceneNode
{
public:
	explicit			SpriteNode(const sf::Texture& texture);
						SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);

	void				setTexture(sf::Texture& texture);
	void				setTextureRect(sf::IntRect& textureRect);

	virtual sf::FloatRect	getBoundingRect() const;
private:
	virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	sf::Sprite			mSprite;
};
