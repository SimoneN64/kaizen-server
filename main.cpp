#define ENET_IMPLEMENTATION
#include <enet.h>
#include <cstdio>
#include <vector>
#include <arena_buffer.hpp>
#include <algorithm>

static std::vector<ENetPeer*> gPeers{};

enum PeerCommand : uint8_t {
  ePC_None,
  ePC_PeerList,
  ePC_NewPeer,
};

int main() {
  ArenaBuffer b;
  if (enet_initialize() != 0) {
    printf("An error occurred while initializing ENet.\n");
    return 1;
  }

  ENetAddress hostAddress;
  hostAddress.host = ENET_HOST_ANY;
  hostAddress.port = 7788;
  ENetHost* host = enet_host_create(&hostAddress, 4, 2, 0, 0);

  printf("Listening on port %d\n", hostAddress.port);
  while(true) {
    ENetEvent evt;
    if(enet_host_service(host, &evt, 1000) > 0) {
      char ip[40];
      enet_address_get_host_ip(&evt.peer->address, ip, 40);

      if(evt.type == ENET_EVENT_TYPE_CONNECT) {
        printf("New connection from %s\n", ip);

        b.Reset();
        b.Write(ePC_PeerList);
        b.Write((uint32_t) gPeers.size());

        for (auto peer: gPeers) {
          b.Write(peer->address.host);
          b.Write(peer->address.port);
        }

        ENetPacket *peerListPacket = enet_packet_create(b.GetBuffer(), b.GetSize(), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(evt.peer, 0, peerListPacket);

        b.Reset();
        b.Write(ePC_NewPeer);
        b.Write(evt.peer->address.host);
        b.Write(evt.peer->address.port);
        ENetPacket *newPeerPacket = enet_packet_create(b.GetBuffer(), b.GetSize(), ENET_PACKET_FLAG_RELIABLE);

        for (auto peer: gPeers) {
          enet_peer_send(peer, 0, newPeerPacket);
        }

        gPeers.emplace_back(evt.peer);
      } else if(evt.type == ENET_EVENT_TYPE_DISCONNECT) {
        auto it = std::find(gPeers.begin(), gPeers.end(), evt.peer);
        gPeers.erase(it);
      } else if(evt.type == ENET_EVENT_TYPE_RECEIVE) {
        printf("Data received! %d bytes from %s\n", (int)evt.packet->dataLength, ip);
      } else {
        printf("Unhandled event from %s\n", ip);
      }
    } else {
      printf("... (%zu peers)\n", gPeers.size());
    }
  }

  enet_host_destroy(host);
  enet_deinitialize();
  return 0;
}