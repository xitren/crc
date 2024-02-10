#pragma once

#include <xitren/crc_concept.hpp>

namespace xitren::crc {

class crc16 {
    static constexpr uint16_t polynome = 0xA001;
    static constexpr std::array<std::uint16_t, 256>
    prepare_table(uint16_t const poly)
    {
        std::uint16_t const            table[2] = {0x0000, poly};
        std::array<std::uint16_t, 256> array{};
        int                            i = 0;
        for (std::uint16_t& item : array) {
            std::uint16_t crc_mb = i++;
            for (char bit = 0; bit < 8; bit++) {
                std::uint8_t xort = crc_mb & 0x01;
                crc_mb >>= 1;
                crc_mb ^= table[xort];
            }
            item = crc_mb;
        }
        return array;
    }

public:
    using value_type = std::uint16_t;

    template <crc_iterator InputIterator>
    static constexpr value_type
    calculate(InputIterator begin, InputIterator end)
    {
        static_assert(sizeof(*begin) == 1);
        constexpr auto crc16_tbl = crc16::prepare_table(polynome);
        value_type     crc_mb{0xFFFF};
        for (std::input_iterator auto byte = begin; byte < end; byte++)
            crc_mb = (crc_mb >> 8) ^ crc16_tbl[(crc_mb & 0xFF) ^ (*byte)];
        return crc_mb;
    }
};
}    // namespace xitren::crc
