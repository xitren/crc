#pragma once

#include <xitren/crc_concept.hpp>

namespace xitren::crc {

/*!
\brief CRC8
CRC-8-CCITT
x^8+x^2+x+1 (ATM HEC), ISDN Header Error Control and Cell Delineation ITU-T I.432.1 (02/99)
0x07 / 0xE0 / 0x83
*/
class crc8 {
    static constexpr uint16_t polynome = 0x07;
    /*!
    Calculate table for speed up crc computation
    \param[in] poly Polynome initial value (usually equals polynome = 0x07)
    \return crc table in array
    */
    static constexpr auto
    prepare_table(std::uint8_t const poly)
    {
        std::uint8_t const            table[2] = {0x0U, poly};
        std::array<std::uint8_t, 256> array{};
        std::uint16_t                 i = 0;
        for (std::uint8_t& item : array) {
            auto crc_mb = static_cast<std::uint8_t>(i++);
            for (char bit = 0; bit < 8; bit++) {
                std::uint8_t xort = (crc_mb & 0x80) >> 7;
                crc_mb <<= 1;
                crc_mb ^= table[xort];
            }
            item = crc_mb;
        }
        return array;
    }

public:
    using value_type = std::uint8_t;

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
        static_assert(sizeof(*begin) == sizeof(std::uint8_t));
        constexpr auto crc8_tbl = crc8::prepare_table(polynome);
        value_type     crc_mb{0x0U};
        for (std::input_iterator auto byte = begin; byte != end; byte++) {
            crc_mb = crc8_tbl[crc_mb ^ (*byte)];
        }
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
        constexpr auto crc8_tbl = crc8::prepare_table(polynome);
        value_type     crc_mb{0x0U};
        for (auto& byte : data) {
            crc_mb = crc8_tbl[crc_mb ^ (byte)];
        }
        return crc_mb;
    }
};

}    // namespace xitren::crc
