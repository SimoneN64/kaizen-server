#define ENET_IMPLEMENTATION
#include <enet.h>
#include <cstdio>
#include <vector>
#include <string>
#include <unordered_map>
#include <arena.hpp>
#include <algorithm>
#define MAX_LOBBIES 16

enum ServerSideCommand : uint8_t {
  eSCMD_None,
  eSCMD_JoinLobby,
  eSCMD_CreateLobby,
};

enum ClientSideCommand : uint8_t {
  eCCMD_None,
  eCCMD_LobbyIsFull,
  eCCMD_PasscodeIncorrect,
  eCCMD_MaxLobbiesReached,
  eCCMD_LobbyChanged,
  eCCMD_Passcode,
};

const std::string validAscii = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

std::string generatePasscode() {
  std::string ret{"000000"};
  
  ret[0] = validAscii[rand() % validAscii.length()];
  ret[1] = validAscii[rand() % validAscii.length()];
  ret[2] = validAscii[rand() % validAscii.length()];
  ret[3] = validAscii[rand() % validAscii.length()];
  ret[4] = validAscii[rand() % validAscii.length()];
  ret[5] = validAscii[rand() % validAscii.length()];

  return ret;
}

template <typename ...Args>
void SendPacket(ArenaBuffer& wb, ENetPeer* dest, Args... args) {
  wb.Reset();
  wb.Write(args...);
  ENetPacket *packet = enet_packet_create(wb.GetBuffer(), wb.GetSize(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(dest, 0, packet);
}

template <typename ...Args>
void SendPacket(ArenaBuffer& wb, std::vector<ENetPeer*> dests, Args... args) {
  wb.Reset();
  wb.Write(args...);
  ENetPacket *packet = enet_packet_create(wb.GetBuffer(), wb.GetSize(), ENET_PACKET_FLAG_RELIABLE);
  for(auto dest : dests) {
    enet_peer_send(dest, 0, packet);
  }
}

int main() {
  std::unordered_map<const char*, std::vector<ENetPeer*>> lobbies{};
  ArenaBuffer wb;

  if (enet_initialize() != 0) {
    printf("An error occurred while initializing ENet.\n");
    return 1;
  }

  srand(time(nullptr));

  ENetAddress hostAddress = {};
  hostAddress.host = ENET_HOST_ANY;
  hostAddress.port = 7788;
  ENetHost* host = enet_host_create(&hostAddress, 4, 2, 0, 0);

  auto disconnectPeer = [&wb, &lobbies](const char* passcode, ENetPeer* peerToDisconnect) {
    auto& thisLobbyPeers = lobbies[passcode];
    thisLobbyPeers.erase(
      std::remove_if(thisLobbyPeers.begin(), thisLobbyPeers.end(), [&peerToDisconnect](ENetPeer* peer) {
        return peerToDisconnect->connectID == peer->connectID;
      }),
      thisLobbyPeers.end());

    SendPacket(wb, thisLobbyPeers, eCCMD_LobbyChanged, thisLobbyPeers);
  };

  printf("Listening on port %d\n", hostAddress.port);
  while(true) {
    ENetEvent evt = {};
    while(enet_host_service(host, &evt, 10) > 0) {
      switch(evt.type) {
        case ENET_EVENT_TYPE_CONNECT: break;
        case ENET_EVENT_TYPE_RECEIVE: {
          ArenaReadBuffer b{(const char*)evt.packet->data, evt.packet->dataLength};
          auto command = b.Read<uint8_t>();
          switch(command) {
            case eSCMD_JoinLobby: {
              std::string passcode = b.Read<std::string>();
	            printf("Client is attempting to join with this passcode: %s\n", passcode.c_str());
              if(lobbies.find(passcode.c_str()) != lobbies.end()) {
                if(lobbies[passcode.c_str()].size() >= 4) {
                  SendPacket(wb, evt.peer, eCCMD_LobbyIsFull, "Dummy");
                } else {
                  lobbies[passcode.c_str()].push_back(evt.peer);
                }
              } else {
                SendPacket(wb, evt.peer, eCCMD_PasscodeIncorrect, "Oopsie :3");
              }
            } break;
            case eSCMD_CreateLobby: {
              if(lobbies.size() >= MAX_LOBBIES) {
                SendPacket(wb, evt.peer, eCCMD_MaxLobbiesReached, "Sorry :(");
                break;
              }
              auto passcode = generatePasscode();
	            printf("Generated passcode: %s\n", passcode.c_str());
              while(lobbies.find(passcode.c_str()) != lobbies.end()) {
                // regenerate
                passcode = generatePasscode();
              }
              lobbies[passcode.c_str()] = {evt.peer};
              SendPacket(wb, evt.peer, eCCMD_Passcode);
              SendPacket(wb, evt.peer, passcode);
            } break;
            case eSCMD_None:
              break;
          }
        } break;
        case ENET_EVENT_TYPE_DISCONNECT:
          disconnectPeer((char*)evt.packet->data, evt.peer);
          break;
        case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: // we need to find the timedout peer ourselves
          for (auto [passcode, peers]: lobbies) {
            for (auto peer : peers) {
              if (peer->connectID == evt.peer->connectID) {
                disconnectPeer(passcode, peer);
              }
            }
          }
          break;
        default: break;
      }
    }
  }

  enet_host_destroy(host);
  enet_deinitialize();
  return 0;
}
