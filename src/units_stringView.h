#pragma once
#ifndef STRING_VIEW_H
#define STRING_VIEW_H
#include <cstddef>
#include <cstdlib>
#include <cstring>

#ifdef USE_CHAR_TRAITS
#include <bits/char_traits.h>
#else
#include <string>
#endif

class StringView {
    static inline constexpr const char* empty = "";

public:
    const char* data;
    size_t length;

    constexpr StringView(const char* d, size_t len) : data(d), length(len) {
    }

    constexpr StringView() : data(empty), length(0) {
    }
    constexpr StringView(const StringView&) = default;
    constexpr StringView(StringView&&) noexcept = default;
    constexpr StringView& operator=(const StringView&) = default;
    constexpr StringView& operator=(StringView&&) noexcept = default;
    constexpr const char* begin() const {
        return data;
    }
    constexpr const char* end() const {
        return data + length;
    }
    constexpr char operator[](size_t i) const noexcept {
        return data[i];
    }
    constexpr size_t size() const noexcept {
        return length;
    }
    constexpr StringView removePrefix(size_t len) {
        if (len <= length) {
            return StringView(data + len, length - len);
        }
        return StringView();
    }
    constexpr StringView removeSuffix(size_t len) {
        if (len <= length) {
            return StringView(data, length - len);
        }
        return StringView();
    }

    constexpr static StringView fromRawString(const char* str) {
        return StringView(str, std::char_traits<char>::length(str));
    }
    constexpr static StringView fromRawString(StringView v) {
        return v;
    }
};

#endif // STRING_VIEW_H
