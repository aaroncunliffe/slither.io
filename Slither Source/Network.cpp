#include "Network.h"


SlitherClient::SlitherClient()
{
    // a. Initialize enet
    if (enet_initialize() != 0) {
        fprintf(stderr, "An error occured while initializing ENet.\n");
        throw;
    }

    // b. Create a host using enet_host_create
    client = enet_host_create(NULL /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        2 /* allow up 2 channels to be used, 0 and 1 */,
        57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
        14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);

    if (client == NULL) {
        fprintf(stderr, "An error occured while trying to create an ENet server host\n");
        exit(EXIT_FAILURE);
    }

    enet_address_set_host(&address, "localhost");
    address.port = 1234;

    // c. Connect and user service
    peer = enet_host_connect(client, &address, 2, 0);
    
    if (peer == NULL) {
        fprintf(stderr, "No available peers for initializing an ENet connection");
        exit(EXIT_FAILURE);
    }

   

   ID = peer->connectID;

   printf("%i", ID);

    eventStatus = 1;


}


void SlitherClient::Poll()
{
    eventStatus = enet_host_service(client, &event, 0); // 50000

    // If we had some event that interested us
    if (eventStatus > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            printf("(Client) We got a new connection from %x\n", event.peer->address.host);

            

            //printf("%s\n", event.peer->data);
            break;

        case ENET_EVENT_TYPE_RECEIVE:

            printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                event.packet->dataLength,
                event.packet->data,
                event.peer->data,
                event.channelID);
            /* Clean up the packet now that we're done using it. */

            enet_packet_destroy(event.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("(Client) %x disconnected.\n", event.peer->connectID);
            // Reset client's information
            event.peer->data = NULL;
            break;
        }
    }

   /* printf("Say > ");
    gets_s(message, 1024);

    if (strlen(message) > 0) {
        ENetPacket *packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
    }*/
}

void SlitherClient::SendPosition(int x, int y)
{
    /* Create a reliable packet of size 7 containing "packet\0" */
    std::string packetString = std::to_string(peer->connectID);

    packetString = packetString + "|" + "X:" + std::to_string(x) + "Y:" + std::to_string(y) + "$";


    packet = enet_packet_create(packetString.c_str(), strlen(packetString.c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);

    /* Extend the packet so and append the string "foo", so it now */
    /* contains "packetfoo\0"                                      */
    
    //enet_packet_resize(packet, strlen("packetfoo") + 1);
    //strcpy(&packet->data[strlen("packet")], "foo");
    
    /* Send the packet to the peer over channel id 0. */
    /* One could also broadcast the packet by         */
    /* enet_host_broadcast (host, 0, packet);         */
    enet_peer_send(peer, 0, packet);
}