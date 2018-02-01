#include <sys/types.h>
#include <sys/socket.h>

#include <string>

#include <vbus_sockets/TcpAcceptor.hpp>
#include <vbus_sockets/Selector.hpp>

using namespace vbus_sockets;

TcpAcceptor::TcpAcceptor(IpAddr ip, uint16_t port, int backlog_size, const Handler &handler):
  handler_(handler),
  sp_(),
  socket_(Socket::Type::TCP),
  thread_()
{
  socket_.enable_reuse_address();
  socket_.bind(ip, port);
  socket_.listen(backlog_size);
  thread_ = std::thread(std::bind(&TcpAcceptor::thread_main, this));
}

TcpAcceptor::~TcpAcceptor()
{
  if (!thread_.joinable())
    return;

  sp_.signal();
  thread_.join();
}

void TcpAcceptor::thread_main()
{
  while (true)
  {
    Selector s;
    s.add_reader(sp_);
    s.add_reader(socket_);
    s();

    if (s.is_readable(sp_))
      return;

    if (s.is_readable(socket_))
    {
      try
      {
        handler_(socket_.accept());
      }
      catch (...)
      {
      }
    }
  }
}

