#pragma once

#include "resourceidentifiers.h"
#include "stateidentifiers.h"
#include "musicplayer.h"
#include "soundplayer.h"

#include <SFML\System\Time.hpp>
#include <SFML\Window\Event.hpp>

#include <memory>
#include <string.h>

namespace sf
{
	class RenderWindow;
}

class StateStack;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, MusicPlayer& music, SoundPlayer& sounds, std::string& playerName, int& score);

		sf::RenderWindow*	window;
		TextureHolder*		textures;
		FontHolder*			fonts;
		MusicPlayer*		music;
		SoundPlayer*		sounds;
		bool*				hasPlayerWon;
		std::string*		playerName;
		int*				playaerScore;
	};


public:
						State(StateStack& stack, Context context);
	virtual				~State();

	virtual void		draw() = 0;
	virtual bool		update(sf::Time dt) = 0;
	virtual bool		handleEvent(const sf::Event& event) = 0;


protected:
	void				requestStackPush(States::ID stateID);
	void				requestStackPop();
	void				requestStateClear();

	Context				getContext() const;

private:
	StateStack*			mStack;
	Context				mContext;
};