#pragma once

#include "component.h"
#include "resourceholder.h"
#include "resourceidentifiers.h"
#include "soundplayer.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML\Graphics\Color.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>


namespace GUI
{

	class Button : public Component
	{
	public:
		typedef std::shared_ptr<Button>		Ptr;
		typedef std::function<void()>		Callback;

		enum Type
		{
			Normal,
			Selected,
			Pressed,
			Disabled,
			ButtonCount
		};

		enum BType
		{
			PlantButton,
			AdventureButton,
			Options,
			Quit,
			OptionsBack,
			PauseMenu,
			Back,
			MainMenu,
			Player,
			Score,
			TypeCount,
		};


	public:
								Button(const FontHolder& fonts, TextureHolder& textures, SoundPlayer& sounds, BType type);

		void					setCallback(Callback callback);
		void					setText(const std::string& text);
		void					setFont(const sf::Font& font);
		void					setCharacterSize(const unsigned int size);
		void					setContent(sf::Texture& texture);
		void					setImageOffset(sf::Vector2f offset);
		void					setTextOffset(sf::Vector2f offset);
		void					setToggle(bool flag);
		void					hasIcon(bool flag);

		void					setTextColor(const sf::Color& color, Type type);
		void					setIconColor(const sf::Color& color, Type type);

		virtual bool			isSelectable() const;
		virtual void			select();
		virtual void			deselect();

		virtual void			activate();
		virtual void			deactivate();

		virtual void			handleEvent(const sf::Event& event);
		virtual sf::FloatRect	getBoundingRect();

		virtual void			setDisabled(bool flag);

		virtual void			playHoverSound();

	private:
		virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
		void					changeTexture(Type buttonType);


	private:
		Callback				mCallback;
		sf::Sprite				mSprite;
		sf::Text				mText;
		SoundPlayer&			mSounds;
		sf::Sprite				mIcon;
		sf::IntRect				mTextureRect;
		bool					mIsToggle;
		bool					mHasIcon;
		bool					mIsDisabled;

		std::vector<sf::Color>	mTextColor;
		std::vector<sf::Color>	mIconColor;

		BType					mType;
	};

}