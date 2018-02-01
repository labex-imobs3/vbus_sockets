#include <sys/select.h>
#include <stdexcept>
#include <unistd.h>
#include <cassert>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <vbus_sockets/Descriptor.hpp>

using namespace vbus_sockets;

namespace
{
  int mode_to_int(Descriptor::Mode m)
  {
    switch (m)
    {
    case Descriptor::Mode::ReadOnly:
      return O_RDONLY;

    case Descriptor::Mode::WriteOnly:
      return O_WRONLY;

    case Descriptor::Mode::ReadWrite:
      return O_RDWR;

    default: // not reached, but keep the compiler happy
      assert(false);
      throw std::logic_error("NOT REACHED");
    }
  }

  int build_flags(Descriptor::Mode m)
  {
    return (mode_to_int(m) | O_NOCTTY | O_CLOEXEC);
  }

  constexpr ::mode_t create_mode = S_IROTH | S_IWOTH | S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR;
}

Descriptor::Descriptor():
  mode_(Mode::ReadWrite),
  fd_(-1)
{
}

Descriptor::Descriptor(int fd, Mode m):
  Descriptor(fd, m, "Can not build Descriptor with negative value")
{
}

Descriptor::Descriptor(int fd, Mode m, const std::string &err):
  mode_(m),
  fd_(fd)
{
  if (fd_ < 0)
    throw std::runtime_error(err);
}

Descriptor::Descriptor(const std::string &f, Mode m):
  Descriptor(::open(f.c_str(), build_flags(m), create_mode), m, std::string("Failed to open: ") + f)
{
}

Descriptor::Descriptor(Descriptor &&d):
  mode_(d.mode_),
  fd_(d.fd_)
{
  d.fd_ = -1; // disable other
}

Descriptor::~Descriptor()
{
  close();
}

Descriptor & Descriptor::operator=(Descriptor &&d)
{
  close();

  mode_ = d.mode_;
  fd_ = d.fd_;

  d.fd_ = -1; // disable other

  return (*this);
}

void Descriptor::close()
{
  if (fd_ >= 0)
  {
    ::close(fd_);
    fd_ = -1;
  }
}

size_t Descriptor::read(void *buf, size_t size) const
{
  check_enabled();
  if (!is_readable())
    throw std::logic_error("Attempting to read a non-readable descriptor");

  const ssize_t r = ::read(fd_, buf, size);
  if (r < 0)
    throw std::runtime_error("read() failed");

  return r;
}

size_t Descriptor::write(const void *buf, size_t size) const
{
  check_enabled();
  if (!is_writable())
    throw std::logic_error("Attempting to write to a non-writable descriptor");

  const ssize_t r = ::write(fd_, buf, size);
  if (r < 0)
    throw std::runtime_error("write() failed");

  return r;
}

void Descriptor::check_enabled() const
{
  if (!is_enabled())
    throw std::logic_error("Descriptor is disabled");
}

