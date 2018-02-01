#pragma once

#include <cinttypes>
#include <thread>
#include <functional>

#include <vbus_sockets/NonCopyable.hpp>
#include <vbus_sockets/SyncPair.hpp>
#include <vbus_sockets/Socket.hpp>
#include <vbus_sockets/IpAddr.hpp>

namespace vbus_sockets {

class TcpAcceptor: private NonCopyable
{
public:
  typedef std::function<void(Socket &&)> Handler;

  explicit TcpAcceptor(IpAddr, uint16_t, int, const Handler &);
  ~TcpAcceptor();

private:
  void thread_main();

  const Handler handler_;
  const SyncPair sp_;
  const Socket socket_;
  std::thread thread_;
};

}

