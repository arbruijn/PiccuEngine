#pragma once

#include <cstddef>
#include <cstdio>
#include <cstring>
namespace tinyprintf {

enum class format_result {
  ok,
  write_error,
  invalid_format,
};

// Expected interface for ArgSource:
//   const char* next_string();
//   int next_int();
//   unsigned next_hex();
//   char next_char();
//   const void* next_pointer();
//
// The writer must be callable as:
//   bool write(const char* data, std::size_t length);
//
// Returning false from the writer aborts formatting with write_error.
template <typename ArgSource, typename Writer>
format_result vformat(const char* fmt, ArgSource& args, Writer&& write, void *write_data) {
  if (!fmt) {
    return format_result::invalid_format;
  }

  const char* p = fmt;
  const char* end = fmt + std::strlen(fmt);
  std::size_t remaining = static_cast<std::size_t>(end - p);
  while (*p != '\0') {
    const char* percent = static_cast<const char*>(std::memchr(p, '%', remaining));
    if (!percent) {
      if (remaining != 0 && !write(write_data, p, remaining)) {
        return format_result::write_error;
      }
      break;
    }

    if (percent > p) {
      const std::size_t len = static_cast<std::size_t>(percent - p);
      if (!write(write_data, p, len)) {
        return format_result::write_error;
      }
    }

    const char spec = percent[1];
    if (spec == '\0') {
      return format_result::invalid_format;
    }

    if (spec == '%') {
      if (!write(write_data, "%", 1)) {
        return format_result::write_error;
      }
      p = percent + 2;
      remaining = static_cast<std::size_t>(end - p);
      continue;
    }

    char buffer[64];
    std::size_t out_len = 0;

    switch (spec) {
      case 's': {
        const char* s = args.next_string();
        if (!s) {
          s = "(null)";
        }
        out_len = std::strlen(s);
        if (!write(write_data, s, out_len)) {
          return format_result::write_error;
        }
        p = percent + 2;
        remaining = static_cast<std::size_t>(end - p);
        continue;
      }
      case 'd': {
        const int value = args.next_int();
        const int written = std::snprintf(buffer, sizeof(buffer), "%d", value);
        if (written < 0) {
          return format_result::invalid_format;
        }
        out_len = static_cast<std::size_t>(written);
        if (out_len >= sizeof(buffer)) {
          return format_result::invalid_format;
        }
        break;
      }
      case 'x': {
        const unsigned value = args.next_hex();
        const int written = std::snprintf(buffer, sizeof(buffer), "%x", value);
        if (written < 0) {
          return format_result::invalid_format;
        }
        out_len = static_cast<std::size_t>(written);
        if (out_len >= sizeof(buffer)) {
          return format_result::invalid_format;
        }
        break;
      }
      case 'c': {
        const char value = args.next_char();
        buffer[0] = value;
        out_len = 1;
        break;
      }
      case 'p': {
        const void* value = args.next_pointer();
        const int written = std::snprintf(buffer, sizeof(buffer), "%p", value);
        if (written < 0) {
          return format_result::invalid_format;
        }
        out_len = static_cast<std::size_t>(written);
        if (out_len >= sizeof(buffer)) {
          return format_result::invalid_format;
        }
        break;
      }
      default:
        return format_result::invalid_format;
    }

    if (!write(write_data, buffer, out_len)) {
      return format_result::write_error;
    }
    p = percent + 2;
    remaining = static_cast<std::size_t>(end - p);
  }

  return format_result::ok;
}

}  // namespace tinyprintf
