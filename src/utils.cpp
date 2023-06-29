#include "eth-bls/utils.hpp"

extern "C" {
#include "crypto/keccak.h"
}

std::vector<unsigned char> utils::fromHexString(const std::string& hex_str) {
    std::vector<unsigned char> bytes;

    for (unsigned int i = 0; i < hex_str.length(); i += 2) {
        std::string byteString = hex_str.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}

std::array<unsigned char, 32> utils::hash(const std::string& in) {
    std::array<unsigned char, 32> bytes;
    keccak(reinterpret_cast<const uint8_t*>(in.c_str()), in.size(), bytes.data(), 32);
    return bytes;
}

