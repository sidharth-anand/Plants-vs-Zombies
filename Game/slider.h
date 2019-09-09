#pragma once

#include "component.h"
#include "resourceholder.h"
#include "resourceidentifiers.h"

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Color.hpp>

#include <memory>
#include <functional>

namespace GUI
{
	class Slider : public Component
	{
	public:
		typedef std::shared_ptr<Slider>		Ptr;
		typedef std::function<void()>		Callback;

		enum Type
		{
			Normal,
			Selected,
			Pressed,
			Disabled,
			Count,
		};

		enum SliderType
		{
			SettingsMenu,
			TypeCount
		};

	public:
								Slider(TextureHolder& textures, SliderType type);

		void					setCallback(Callback callback);
		void					setHandleImage(sf::Texture& texture);

		void					setQuantum(int quantum);
		int						getQuantum() const;
		void					setValue(int value);
		int						getValue() const;

		virtual bool			isSelectable() const;
		virtual void			select();
		virtual void			deselect();

		virtual void			activate();
		virtual void			deactivate();

		virtual void			handleEvent(const sf::Event& event);
		virtual sf::FloatRect	getBoundingRect();

		virtual void			setDisabled(bool flag);

		virtual void			setPosition(sf::Vector2f position);
		virtual void			setPosition(float x, float y);

	private:
		virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void					changeTexture(Type type);
		void					updateHandlePosition();

	private:
		Callback				mCallback;
		sf::Sprite				mSlidingBar;
		sf::Sprite				mHandle;
		sf::IntRect				mBarRect;
		sf::IntRect				mHandleRect;
		bool					mIsDisabled;
		int						mQuantum;
		int						mValue;
		bool					mIsPressed;
	};
}