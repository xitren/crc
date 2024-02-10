#pragma once

#include <array>
#include <concepts>
#include <cstdint>
#include <cstring>

namespace xitren::crc {

template <typename T>
concept crc_concept = requires {
    typename T::value_type;
    T::calculate(std::array<std::uint8_t, 8>::iterator{nullptr},
                 std::array<std::uint8_t, 8>::iterator{reinterpret_cast<unsigned char*>(10)});
};

template <typename T>
concept crc_iterator = std::input_iterator<T>;

}    // namespace xitren::crc
