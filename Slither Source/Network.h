#pragma once


#include "Globals.h"

#include <enet/enet.h>

class SlitherClient
{

private:
    

    ENetAddress address;
    ENetHost *client;
    ENetPeer *peer;
    char message[1024];
    ENetEvent event;
    int eventStatus;

    unsigned int ID;

    ENetPacket* packet;
    std::string packetString;

public:

    //Constructor, Deconstructor
    SlitherClient();
    ~SlitherClient();

    void Poll();
    void SendPosition(int x, int y);

};
