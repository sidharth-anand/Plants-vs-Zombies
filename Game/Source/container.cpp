#include "container.h"
#include "foreach.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

namespace GUI
{

	Container::Container()
		: mChildren()
		, mSelectedChild(-1)
	{
	}

	void Container::pack(Component::Ptr component)
	{
		mChildren.push_back(component);
	}

	bool Container::isSelectable() const
	{
		return false;
	}

	void Container::setMousePosition(sf::Vector2i position)
	{
		mMousePosition = position;
	}

	void Container::toggleDisable(int index, bool flag)
	{
		mChildren[index]->setDisabled(flag);
	}

	void Container::clear()
	{
		mChildren.clear();
		mSelectedChild = -1;
	}

	int Container::size()
	{
		return mChildren.size();
	}

	bool Container::isButtonSelected()
	{
		return mSelectedChild != -1;
	}

	void Container::handleEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseMoved)
		{
			mSelectedChild = -1;

			FOREACH(const Component::Ptr& child, mChildren)
			{
				if (child->getBoundingRect().contains(mMousePosition.x, mMousePosition.y) && child->isSelectable())
				{
					if (!child->isSelected())
						child->playHoverSound();
					auto it = std::find(mChildren.begin(), mChildren.end(), child);
					if (it != mChildren.end())
					{
						auto index = std::distance(mChildren.begin(), it);
						mSelectedChild = index;
					}
					child->select();
				}
				else
					child->deselect();
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			if (hasSelection())
				mChildren[mSelectedChild]->activate();
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (hasSelection())
				mChildren[mSelectedChild]->deactivate();
		}

		FOREACH(const Component::Ptr& child, mChildren)
		{
			child->handleEvent(event);
		}
	}

	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		FOREACH(const Component::Ptr& child, mChildren)
			target.draw(*child, states);
	}

	bool Container::hasSelection() const
	{
		return mSelectedChild >= 0;
	}

	void Container::select(std::size_t index)
	{
		if (mChildren[index]->isSelectable())
		{
			if (hasSelection())
				mChildren[mSelectedChild]->deselect();

			mChildren[index]->select();
			mSelectedChild = index;
		}
	}

	void Container::selectNext()
	{
		if (!hasSelection())
			return;

		int next = mSelectedChild;
		do
			next = (next + 1) % mChildren.size();
		while (!mChildren[next]->isSelectable());

		select(next);
	}

	void Container::selectPrevious()
	{
		if (!hasSelection())
			return;

		int prev = mSelectedChild;
		do
			prev = (prev + mChildren.size() - 1) % mChildren.size();
		while (!mChildren[prev]->isSelectable());

		select(prev);
	}

}