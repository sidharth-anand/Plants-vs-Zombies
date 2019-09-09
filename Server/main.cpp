#include "server.h"

#include <iostream>

int main()
{
	Server server;
	while (true)
	{
		server.tick();
	}
}