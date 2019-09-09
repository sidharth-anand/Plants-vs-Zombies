#pragma once

#include "component.h"
#include "resourceholder.h"
#include "resourceidentifiers.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Text.hpp>

#include <memory>
#include <functional>

namespace GUI
{
	class TextBox : public Component
	{
	public:
		typedef std::shared_ptr<TextBox>	Ptr;
		typedef std::function<void()>		Callback;

		enum Type
		{
			Normal,
			Selected,
			Focused,
			Disabled,
			Count,
		};

	public:
									TextBox(float width, std::string default, FontHolder& fonts);
									
		void						setText(std::string text);
		std::string					getText() const;

		virtual bool				isSelectable() const;
		virtual void				select();
		virtual void				deselect();

		virtual void				activate();
		virtual void				deactivate();
			
		virtual void				handleEvent(const sf::Event& event);
		virtual sf::FloatRect		getBoundingRect();

		virtual void				setDisabled(bool flag);

		virtual void				setPosition(sf::Vector2f position);
		virtual void				setPosition(float x, float y);

		void						setCallback(Callback callback);

	private:
		virtual void				draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void						setCursorPosition(std::size_t position);
		std::size_t					getCursorPosition() const;

		void						handleKeyPress(const sf::Event& event);
		void						handleMouseEvent(const sf::Event& event);
		void						handleTextEntry(const sf::Event& event);

	private:
		sf::Text					mText;
		sf::RectangleShape			mRect;

		sf::RectangleShape			mCursor;
		sf::Clock					mCursorClock;

		std::size_t					mCursorPosition;
		int							mMaxCharacters;

		Callback					mCallback;

		bool						mIsFocused;
		bool						mIsDisabled;
	};
}