#include "button.h"
#include "utitlty.h"
#include "datatables.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace
{
	std::vector<ButtonData> Table = initializeButtonData();
}

namespace GUI
{
	Button::Button(const FontHolder& fonts, TextureHolder& textures, SoundPlayer& sounds, BType type)
		:mCallback()
		, mSprite(textures.get(Table[type].texture))
		, mTextureRect(Table[type].textureRect)
		, mHasIcon(false)
		, mText("", fonts.get(Fonts::Main))
		, mIcon()
		, mIsDisabled(false)
		, mTextColor()
		, mIconColor()
		, mSounds(sounds)
		, mType(type)
	{
		mTextColor = std::vector<sf::Color>(ButtonCount);
		mIconColor = std::vector<sf::Color>(ButtonCount);

		mTextColor[Type::Normal] = sf::Color::White;
		mTextColor[Type::Selected] = sf::Color::Black;
		mTextColor[Type::Pressed] = sf::Color::Black;
		mTextColor[Type::Disabled] = sf::Color(0, 0, 0, 125);

		mIconColor[Type::Normal] = sf::Color::White;
		mIconColor[Type::Selected] = sf::Color::White;
		mIconColor[Type::Pressed] = sf::Color::White;
		mIconColor[Type::Disabled] = sf::Color(0, 0, 0, 100);

		changeTexture(Normal);

		centerOrigin(mSprite);
		centerOrigin(mText);
		centerOrigin(mIcon);

		mText.setPosition(mSprite.getPosition());
		mIcon.setPosition(mSprite.getPosition());
	}

	void Button::setCallback(Callback callback)
	{
		mCallback = std::move(callback);
	}

	void Button::setText(const std::string& text)
	{
		mText.setString(text);
		centerOrigin(mText);
	}

	void Button::setFont(const sf::Font & font)
	{
		mText.setFont(font);
		centerOrigin(mText);
	}

	void Button::setContent(sf::Texture& texture)
	{
		mIcon.setTexture(texture);
		centerOrigin(mIcon);
	}

	void Button::setImageOffset(sf::Vector2f offset)
	{
		mIcon.setPosition(mIcon.getPosition() + offset);
	}

	void Button::setTextOffset(sf::Vector2f offset)
	{
		mText.setPosition(mText.getPosition() + offset);
	}

	void Button::setToggle(bool flag)
	{
		mIsToggle = flag;
	}

	bool Button::isSelectable() const
	{
		return true;
	}

	void Button::select()
	{
		if (!mIsDisabled)
		{
			Component::select();
			changeTexture(Selected);
		}
	}

	void Button::deselect()
	{
		if (!mIsDisabled)
		{
			Component::deselect();
			changeTexture(Normal);
		}
	}

	void Button::activate()
	{
		if (!mIsDisabled)
		{
			Component::activate();

			if (mIsToggle)
				changeTexture(Pressed);

			if (mCallback)
				mCallback();

			if (!mIsToggle)
				deactivate();

			mSounds.play(Table[mType].click);
		}
	}

	void Button::deactivate()
	{
		Component::deactivate();

		if (mIsToggle)
		{
			if (isSelected())
				changeTexture(Selected);
			else
				changeTexture(Normal);
		}
	}

	void Button::handleEvent(const sf::Event&)
	{
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mSprite, states);
		if (mHasIcon)
		{
			target.draw(mIcon, states);
			if (mText.getString() != "")
				target.draw(mText, states);
		}
		else
			target.draw(mText, states);
	}

	void Button::changeTexture(Type buttonType)
	{
		sf::IntRect textureRect(mTextureRect.width * buttonType, 0, mTextureRect.width, mTextureRect.height);
		mSprite.setTextureRect(textureRect);

		mIcon.setColor(mIconColor[buttonType]);
		mText.setColor(mTextColor[buttonType]);
	}

	sf::FloatRect Button::getBoundingRect()
	{
		sf::Vector2f size(mTextureRect.width,mTextureRect.height);
		sf::Vector2f position;
		position.x = getPosition().x - size.x / 2;
		position.y = getPosition().y - size.y / 2;
		return sf::FloatRect(position, size);
	}
	void Button::setDisabled(bool flag)
	{
		if (flag)
			changeTexture(Disabled);
		else
		{
			if (isSelected())
				changeTexture(Selected);
			else
				changeTexture(Normal);
		}
		mIsDisabled = flag;
	}

	void Button::playHoverSound()
	{
		mSounds.play(Table[mType].hover);
	}

	void Button::hasIcon(bool flag)
	{
		mHasIcon = flag;
	}

	void Button::setTextColor(const sf::Color & color, Type type)
	{
		mTextColor[type] = color;
		if (isSelected())
			changeTexture(Selected);
		else
			changeTexture(Normal);
	}

	void Button::setIconColor(const sf::Color& color, Type type)
	{
		mIconColor[type] = color;
		if (isSelected())
			changeTexture(Selected);
		else
			changeTexture(Normal);
	}

	void Button::setCharacterSize(const unsigned int size)
	{
		mText.setCharacterSize(size);
		centerOrigin(mText);
	}
}
