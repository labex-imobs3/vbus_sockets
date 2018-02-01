#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <utility>
#include <stdexcept>

#include <vbus_sockets/Socket.hpp>

using namespace vbus_sockets;

namespace
{
  int type_to_int(Socket::Type t)
  {
    switch (t)
    {
    case Socket::Type::UDP:
      return SOCK_DGRAM;

    case Socket::Type::TCP:
      return SOCK_STREAM;

    default:
      throw std::logic_error("Bad socket type");
    }
  }
}

Socket::Socket():
  Descriptor()
{
}

Socket::Socket(Type t):
  Descriptor(::socket(AF_INET, type_to_int(t), 0), Mode::ReadWrite, "Failed to create socket")
{
}

Socket::Socket(int s):
  Descriptor(s, Mode::ReadWrite, "Bad socket")
{
}

Socket::Socket(Socket &&s):
  Descriptor(std::move(s))
{
}

Socket & Socket::operator=(Socket &&s)
{
  Descriptor::operator=(std::move(s));
  return (*this);
}

void Socket::enable_reuse_address() const
{
  static const int arg = 1;

  if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(int)) != 0)
    throw std::runtime_error("Failed to set SO_REUSEADDR");
}

void Socket::bind(IpAddr ip, uint16_t port) const
{
  check_enabled();

  ::sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(ip.value());

  if (::bind(fd_, reinterpret_cast<const struct sockaddr *>(&addr), sizeof(::sockaddr_in)) != 0)
    throw std::runtime_error(std::string("bind() to address ") + ip.to_string() + " failed on port " + std::to_string(port));
}

void Socket::listen(int backlog_size) const
{
  check_enabled();

  if (::listen(fd_, backlog_size) != 0)
    throw std::runtime_error("listen() failed");
}

Socket Socket::accept() const
{
  check_enabled();

  const int r = ::accept(fd_, nullptr, nullptr);
  if (r < 0)
    throw std::runtime_error("accept() failed");
  return Socket(r);
}

void Socket::connect(IpAddr ip, uint16_t port) const
{
  check_enabled();

  ::sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(ip.value());

  const int r = ::connect(fd_, reinterpret_cast<const struct sockaddr *>(&addr), sizeof(sockaddr_in));
  if (r < 0)
    throw std::runtime_error(std::string("connect() to address ") + ip.to_string() + " failed on port " + std::to_string(port));
}

size_t Socket::send(const void *buf, size_t size) const
{
  check_enabled();
  if (!is_writable())
    throw std::logic_error("Attempting to write to a non-writable socket");

  const ssize_t r = ::send(fd_, buf, size, MSG_NOSIGNAL);
  if (r < 0)
    throw std::runtime_error("send() failed");

  return r;
}

