#pragma once

#include <string.h>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

class Server
{
public:
								Server();

	void						tick();

private:
	struct Connection
	{
								Connection();

		sf::TcpSocket			socket;
		sf::Clock				lastRepsonse;
		bool					active;
	};

private:
	void						setListening(bool enabled);
	void						handleConnection();
	void						handleDisconnection();

	void						handleIncomingPackets();
	void						handlePacket(sf::Packet packet, Connection& origin);

private:
	sf::TcpListener				mListener;
	sf::TcpSocket				mSocket;

	bool						mListeningState;

	int							mActiveConnections;
	int							mMaxConnections;

	sf::Time					mMaxClientRepsonseTime;

	sf::Clock					mElapsedTime;
	sf::Time					mTickTime;

	std::vector<std::unique_ptr<Connection>>		mConnections;

	std::vector<std::pair<std::string, int>>		mData;
};