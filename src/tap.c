#include "tap.h"

int start_listening(int fd, struct nw_layer *tap)
{
    for (;;)
    {
        unsigned char *buffer = malloc(MAX_ETH_FRAME_SIZE);
        if (buffer == NULL)
        {
            perror("Allocating buffer for TAP read");
            close(fd);
            return -1;
        }
        
        ssize_t nread = read(fd, buffer, MAX_ETH_FRAME_SIZE);
        if (nread < 0)
        {
            perror("Reading from TAP interface");
            close(fd);
            free(buffer);
            return -1;
        }

        struct pkt *packet = malloc(sizeof(struct pkt));
        if (packet == NULL)
        {
            perror("Allocating packet structure");
            close(fd);
            free(buffer);
            return -1;
        }
        
        *packet = (struct pkt){
            .data = buffer,
            .len = (size_t)nread,
            .offset = 0,
            .metadata = malloc(sizeof(struct pkt_metadata))
        };

        if (packet->metadata == NULL)
        {
            perror("Allocating packet metadata");
            close(fd);
            free(buffer);
            free(packet);
            return -1;
        }
        
        int result = tap->rcv_up(tap, packet);
        free(buffer);
        free(packet->metadata);
        free(packet);
    }
}

pkt_result send_up_to_ethernet(struct nw_layer *tap, struct pkt *data)
{
    return tap->ups[0]->rcv_up(tap->ups[0], data);
}

pkt_result write_to_tap(struct nw_layer *tap, struct pkt *data)
{
    struct tap_context *tap_ctx = (struct tap_context *)tap->context;
    int fd = tap_ctx->fd;
    ssize_t nwrite = write(fd, data->data, data->len);

    if (nwrite < 0)
    {
        perror("Writing to TAP interface");
        close(fd);
        return REPLY_WRITE_ERROR;
    }
    return REPLY_SENT;
}