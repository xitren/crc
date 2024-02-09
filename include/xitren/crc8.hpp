#pragma once

#include <xitren/crc_concept.hpp>

namespace xitren::crc {

template <std::input_iterator InputIterator>
std::uint8_t
crc8(InputIterator begin, InputIterator end)
{
    static constexpr std::uint8_t polynom{0x07};
    std::uint8_t                  crc{0x00};

    for (auto& byte = begin; byte != end; byte++) {
        crc ^= *byte;
        for (std::uint8_t i{}; i < 8; i++) {
            if ((crc & 0x80) != 0) {
                crc = crc << 1;
                crc ^= polynom;
            } else {
                crc = crc << 1;
            }
        }
    }

    return crc;
}

}    // namespace xitren::crc8
