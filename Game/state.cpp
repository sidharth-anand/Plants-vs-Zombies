#include "state.h"
#include "statestack.h"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, MusicPlayer& music, SoundPlayer& sounds, std::string& name, int& score)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
	, music(&music)
	, sounds(&sounds)
	, hasPlayerWon(new bool(false))
	, playerName(&name)
	, playaerScore(&score)
{
}

State::State(StateStack& stack, Context context)
	: mStack(&stack)
	, mContext(context)
{

}

State::~State()
{

}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}