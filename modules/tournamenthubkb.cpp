 
#include "funkeymonkeymodule.h"
#include "uinputdevice.h"
#include <iostream>
#include <sys/socket.h>
#include <array>
#include <pthread.h>

void createsocket();
void* processsocket(void* arg);
void closesocket();

static constexpr unsigned int FIRST_KEY = KEY_RESERVED;
static constexpr unsigned int LAST_KEY = KEY_UNKNOWN;

int overlayactive = 0;
UinputDevice* out = 0;
pthread_t socketthread;

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
	std::cout << "Event! " << e.type << " " << e.code << " " << e.value << " " << std::endl;
	
	if( overlayactive != 0 )
	{
		// TODO: Redirect THub keyboard stuff
	} else {
		out->send(e.type, e.code, e.value);
	}
	
}
void destroy()
{
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
	// TODO: Poll socket for connections from the THub Overlay
}

void closesocket()
{
	pthread_cancel( socketthread );
	// TODO: Close socket
}