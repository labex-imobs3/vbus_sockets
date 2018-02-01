#pragma once

#include <vbus_sockets/Descriptor.hpp>
#include <vbus_sockets/IpAddr.hpp>

namespace vbus_sockets {

class Socket: public Descriptor
{
public:
  enum class Type
  {
    TCP,
    UDP
  };

  Socket();
  explicit Socket(Type);
  explicit Socket(int);

  Socket(Socket &&);

  Socket & operator=(Socket &&);

  void enable_reuse_address() const;

  void bind(IpAddr, uint16_t) const;
  void listen(int) const;
  Socket accept() const;

  void connect(IpAddr, uint16_t) const;

  size_t send(const void *, size_t) const;
};

}

