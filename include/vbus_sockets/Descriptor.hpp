#pragma once

#include <sys/ioctl.h>

#include <string>

#include <vbus_sockets/NonCopyable.hpp>

namespace vbus_sockets {

class Descriptor: public NonCopyable
{
public:
  enum class Mode
  {
    ReadOnly,
    WriteOnly,
    ReadWrite
  };

public:
  Descriptor();
  Descriptor(int, Mode);
  Descriptor(int, Mode, const std::string &);
  Descriptor(const std::string &, Mode);
  Descriptor(Descriptor &&);
  ~Descriptor();

  Descriptor & operator=(Descriptor &&);

  operator int() const { return fd_; }

  void close();
  size_t read(void *, size_t) const;
  size_t write(const void *, size_t) const;

  template<typename... Args>
  int ioctl(Args &&... args) const { check_enabled(); return ::ioctl(fd_, std::forward<Args>(args)...); }

  bool is_readable() const { return (mode_ != Mode::WriteOnly); }
  bool is_writable() const { return (mode_ != Mode::ReadOnly); }
  bool is_enabled() const { return (fd_ >= 0); }

  void release() { fd_ = -1; } // use with caution

protected:
  void check_enabled() const;

  Mode mode_;
  int fd_;
};

}

