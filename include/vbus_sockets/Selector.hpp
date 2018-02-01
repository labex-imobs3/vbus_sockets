#pragma once

#include <sys/select.h>
#include <chrono>

#include <vbus_sockets/NonCopyable.hpp>

namespace vbus_sockets {

class Descriptor;

class Selector: private NonCopyable
{
public:
  Selector();

  Selector & add_reader(const Descriptor &);
  Selector & add_writer(const Descriptor &);
  Selector & add_timeout(std::chrono::microseconds);

  unsigned operator()();

  bool is_readable(const Descriptor &d) const;
  bool is_writable(const Descriptor &d) const;

private:
  void check_used() const;
  void check_unused() const;

  bool used_;
  int max_;
  fd_set read_;
  fd_set write_;
  bool use_timeout_;
  timeval tv_timeout_;
};

}

