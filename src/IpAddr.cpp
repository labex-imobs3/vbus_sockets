#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <stdexcept>

#include <vbus_sockets/IpAddr.hpp>

using namespace vbus_sockets;

const IpAddr IpAddr::any = IpAddr(INADDR_ANY);
const IpAddr IpAddr::loopback = IpAddr(INADDR_LOOPBACK);
const IpAddr IpAddr::broadcast = IpAddr(INADDR_BROADCAST);
const IpAddr IpAddr::none = IpAddr(INADDR_NONE);

IpAddr::IpAddr():
  value_(INADDR_NONE)
{
}

IpAddr::IpAddr(uint32_t value):
  value_(value)
{
}

IpAddr::IpAddr(const std::string &s):
  value_(INADDR_NONE)
{
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *res = nullptr;

  const int r = getaddrinfo(s.c_str(), nullptr, &hints, &res);
  if (r != 0)
    throw std::runtime_error(std::string("Could not resolve ") + s);

  for (const struct addrinfo *rp = res; rp != nullptr; rp = rp->ai_next) {
    if (rp->ai_family == AF_INET) {
      const sockaddr_in * const sin = reinterpret_cast<const sockaddr_in *>(rp->ai_addr);
      value_ = ntohl(sin->sin_addr.s_addr);
      freeaddrinfo(res);
      return;
    }
  }

  freeaddrinfo(res);
  throw std::runtime_error(std::string("Error resolving ") + s);
}

IpAddr::IpAddr(const char *s):
  IpAddr(std::string(s))
{
}

std::string IpAddr::to_string() const
{
  const unsigned u1 = (value_ >> 24);
  const unsigned u2 = (value_ >> 16) & 0xff;
  const unsigned u3 = (value_ >> 8) & 0xff;
  const unsigned u4 = value_ & 0xff;

  return (std::to_string(u1) + "." + std::to_string(u2) + "."
    + std::to_string(u3) + "." + std::to_string(u4));
}

