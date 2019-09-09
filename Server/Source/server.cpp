#include "server.h"
#include "packettypes.h"

#include <iostream>

Server::Connection::Connection()
{
	socket.setBlocking(false);
	active = false;
}

Server::Server()
	: mSocket()
	, mListeningState(false)
	, mActiveConnections(0)
	, mMaxConnections(1)
	, mMaxClientRepsonseTime(sf::seconds(2.f))
	, mTickTime(sf::seconds(0.05f))
	, mElapsedTime()
	, mData()
{
	mListener.setBlocking(false);
	mSocket.setBlocking(false);

	mData.push_back(std::make_pair("Sidharth", 2000));

	mConnections.push_back(std::unique_ptr<Connection>(new Connection()));

	mElapsedTime.restart();

	setListening(true);
}

void Server::tick()
{
	if (mElapsedTime.getElapsedTime() >= mTickTime)
	{
		handleConnection();
		handleDisconnection();

		handleIncomingPackets();

		mElapsedTime.restart();
	}
}

void Server::setListening(bool enabled)
{
	if (enabled)
	{
		if(!mListeningState)
			mListeningState = mListener.listen(5000) == sf::TcpListener::Done;
	}
	else
	{
		mListener.close();
		mListeningState = false;
	}
}

void Server::handleConnection()
{
	if (!mListeningState)
		return;

	if (mListener.accept(mConnections[mActiveConnections]->socket) == sf::TcpListener::Done)
	{
		sf::Packet packet;
		packet << static_cast<sf::Int32>(123);
		mConnections[mActiveConnections]->socket.send(packet);
		mConnections[mActiveConnections]->lastRepsonse.restart();
		mConnections[mActiveConnections]->active = true;
		

		mActiveConnections++;

		if (mActiveConnections >= mMaxConnections)
			setListening(false);

		mListeningState = false;

		mConnections.push_back(std::unique_ptr<Connection>(new Connection()));
	}
}

void Server::handleDisconnection()
{
	for (int i = mConnections.size() - 1; i >= 0; i--)
	{
		if (mConnections[i]->lastRepsonse.getElapsedTime() > mMaxClientRepsonseTime && mConnections[i]->active)
		{
			mConnections.erase(mConnections.begin() + i);
			mActiveConnections--;

			if (mActiveConnections < mMaxConnections)
			{
				mConnections.push_back(std::unique_ptr<Connection>(new Connection()));
				setListening(true);
			}

		}
	}
}

void Server::handleIncomingPackets()
{
	for (int i = 0; i < mConnections.size(); i++)
	{
		sf::Packet packet;

		while (mConnections[i]->socket.receive(packet) == sf::Socket::Done)
		{
			handlePacket(packet, *mConnections[i]);
			mConnections[i]->lastRepsonse.restart();
			packet.clear();
		}
	}
}

void Server::handlePacket(sf::Packet packet, Connection& origin)
{
	sf::Int32 packetType;
	packet >> packetType;

	if (packetType == Packets::SendScore)
	{
		std::string name;
		int score;
		packet >> name;
		packet >> score;
		mData.push_back(std::make_pair(name, score));
		if (mData.size() > 3)
		{
			std::sort(mData.begin(), mData.end(), [](auto a, auto b) {
				return a.second > b.second;
			});
			mData.resize(3);
		}
	}
	else if (packetType == Packets::RequestLeaderboard)
	{
		sf::Packet leaderboard;

		std::cout << "Recieved Request\n";

		leaderboard << static_cast<sf::Int32>(Packets::SendLeaderboard);
		leaderboard << static_cast<sf::Int32>(mData.size());
		for (int i = 0; i < mData.size(); i++)
		{
			leaderboard << mData[i].first << mData[i].second;
		}
		origin.socket.send(leaderboard);
		origin.lastRepsonse.restart();

		std::cout << "Data Sent\n";

	}
}