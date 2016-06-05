#pragma once


#include "Globals.h"

#include <enet/enet.h>
#include <string>

#include <iostream>
#include <sstream>


class SlitherClient
{

private:

    bool connected;
    PacketTypes lastPacketType;

    ENetAddress address;
    ENetHost *client;
    ENetPeer *peer;

    ENetEvent event;
    int eventStatus;

    ENetPacket* packet;
    std::string packetString;
    char* receivedPacket;
    std::stringstream ss;
    //std::string receivedString;

    //char message[1024];

    
public:
    int ID = -1;
    float X = -1;
    float Y = -1;


    //Constructor, Deconstructor
    SlitherClient();
    ~SlitherClient();

    bool AttemptConnection(bool &connected);

    bool Poll(PacketTypes &lastPacket);

    void SendPositionPacket(int x, int y, SDL_Rect &camera);

    void SplitAndStore(PacketTypes &lastPacket);


};
