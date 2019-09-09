#include "textbox.h"
#include "utitlty.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace
{
	const float padding = 0.f;
	const float height = 20.f;
}

namespace GUI
{
	TextBox::TextBox(float width, std::string default, FontHolder& fonts)
		: mCursorPosition(0)
		, mText()
		, mRect()
		, mMaxCharacters(-1)
		, mCursor()
		, mCallback()
		, mIsFocused(false)
		, mIsDisabled(false)
	{
		mText.setString(toString(default));
		mText.setFont(fonts.get(Fonts::CSMS));
		mText.setPosition(0, -padding * 4);
		mText.setCharacterSize(15);
		mText.setFillColor(sf::Color::White);
		centerOrigin(mText);

		mCursor.setSize(sf::Vector2f(1.f, height * 0.75));
		mCursor.setPosition(padding, padding);
		mCursor.setFillColor(sf::Color::Black);

		mRect.setSize(sf::Vector2f(width, height));
		mRect.setPosition(0.f, 0.f);
		mRect.setFillColor(sf::Color(82, 70, 50));
		mRect.setOrigin(width / 2, height / 2);

		setCursorPosition(0);
	}

	void TextBox::setText(std::string text)
	{
		mText.setString(text);
		setCursorPosition(text.size());
	}

	std::string TextBox::getText() const
	{
		return mText.getString();
	}

	void TextBox::setCursorPosition(std::size_t position)
	{
		if (position <= mText.getString().getSize())
		{
			mCursorPosition = position;

			mCursor.setPosition(mText.findCharacterPos(position).x, mText.getPosition().y);
			std::cout << mCursor.getPosition().x << " " << mText.getPosition().x << "\n";
			mCursorClock.restart();
		}
	}

	bool TextBox::isSelectable() const
	{
		return false;
	}

	void TextBox::select()
	{
	}

	void TextBox::deselect()
	{
	}

	void TextBox::activate()
	{
		if (!mIsDisabled)
		{
			Component::activate();

			mRect.setOutlineThickness(2.f);
			mRect.setOutlineColor(sf::Color(169, 178, 157));
			mIsFocused = true;
		}
	}

	void TextBox::deactivate()
	{
		Component::deactivate();

		mRect.setOutlineThickness(0.f);
		mIsFocused = false;
	}

	sf::FloatRect TextBox::getBoundingRect()
	{
		return sf::FloatRect(sf::Vector2f(mRect.getPosition().x - mRect.getSize().x / 2, mRect.getPosition().y - mRect.getSize().y / 2), mRect.getSize());
	}

	void TextBox::setDisabled(bool flag)
	{
		mIsDisabled = flag;
	}

	void TextBox::setPosition(sf::Vector2f position)
	{
		mRect.setPosition(position);
		mCursor.setPosition(position.x + padding, position.y + padding);
		mText.setPosition(position.x, position.y);
		setCursorPosition(0);
	}

	void TextBox::setPosition(float x, float y)
	{
		mRect.setPosition(x, y);
		mCursor.setPosition(x + padding, y);
		mText.setPosition(x, y);
		setCursorPosition(0);
	}

	std::size_t TextBox::getCursorPosition() const
	{
		return mCursorPosition;
	}

	void TextBox::setCallback(Callback callback)
	{
		mCallback = callback;
	}

	void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mRect, states);

		if(mIsFocused)
			target.draw(mCursor, states);

		target.draw(mText, states);
	}

	void TextBox::handleEvent(const sf::Event& event)
	{
		switch(event.type)
		{
		case sf::Event::KeyPressed:
			handleKeyPress(event);
			break;
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
			handleMouseEvent(event);
			break;
		case sf::Event::TextEntered:
			handleTextEntry(event);
			break;
		}
	}

	void TextBox::handleMouseEvent(const sf::Event& event)
	{
		if (getBoundingRect().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
		{
			for (int i = mText.getString().getSize(); i >= 0; --i)
			{
				sf::Vector2f pos = mText.findCharacterPos(i);
				if (pos.x <= event.mouseButton.x)
				{
					setCursorPosition(i);
					break;
				}
			}
			activate();
		}
		else
			deactivate();
	}

	void TextBox::handleKeyPress(const sf::Event& event)
	{
		if (mIsFocused)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Left:
				setCursorPosition(mCursorPosition - 1);
				break;

			case sf::Keyboard::Right:
				setCursorPosition(mCursorPosition + 1);
				break;

			case sf::Keyboard::BackSpace:
				if (mCursorPosition > 0)
				{
					sf::String string = mText.getString();
					string.erase(mCursorPosition - 1);
					mText.setString(string);

					setCursorPosition(mCursorPosition - 1);
				}
				break;

			case sf::Keyboard::Delete:
				if (mCursorPosition < mText.getString().getSize())
				{
					sf::String string = mText.getString();
					string.erase(mCursorPosition);
					mText.setString(string);

					setCursorPosition(mCursorPosition);
				}
				break;

			case sf::Keyboard::Home:
				setCursorPosition(0);
				break;

			case sf::Keyboard::End:
				setCursorPosition(mText.getString().getSize());
				break;

			case sf::Keyboard::Return:
				if (mCallback)
					mCallback();
				break;
			}
		}
	}

	void TextBox::handleTextEntry(const sf::Event& event)
	{
		if (mIsFocused)
		{
			auto unicode = event.text.unicode;
			if (unicode > 30 && (unicode < 127 || unicode > 159))
			{
				sf::String string = mText.getString();
				if (mMaxCharacters == -1 || (int)string.getSize() < mMaxCharacters)
				{
					string.insert(mCursorPosition, unicode);
					mText.setString(string);

					setCursorPosition(mCursorPosition + 1);

					mText.move(-4.f, 0.f);
				}
			}
		}
	}
}