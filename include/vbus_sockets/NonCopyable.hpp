#pragma once

namespace vbus_sockets {

/**
 * Base class for everything that should not be copied,
 * either via operator= or copy/move constructor.
 */
struct NonCopyable
{
  constexpr NonCopyable() {}

  NonCopyable(const NonCopyable &) = delete;
  NonCopyable(NonCopyable &&) = delete;
  NonCopyable & operator=(const NonCopyable &) = delete;
  NonCopyable & operator=(NonCopyable &&) = delete;
};

}

