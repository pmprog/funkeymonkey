 
#include "funkeymonkeymodule.h"
#include "uinputdevice.h"
#include <iostream>
#include <sys/socket.h>
#include <array>
#include <list>
#include <pthread.h>

void createsocket();
void* processsocket(void* arg);
void closesocket();

static constexpr unsigned int FIRST_KEY = KEY_RESERVED;
static constexpr unsigned int LAST_KEY = KEY_UNKNOWN;

int overlayactive = 0;
int killsocketthread = 0;
UinputDevice* out = 0;
pthread_t socketthread;
std::list<unsigned int> depressedkeys;

void init()
{
	createsocket();
	
	std::vector<unsigned int> keycodes;
	for(unsigned int i = FIRST_KEY; i <= LAST_KEY; ++i)
	{
		keycodes.push_back(i);
	}

	out = new UinputDevice("/dev/uinput", BUS_USB, "TournamentHubKB", 1, 1, 1, {
		{ EV_KEY, keycodes }
	});
}
void handle(input_event const& e)
{
	if( overlayactive != 0 )
	{
		// Release keys until none left "depressed"
		while( !depressedkeys.empty() )
		{
			out->send(EV_KEY, depressedkeys.front(), 0);
			depressedkeys.pop_front();
		}

		// TODO: Redirect keys to TournamentHub Overlay

	} else {
		out->send(e.type, e.code, e.value);

		if( e.value == 1 )
		{
			// Log depressed key for unpressing
			depressedkeys.push_back( e.code ); 
		} else {
			// Remove key code as being depressed
			depressedkeys.remove( e.code );
		}
	}

}
void destroy()
{
	killsocketthread = 1;
	closesocket();
	if( out != 0 )
	{
		delete out;
	}
}


void createsocket()
{
	// TODO: Create socket and listen
	pthread_create( &socketthread, 0, processsocket, 0 );
}

void* processsocket(void*)
{
	while( killsocketthread == 0 )
	{
		// TODO: Poll socket for connections from the THub Overlay
	}
}

void closesocket()
{
	pthread_cancel( socketthread );
	// TODO: Close socket
}
