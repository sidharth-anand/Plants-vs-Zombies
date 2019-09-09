#include "slider.h"
#include "utitlty.h"
#include "datatables.h"

#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Texture.hpp>

namespace
{
	std::vector<SliderData> Table = initializeSliderData();
}

namespace GUI
{
	Slider::Slider(TextureHolder& textures, SliderType type)
		: mCallback()
		, mSlidingBar(textures.get(Table[type].barTexture))
		, mBarRect(Table[type].barRect)
		, mHandle(textures.get(Table[type].handleTexture))
		, mHandleRect(Table[type].handleRect)
		, mIsDisabled(false)
		, mValue(0)
		, mQuantum(1)
		, mIsPressed(false)
	{
		changeTexture(Normal);

		centerOrigin(mSlidingBar);
		centerOrigin(mHandle);

		updateHandlePosition();
	}

	void Slider::setCallback(Callback callback)
	{
		mCallback = callback;
	}

	bool Slider::isSelectable() const
	{
		return true;
	}

	void Slider::select()
	{
		if (!mIsDisabled)
		{
			Component::select();
			changeTexture(Selected);
		}
	}

	void Slider::deselect()
	{
		if (!mIsDisabled)
		{
			Component::deselect();
			changeTexture(Normal);
		}
	}

	void Slider::activate()
	{
		if (!mIsDisabled)
		{
			Component::activate();

			changeTexture(Pressed);
			mIsPressed = true;
		}
	}

	void Slider::deactivate()
	{
		if (!mIsDisabled)
		{
			if (isSelected())
				changeTexture(Selected);
			else
				changeTexture(Normal);

			mIsPressed = false;
		}
	}

	void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mSlidingBar, states);
		target.draw(mHandle, states);
	}

	void Slider::changeTexture(Type type)
	{
		mHandle.setTextureRect(sf::IntRect(mHandleRect.width * type, 0, mHandleRect.width, mHandleRect.height));
	}

	void Slider::setDisabled(bool flag)
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

	void Slider::setQuantum(int quantum)
	{
		if (quantum > 0 && quantum < 100)
			mQuantum = quantum;
	}

	int Slider::getQuantum() const
	{
		return mQuantum;
	}

	void Slider::setValue(int value)
	{
		if (value < 0)
			value = 0;
		else if (value > 100)
			value = 100;
		else
		{
			int temp = value + mQuantum / 2;
			value = temp - temp % mQuantum;
		}

		if (value != mValue)
		{
			mValue = value;
			updateHandlePosition();
			if (mCallback)
				mCallback();
		}
	}

	int Slider::getValue() const
	{
		return mValue;
	}

	void Slider::setHandleImage(sf::Texture& texture)
	{
		mHandle.setTexture(texture);
		mHandleRect = sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y);
	}

	sf::FloatRect Slider::getBoundingRect()
	{
		return sf::FloatRect(mHandle.getPosition().x - mHandleRect.width / 2, mHandle.getPosition().y - mHandleRect.height / 2, mHandleRect.width, mHandleRect.height);
	}

	void Slider::handleEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseMoved && mIsPressed)
		{
			int x = event.mouseMove.x - mSlidingBar.getPosition().x + mBarRect.width / 2;
			setValue(100 * x / mBarRect.width);
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			deactivate();
		}
	}

	void Slider::updateHandlePosition()
	{
		mHandle.setPosition(mSlidingBar.getPosition().x - mBarRect.width / 2 + mValue * mBarRect.width / 100, mSlidingBar.getPosition().y);
	}

	void Slider::setPosition(sf::Vector2f position)
	{
		mSlidingBar.setPosition(position);
		updateHandlePosition();
	}

	void Slider::setPosition(float x, float y)
	{
		mSlidingBar.setPosition(x, y);
		updateHandlePosition();
	}
}