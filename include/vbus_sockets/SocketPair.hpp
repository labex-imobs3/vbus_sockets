#pragma once

#include <vbus_sockets/NonCopyable.hpp>
#include <vbus_sockets/Descriptor.hpp>

namespace vbus_sockets {

class SocketPair: public NonCopyable
{
public:
  enum class Type
  {
    Unidirectional,
    Bidirectional
  };

  explicit SocketPair(Type = Type::Unidirectional);

  Descriptor first_;
  Descriptor second_;
};

}

