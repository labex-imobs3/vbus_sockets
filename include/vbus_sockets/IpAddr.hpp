#pragma once

#include <cinttypes>
#include <string>

namespace vbus_sockets {

class IpAddr
{
public:
  static const IpAddr any;
  static const IpAddr loopback;
  static const IpAddr broadcast;
  static const IpAddr none;

  IpAddr();
  explicit IpAddr(uint32_t);
  explicit IpAddr(const std::string &);
  explicit IpAddr(const char *);

  uint32_t value() const { return value_; }
  std::string to_string() const;

protected:
  uint32_t value_;
};

}

