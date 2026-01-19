#pragma once
#include "types.h"

pkt_result receive_ipv4_up(struct nw_layer *self, struct pkt *data);
pkt_result send_ipv4_down(struct nw_layer *self, struct pkt *data);