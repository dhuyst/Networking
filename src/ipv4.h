#pragma once
#include "types.h"

int receive_ipv4_up(struct nw_layer *self, struct pkt *data);
int send_ipv4_down(struct nw_layer *self, struct pkt *data);