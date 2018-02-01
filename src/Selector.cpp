#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <error.h>

#include <vbus_sockets/Descriptor.hpp>
#include <vbus_sockets/Selector.hpp>

using namespace vbus_sockets;

namespace
{
  void check_fd(int fd)
  {
    if (fd >= FD_SETSIZE)
      throw std::runtime_error("File descriptor is too high for select()");
    if (fd < 0)
      throw std::runtime_error("Selector does not work with negative file descriptors");
  }
}

Selector::Selector():
  used_(false),
  max_(0),
  read_(),
  write_(),
  use_timeout_(false),
  tv_timeout_()
{
  FD_ZERO(&read_);
  FD_ZERO(&write_);
}

Selector & Selector::add_reader(const Descriptor &d)
{
  if (!d.is_readable())
    throw std::logic_error("Can not select() for reading non-readable descriptor");

  const int fd = int(d);
  check_fd(fd);
  FD_SET(fd, &read_);
  max_ = std::max(max_, fd);

  return *this;
}

Selector & Selector::add_writer(const Descriptor &d)
{
  if (!d.is_writable())
    throw std::logic_error("Can not select() for writing non-writable descriptor");

  const int fd = int(d);
  check_fd(fd);
  FD_SET(fd, &write_);
  max_ = std::max(max_, fd);

  return *this;
}

Selector & Selector::add_timeout(std::chrono::microseconds dur)
{
  tv_timeout_.tv_sec = dur.count() / 1000000;
  tv_timeout_.tv_usec = dur.count() % 1000000;
  use_timeout_ = true;

  return *this;
}

unsigned Selector::operator()()
{
  check_unused();

  while (true)
  {
    const int r = ::select(max_ + 1, &read_, &write_, nullptr, use_timeout_ ? &tv_timeout_ : nullptr);

    if (r < 0)
    {
      // If we've been interrupted by a signal, restart the system call.
      // Linuxism: when select() returns, the timeout has been updated with the remaining time
      // -> we can call select() again with the same timeout structure
      if (errno == EINTR)
        continue;
      else
        throw std::runtime_error(std::string("select() failed: ") + strerror(errno));
    }

    used_ = true;
    return unsigned(r);
  }
}

bool Selector::is_readable(const Descriptor &d) const
{
  check_used();
  const int fd = int(d);
  check_fd(fd);
  return FD_ISSET(fd, &read_);

}

bool Selector::is_writable(const Descriptor &d) const
{
  check_used();
  const int fd = int(d);
  check_fd(fd);
  return FD_ISSET(fd, &write_);
}

void Selector::check_used() const
{
  assert(used_);
  if (!used_)
    throw std::logic_error("This selector has not been used yet");
}

void Selector::check_unused() const
{
  assert(!used_);
  if (used_)
    throw std::logic_error("This selector has already been used");
}

