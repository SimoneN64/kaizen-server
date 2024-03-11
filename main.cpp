#define ENET_IMPLEMENTATION
#include <enet.h>
#include <cstdio>

int main() {
  if (enet_initialize() != 0) {
    printf("An error occurred while initializing ENet.\n");
    return 1;
  }

  enet_deinitialize();
  return 0;
}