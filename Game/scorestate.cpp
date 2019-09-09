#include "scorestate.h"
#include "utitlty.h"
#include "packettypes.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Network.hpp>

#include <iostream>

ScoreState::ScoreState(StateStack& stack, Context context)
	: State(stack, context)
	, mText()
	, mBackground()
	, mGUIContainer()
	, mScores(3)
{
	mText.setString("Player");
	mText.setFillColor(sf::Color::White);
	mText.setCharacterSize(30u);
	mText.setFont(context.fonts->get(Fonts::CSMS));

	mBackground = sf::Sprite(context.textures->get(Textures::ScoreScreen));
	centerOrigin(mBackground);
	mBackground.setPosition(400.f, 300.f);

	sf::TcpSocket socket;
	socket.setBlocking(true);
	socket.connect("127.0.0.1", 5000, sf::seconds(5.f));

	sf::Packet packet;
	packet << static_cast<sf::Int32>(Packets::RequestLeaderboard);

	socket.send(packet);
	packet.clear();

	sf::Packet data;

	while (socket.receive(data) == sf::Socket::Done)
	{
		sf::Int32 type;
		data >> type;
		sf::Int32 length;
		data >> length;
		std::cout << static_cast<int>(length) << "\n";
		for (int i = 0; i < static_cast<int>(length); i++)
		{
			std::string name = "";
			int score = 0;
			data >> name >> score;
			mScores[i] = std::make_pair(name, score);
		}
	}
}

void ScoreState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	sf::Text player1;
	sf::Text player2;
	sf::Text player3;

	centerOrigin(player1);
	centerOrigin(player2);
	centerOrigin(player3);

	if (mScores.size() >= 1)
	{
		player1.setString(mScores[0].first + " - "  + toString(mScores[0].second));
		player1.setColor(sf::Color(132, 133, 177));
		player1.setFont(getContext().fonts->get(Fonts::CSMS));
		player1.setCharacterSize(30u);
		player1.setPosition(320, 215);
	}

	if (mScores.size() >= 2)
	{
		player2.setString(mScores[1].first + " - " + toString(mScores[1].second));
		player2.setColor(sf::Color(132, 133, 177));
		player2.setFont(getContext().fonts->get(Fonts::CSMS));
		player2.setCharacterSize(25u);
		player2.setPosition(340.5, 309);
	}

	if (mScores.size() == 3)
	{
		player3.setString(mScores[2].first + " - " + toString(mScores[2].second));
		player3.setColor(sf::Color(132, 133, 177));
		player3.setFont(getContext().fonts->get(Fonts::CSMS));
		player3.setCharacterSize(20u);
		player3.setPosition(330.5, 390);
	}

	window.draw(mBackground);
	window.draw(player1);
	window.draw(player2);
	window.draw(player3);
}

bool ScoreState::update(sf::Time dt)
{
	return false;
}

bool ScoreState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}
	return false;
}