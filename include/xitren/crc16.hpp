#pragma once

#include <xitren/crc_concept.hpp>

namespace xitren::crc {

/*!
\brief CRC16
CRC-16-IBM
x^16+x^15+x^2+1 (Bisync, Modbus, USB, ANSI X3.28[20])
0x8005 / 0xA001 / 0xC002
*/
class crc16 {
    static constexpr uint16_t polynome = 0xA001;
    /*!
    Calculate table for speed up crc computation
    \param[in] poly Polynome initial value (usually equals polynome = 0xA001)
    \return crc table in array
    */
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

    /*!
    Calculate crc on data by input iterator
    \param[in] begin Begin iterator of byte data
    \param[in] end End iterator of byte data
    \return crc value
    */
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

    /*!
    Calculate crc on data array
    \param[in] data Array
    \return crc value
    */
    template <std::size_t Size>
    static consteval value_type
    calculate(std::array<std::uint8_t, Size> const& data) noexcept
    {
        auto       crc16ansi_tbl = crc16::prepare_table(polynome);
        value_type crc_mb{0xFFFF};
        for (auto& byte : data)
            crc_mb = (crc_mb >> 8) ^ crc16ansi_tbl[(crc_mb & 0xFF) ^ (byte)];
        return crc_mb;
    }
};
}    // namespace xitren::crc
