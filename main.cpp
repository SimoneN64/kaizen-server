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

void SendPacket(ArenaBuffer& wb, ENetPeer* dest, const std::string& s) {
  wb.Reset();
  wb.Write(s);
  ENetPacket* packet = enet_packet_create(wb.GetBuffer(), wb.GetSize(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(dest, 0, packet);
}

template <typename T>
void SendPacket(ArenaBuffer& wb, ENetPeer* dest, const T& data) {
  wb.Reset();
  wb.Write(data);
  ENetPacket* packet = enet_packet_create(wb.GetBuffer(), wb.GetSize(), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(dest, 0, packet);
}

template <typename T>
void SendPacket(ArenaBuffer& wb, const std::vector<ENetPeer*> &dests, const std::vector<T>& data) {
  wb.Reset();
  wb.Write(data);
  ENetPacket *packet = enet_packet_create(wb.GetBuffer(), wb.GetSize(), ENET_PACKET_FLAG_RELIABLE);
  for(auto dest : dests) {
    enet_peer_send(dest, 0, packet);
  }
}

template <typename T>
void SendPacket(ArenaBuffer& wb, const std::vector<ENetPeer*>& dests, const T& data) {
  wb.Reset();
  wb.Write(data);
  ENetPacket* packet = enet_packet_create(wb.GetBuffer(), wb.GetSize(), ENET_PACKET_FLAG_RELIABLE);
  for (auto dest : dests) {
    enet_peer_send(dest, 0, packet);
  }
}

template <typename T>
std::vector<uint8_t> CreatePacketWithData(const uint8_t &cmd, const std::vector<T>& second) {
  std::vector<uint8_t> ret{};
  ret.resize(sizeof(T)*second.size() + 1);
  ret[0] = cmd;
  memcpy(ret.data() + 1, second.data(), sizeof(T)*second.size());

  return ret;
}

std::vector<uint8_t> CreatePacketWithData(const uint8_t &cmd, const std::string& second) {
  std::vector<uint8_t> ret{};
  ret.resize(second.length() + 1);
  ret[0] = cmd;
  memcpy(ret.data() + 1, second.data(), second.length());

  return ret;
}

int main() {
  std::unordered_map<const char*, std::vector<ENetPeer>> lobbies{};
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

  auto disconnectPeer = [&wb, &lobbies](const char* passcode, uint32_t peerToDisconnect) {
    auto& thisLobbyPeers = lobbies[passcode];
    thisLobbyPeers.erase(
      std::remove_if(thisLobbyPeers.begin(), thisLobbyPeers.end(), [&peerToDisconnect](ENetPeer peer) {
        return peerToDisconnect == peer.connectID;
      }),
      thisLobbyPeers.end());

    auto fullPacket = CreatePacketWithData(eCCMD_LobbyChanged, thisLobbyPeers);
    //SendPacket(wb, thisLobbyPeers, fullPacket);
  };

  printf("Listening on port %d\n", hostAddress.port);
  while(true) {
    ENetEvent evt = {};
    while(enet_host_service(host, &evt, 10) > 0) {
      switch(evt.type) {
        case ENET_EVENT_TYPE_CONNECT: break;
        case ENET_EVENT_TYPE_RECEIVE: {
          ArenaReadBuffer b{evt.packet->data, evt.packet->dataLength};
          auto command = b.Read<uint8_t>();
          switch(command) {
            case eSCMD_JoinLobby: {
              auto passcode = b.Read<std::string>();
	            printf("Client is attempting to join with this passcode: %s\n", passcode.c_str());
              if(lobbies.find(passcode.c_str()) != lobbies.end()) {
                if(lobbies[passcode.c_str()].size() >= 4) {
                  SendPacket(wb, evt.peer, eCCMD_LobbyIsFull);
                } else {
                  lobbies[passcode.c_str()].push_back(*evt.peer);
                }
              } else {
                SendPacket(wb, evt.peer, eCCMD_PasscodeIncorrect);
              }
            } break;
            case eSCMD_CreateLobby: {
              if(lobbies.size() >= MAX_LOBBIES) {
                SendPacket(wb, evt.peer, eCCMD_MaxLobbiesReached);
                break;
              }
              auto passcode = generatePasscode();
	            printf("Generated passcode: %s\n", passcode.c_str());
              while(lobbies.find(passcode.c_str()) != lobbies.end()) {
                // regenerate
                passcode = generatePasscode();
              }
              lobbies[passcode.c_str()] = {*evt.peer};
              auto fullPacket = CreatePacketWithData(eCCMD_Passcode, passcode);
              SendPacket(wb, evt.peer, fullPacket);
            } break;
            case eSCMD_None:
              break;
          }
        } break;
        case ENET_EVENT_TYPE_DISCONNECT:
          disconnectPeer((char*)evt.packet->data, evt.peer->connectID);
          break;
        case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT: // we need to find the timedout peer ourselves
          for (const auto& [passcode, peers]: lobbies) {
            for (auto peer : peers) {
              if (peer.connectID == evt.peer->connectID) {
                disconnectPeer(passcode, peer.connectID);
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
