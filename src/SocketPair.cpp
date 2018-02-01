#include <stdexcept>
#include <cinttypes>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <vbus_sockets/SocketPair.hpp>

using namespace vbus_sockets;

namespace
{
  constexpr Descriptor::Mode first_mode(SocketPair::Type t)
  {
    return (t == SocketPair::Type::Unidirectional ?
      Descriptor::Mode::WriteOnly : Descriptor::Mode::ReadWrite);
  }

  constexpr Descriptor::Mode second_mode(SocketPair::Type t)
  {
    return (t == SocketPair::Type::Unidirectional ?
      Descriptor::Mode::ReadOnly : Descriptor::Mode::ReadWrite);
  }
}

SocketPair::SocketPair(Type t):
  first_(),
  second_()
{
  int fd[2] = { -1, -1 };
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) != 0)
    throw std::runtime_error("socketpair() failed");

  try
  {
    first_ = Descriptor(fd[0], first_mode(t));
  }
  catch (...)
  {
    ::close(fd[0]);
    ::close(fd[1]);
    throw;
  }

  try
  {
    second_ = Descriptor(fd[1], second_mode(t));
  }
  catch (...)
  {
    ::close(fd[1]);
    throw;
  }
}

