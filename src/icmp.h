#pragma once
#include "types.h"

pkt_result send_icmp_down(struct nw_layer *self, struct pkt *data);
pkt_result receive_icmp_up(struct nw_layer *self, struct pkt *data);