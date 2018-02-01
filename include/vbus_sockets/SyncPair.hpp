#pragma once

#include <vbus_sockets/Descriptor.hpp>

namespace vbus_sockets {

class SyncPair: public Descriptor
{
public:
  SyncPair();

  void signal() const;

private:
  Descriptor wdesc_;
};

}

