#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <unistd.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nf_tables.h>

const long SOCKET_BUFFER_SIZE = getpagesize() < 8192 ? getpagesize() : 8192;

int main()
{
  int fd = socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_NETFILTER);
  sockaddr_nl addr;
  addr.nl_family = AF_NETLINK;
  addr.nl_pid = 0;
  char buf[SOCKET_BUFFER_SIZE];
  if(fd < 0)
  {
    return -1;
  }
  if(bind(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
  {
    close(fd);
    return -2;
  }
  nlmsghdr* hdr = reinterpret_cast<nlmsghdr*>(buf);
  hdr->nlmsg_len = NLMSG_ALIGN(sizeof(nlmsghdr));
  hdr->nlmsg_type = (NFNL_SUBSYS_NFTABLES << 8) | NFT_MSG_NEWTABLE;

  close(fd);
}
