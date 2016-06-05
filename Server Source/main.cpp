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

    char message[1024]; // Debugging
    ENetPacket* packet = NULL;

    int peers = 1;

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
        eventStatus = enet_host_service(server, &event, 00);

        // If we had some event that interested us
        if (eventStatus > 0) {

            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    event.peer->data = (char*)peers;

                    printf("(Server) We got a new connection from %x\n", event.peer->data);
                    peers++;

                    enet_packet_destroy(event.packet);

                    break;
                }


                case ENET_EVENT_TYPE_RECEIVE:
                {
                    //printf("(Server) Message from client %x : %s\n", event.peer->connectID, event.packet->data);

                    //std::strcpy(message, (char*)event.packet->); // copy ID
                    //std::strcpy(message, "|");
                    //std::strcpy(message, (char*)event.peer->data);  //
                    //std::strcat(message, (char*)event.packet->data);

                    std::string packetString = "";

                    packetString = packetString + std::to_string((int)event.peer->data) + ":" + (char*)event.packet->data;

                    //printf("%i\n", (int)event.peer->data);
                    enet_packet_destroy(event.packet);

                    packet = enet_packet_create(packetString.c_str(), strlen(packetString.c_str()) + 1, 0);


                    //std::strcpy(message, charArray);

                    //(char*)event.packet->data
                    

                    for (size_t i = 0; i < peers; i++) {

                        if (&server->peers[i] != event.peer) {


                            // message = (char*)event.packet->data;

                            enet_peer_send(&server->peers[i], 0, packet);
                            enet_host_flush(server);

                        }
                    }


                    //enet_host_broadcast(server, 0, packet);



                    break;
                }


                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    printf("%s disconnected.\n", event.peer->data);
                    // Reset client's information
                    event.peer->data = NULL;
                    enet_packet_destroy(event.packet);

                    break;
                    
                }


                
            }
        }
    }

    enet_host_destroy(server);

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