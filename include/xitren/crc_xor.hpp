#pragma once

#include <xitren/crc_concept.hpp>

namespace xitren::crc {

/*!
\brief CRC XOR
*/
class crc_xor {
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
        value_type crc_mb{0xFF};
        for (std::input_iterator auto byte = begin; byte != end; byte++)
            crc_mb = crc_mb ^ (*byte);
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
        value_type crc_mb{0xFF};
        for (auto& byte : data) {
            crc_mb = crc_mb ^ (byte);
        }
        return crc_mb;
    }
};

}    // namespace xitren::crc
