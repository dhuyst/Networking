#pragma once
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

int receive_frame_up(struct nw_layer *self, struct pkt *packet);
int send_frame_down(struct nw_layer *self, struct pkt *packet);
void print_incoming(struct ethernet_header *header);
bool relevant_destination_mac(mac_address dest_mac, struct nw_layer *self);
int send_to_arp(struct nw_layer *self, struct pkt *packet);
int send_to_ipv4(struct nw_layer *self, struct pkt *packet);