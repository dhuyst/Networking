#include "ethernet.h"
#include <fcntl.h>
#include <linux/if_tun.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int get_tap(char *name, int flags);
int activate_tap(char* name);

int main()
{
    char name[IFNAMSIZ] = "tap0";
    int tap_fd = get_tap(name, IFF_TAP | IFF_NO_PI);
    if (activate_tap(name) < 0)
    {
        perror("Activating TAP interface");
        close(tap_fd);
        return 1;
    }

    for (;;)
    {
        struct ethernet_frame buffer;
        ssize_t nread = read(tap_fd, &buffer, sizeof(buffer));
        if (nread < 0)
        {
            perror("Reading from TAP interface");
            close(tap_fd);
            return 1;
        }
        
        printf("Read %zd bytes from %s: ", nread, name);
        printf("Source Mac: %02x:%02x:%02x:%02x:%02x:%02x, ",
               buffer.header.src_mac[0], buffer.header.src_mac[1],
               buffer.header.src_mac[2], buffer.header.src_mac[3],
               buffer.header.src_mac[4], buffer.header.src_mac[5]);
        printf("Destination Mac: %02x:%02x:%02x:%02x:%02x:%02x\n",
               buffer.header.dest_mac[0], buffer.header.dest_mac[1],
               buffer.header.dest_mac[2], buffer.header.dest_mac[3],
               buffer.header.dest_mac[4], buffer.header.dest_mac[5]);
    }

    return 0;
}

int activate_tap(char* name)
{
    int sock;
    struct ifreq ifr;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        return -1;
    }

    memset(&ifr, 0, sizeof(ifr));
    strcpy(ifr.ifr_name, name);

    int current_flags = ioctl(sock, SIOCGIFFLAGS, &ifr);
    if (current_flags < 0)
    {
        perror("ioctl SIOCGIFFLAGS");
        return -1;
    }

    ifr.ifr_flags = current_flags | (IFF_UP | IFF_RUNNING);
    if (ioctl(sock, SIOCSIFFLAGS, &ifr) < 0)
    {
        perror("ioctl SIOCSIFFLAGS");
        return -1;
    }
    return 0;
}

int get_tap(char *name, int flags)
{
    struct ifreq ifr;
    int fd, error;

    if ((fd = open("/dev/net/tun", O_RDWR)) < 0)
    {
        perror("open /dev/net/tun");
        return fd;
    }
    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = flags;

    if (name)
        strncpy(ifr.ifr_name, name, IFNAMSIZ);

    if ((error = ioctl(fd, TUNSETIFF, &ifr)) < 0)
    {
        perror("ioctl TUNSETIFF");
        close(fd);
        return error;
    }
    strcpy(name, ifr.ifr_name);
    return fd;
}