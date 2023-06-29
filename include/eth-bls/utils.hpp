#pragma once

#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ios>

namespace utils
{
    template <typename Container>
    std::string toHexString(const Container& bytes) {
        std::ostringstream oss;
        for(const auto byte : bytes) {
            oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
        }                                                                                                                     
        return oss.str();                                                                                                     
    }

    std::vector<unsigned char> fromHexString(const std::string& hex_str);

    std::array<unsigned char, 32> hash(const std::string& in);

    std::vector<unsigned char> intToBytes(uint64_t num);
// END
}
