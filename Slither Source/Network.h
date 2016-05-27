#pragma once


#include "Globals.h"

#include <enet/enet.h>
#include <string>

class SlitherClient
{

private:


    ENetAddress address;
    ENetHost *client;
    ENetPeer *peer;

    ENetEvent event;
    int eventStatus;

    ENetPacket* packet;
    std::string packetString;
    char* receivedPacket;
    //std::string receivedString;

    //char message[1024];

    
public:
    unsigned int ID;
    float X;
    float Y;


    //Constructor, Deconstructor
    SlitherClient();
    ~SlitherClient();

    void Poll();
    void SendPosition(int x, int y, SDL_Rect &camera);

    void SplitAndStore();


};
