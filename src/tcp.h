#pragma once
#include "types.h"

int send_tcp_down(struct nw_layer *self, struct pkt *data);
int receive_tcp_up(struct nw_layer *self, struct pkt *data);