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

   
   eventStatus = 1;

}

bool SlitherClient::AttemptConnection(bool &networkConnected)
{
    if (enet_host_service(client, &event, 0) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        networkConnected = true;
        return true;
    }
    else
    {
        return false;

    }
}


bool SlitherClient::Poll(PacketTypes &lastPacket)
{
    eventStatus = enet_host_service(client, &event, 0); // 50000

    // If we had some event that interested us
    if (eventStatus > 0) {
        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
        {
            printf("Connected to the server: ID - %x\n", event.peer->connectID);
            //event.peer->data = (char*)(event.peer->connectID);
            //printf("%s\n", event.peer->data);
            enet_packet_destroy(event.packet);

            break;

           
            
        }
        

        case ENET_EVENT_TYPE_RECEIVE:
        {
            //printf("%s\n", event.packet->data);
            
            receivedPacket = (char*)event.packet->data;
            //printf("%s\n", receivedPacket);

            SplitAndStore(lastPacket);

            

            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);
            return true;
            break;
        }


        //case ENET_EVENT_TYPE_DISCONNECT:
        //    {
        //        printf("(Client) %x disconnected.\n", event.peer->connectID);
        //        // Reset client's information
        //        event.peer->data = NULL;
        //        break;
        //    }
        //    

        }
    }

   
    return false;
}

void SlitherClient::SendPositionPacket(int x, int y, SDL_Rect &camera)
{
    /* Create a reliable packet of size 7 containing "packet\0" */
    std::string packetString = "";

    packetString = packetString + "S:" + std::to_string(x) + ":" + std::to_string(y);

    //ss << ":S:" + std::to_string(x) + ":" + std::to_string(y) << std::endl;;

    packet = enet_packet_create(packetString.c_str(), strlen(packetString.c_str()) + 1, 0);

    /* Extend the packet so and append the string "foo", so it now */
    /* contains "packetfoo\0"                                      */
    
    //enet_packet_resize(packet, strlen("packetfoo") + 1);
    //strcpy(&packet->data[strlen("packet")], "foo");
    
    /* Send the packet to the peer over channel id 0. */
    /* One could also broadcast the packet by         */
    /* enet_host_broadcast (host, 0, packet);         */
    enet_peer_send(peer, 0, packet);

    //enet_packet_destroy(packet);
}

void SlitherClient::SplitAndStore(PacketTypes &lastPacket)
{

    if (receivedPacket == NULL)
        return;

    char* IDchar = "";
    char* Xchar = "";
    char* Ychar = "";

    int count = 0;


    char* token = strtok(receivedPacket, ":");

    while (token != NULL) 
    {
        switch (count)
        {
        case 0:
        {
            IDchar = token;
            ID = atoi(IDchar);
            break;
        }
           

        case 1:
        {

            if (std::strcmp(token, "S") == 0)
            {
                lastPacket = PacketTypes::SNAKEPOS;
                
                

            }
          

            break;
        }

        case 2:
        {
            if (lastPacket == PacketTypes::SNAKEPOS)
            {
                Xchar = token;
                X = atoi(Xchar);
            }

            break;
        }

        case 3:
        {
            if (lastPacket == PacketTypes::SNAKEPOS)
            {
                Ychar = token;
                Y = atoi(Ychar);
            }

            break;
        }

        } // end switch

        token = strtok(NULL, ":");
        count++;
    }
}