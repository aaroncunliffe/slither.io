#include <enet/enet.h>
#include <stdio.h>

#include <string>  //Debugging
#include <cstring> //Debugging

#include <iostream>
#include <sstream>


int main(int argc, char **argv)
{
    ENetAddress address;
    ENetHost *server;
    ENetEvent event;
    int eventStatus;

    std::stringstream ss;
    char message[1024]; // Debugging
    ENetPacket* packet;

    // a. Initialize enet
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occured while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    atexit(enet_deinitialize);

    // b. Create a host using enet_host_create
    address.host = ENET_HOST_ANY; // Localhost
    address.port = 1234;

    server = enet_host_create(&address, 32, 2, 0, 0);

    if (server == NULL) {
        fprintf(stderr, "An error occured while trying to create an ENet server host\n");
        exit(EXIT_FAILURE);
    }

    // c. Connect and user service
    eventStatus = 1;

    while (1) {
        eventStatus = enet_host_service(server, &event, 50000);

        // If we had some event that interested us
        if (eventStatus > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                printf("(Server) We got a new connection from %x\n", event.peer->address.host);
                event.peer->data = (char*)(event.peer->incomingPeerID);
                break;

            case ENET_EVENT_TYPE_RECEIVE:
               //printf("(Server) Message from client %x : %s\n", event.peer->connectID, event.packet->data);

                //std::strcpy(message, (char*)event.packet->); // copy ID
                //std::strcpy(message, "|");
                std::strcpy(message, (char*)event.packet->data);

                //(char*)event.packet->data
                printf("(Server) Message from client %x : %s\n", event.peer->connectID, message);

                for (size_t i = 0; i < server->peerCount ; i++) {
                    if (&server->peers[i] != event.peer) {
                       // message = (char*)event.packet->data;
                       
                        packet = enet_packet_create(message, strlen(message) + 1, 0);
                        enet_peer_send(&server->peers[i], 0, packet);
                        enet_host_flush(server);
                        
                    }
                }
                enet_packet_destroy(packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%s disconnected.\n", event.peer->data);
                // Reset client's information
                event.peer->data = NULL;
                break;

            }
        }
    }

}

//void CreatePacket()
//{
//    /* Create a reliable packet of size 7 containing "packet\0" */
//    ENetPacket* packet = enet_packet_create("packet", strlen("packet") + 1, ENET_PACKET_FLAG_RELIABLE);
//
//    /* Extend the packet so and append the string "foo", so it now */
//    /* contains "packetfoo\0"                                      */
//    
//    enet_packet_resize(packet, strlen("packetfoo") + 1);
//    strcpy(&packet->data[strlen("packet")], "foo");
//    /* Send the packet to the peer over channel id 0. */
//    /* One could also broadcast the packet by         */
//    /* enet_host_broadcast (host, 0, packet);         */
//    enet_peer_send(peer, 0, packet);
//    ...
//        ...
//        ...
//        /* One could just use enet_host_service() instead. */
//        enet_host_flush(host);
//}