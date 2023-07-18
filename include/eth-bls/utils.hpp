#pragma once

#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ios>

namespace utils
{

    enum class PaddingDirection {
        LEFT,
        RIGHT
    };

    template <typename Container>
    std::string toHexString(const Container& bytes) {
        std::ostringstream oss;
        for(const auto byte : bytes) {
            oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(static_cast<unsigned char>(byte));
        }                                                                                                                     
        return oss.str();                                                                                                     
    }

    std::string decimalToHex(uint64_t decimal);

    std::vector<unsigned char> fromHexString(std::string hex_str);

    std::array<unsigned char, 32> fromHexString32Byte(std::string hex_str);

    std::array<unsigned char, 32> hash(std::string in);

    std::string getFunctionSignature(const std::string& function);
    std::string padTo32Bytes(const std::string& input, PaddingDirection direction = PaddingDirection::LEFT);

    std::vector<unsigned char> intToBytes(uint64_t num);

// END
}
