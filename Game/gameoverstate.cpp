#include "gameoverstate.h"
#include "utitlty.h"
#include "resourceholder.h"
#include "packettypes.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\View.hpp>
#include <SFML\Network.hpp>


GameOverState::GameOverState(StateStack& stack, Context context)
	: State(stack, context)
	, mSprite()
	, mElapsedTime(sf::Time::Zero)
{
	if (*(context.hasPlayerWon))
	{
		sf::Texture& texture = context.textures->get(Textures::Win);
		mSprite.setTexture(texture);
		context.music->play(Music::Win);

		sf::TcpSocket socket;
		socket.connect("127.0.0.1", 5000, sf::seconds(5.f));

		sf::Packet packet;
		packet << static_cast<sf::Int32>(Packets::SendScore);
		packet << static_cast<sf::Int32>(*context.playaerScore);

		socket.send(packet);
		packet.clear();
	}
	else
	{
		sf::Texture& texture = context.textures->get(Textures::Lose);
		mSprite.setTexture(texture);
		context.music->play(Music::Lose);
	}

	centerOrigin(mSprite);
	mSprite.setPosition(context.window->getSize().x / 2, context.window->getSize().y / 2);

	context.music->setLooped(false);
}

GameOverState::~GameOverState()
{
	getContext().music->setLooped(true);
}

void GameOverState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mSprite);
}

bool GameOverState::update(sf::Time dt)
{
	mElapsedTime += dt;
	if (mElapsedTime > sf::seconds(6))
	{
		requestStateClear();
		requestStackPush(States::Menu);
		getContext().music->play(Music::Menu);
	}
	return false;
}

bool GameOverState::handleEvent(const sf::Event&)
{
	return false;
}