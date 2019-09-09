#pragma once

#include "component.h"

#include <vector>
#include <memory>

namespace GUI
{

	class Container : public Component
	{
	public:
		typedef std::shared_ptr<Container> Ptr;


	public:
							Container();

		void				pack(Component::Ptr component);

		virtual bool		isSelectable() const;
		virtual void		handleEvent(const sf::Event& event);

		void				setMousePosition(sf::Vector2i position);

		void				toggleDisable(int index, bool flag);
		void				clear();
		int					size();

		bool				isButtonSelected();


	private:
		virtual void		draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool				hasSelection() const;
		void				select(std::size_t index);
		void				selectNext();
		void				selectPrevious();


	private:
		std::vector<Component::Ptr>		mChildren;
		int								mSelectedChild;
		sf::Vector2i					mMousePosition;
	};

}