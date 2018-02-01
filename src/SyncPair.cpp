#include <stdexcept>
#include <cinttypes>

#include <vbus_sockets/SyncPair.hpp>
#include <vbus_sockets/SocketPair.hpp>

using namespace vbus_sockets;

SyncPair::SyncPair():
  Descriptor(),
  wdesc_()
{
  SocketPair sp(SocketPair::Type::Unidirectional);

  wdesc_ = std::move(sp.first_);
  Descriptor::operator=(std::move(sp.second_));
}

void SyncPair::signal() const
{
  static const std::uint8_t dummy = 0;
  if (wdesc_.write(&dummy, 1) != 1)
    throw std::runtime_error("write() failed");
}

