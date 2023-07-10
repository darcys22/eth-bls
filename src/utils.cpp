#include "eth-bls/utils.hpp"

extern "C" {
#include "crypto/keccak.h"
}

std::vector<unsigned char> utils::fromHexString(std::string hex_str) {
    std::vector<unsigned char> bytes;

    // Check for "0x" prefix and remove it
    if(hex_str.size() >= 2 && hex_str[0] == '0' && hex_str[1] == 'x') {
        hex_str = hex_str.substr(2);
    }

    for (unsigned int i = 0; i < hex_str.length(); i += 2) {
        std::string byteString = hex_str.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(), nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}

std::array<unsigned char, 32> utils::fromHexString32Byte(std::string hex_str) {
    std::vector<unsigned char> bytesVec = fromHexString(hex_str);

    if(bytesVec.size() != 32) {
        throw std::invalid_argument("Input string length should be 64 characters for 32 bytes");
    }

    std::array<unsigned char, 32> bytesArr;
    std::copy(bytesVec.begin(), bytesVec.end(), bytesArr.begin());

    return bytesArr;
}

std::array<unsigned char, 32> utils::hash(std::string in) {
    std::vector<unsigned char> bytes;

    // Check for "0x" prefix and if exists, convert the hex to bytes
    if(in.size() >= 2 && in[0] == '0' && in[1] == 'x') {
        bytes = fromHexString(in);
        in = std::string(bytes.begin(), bytes.end());
    }

    std::array<unsigned char, 32> hash;
    keccak(reinterpret_cast<const uint8_t*>(in.c_str()), in.size(), hash.data(), 32);
    return hash;
}

std::vector<unsigned char> utils::intToBytes(uint64_t num) {
    if (num == 0) 
        return std::vector<unsigned char>{};

    std::stringstream stream;
    stream << std::hex << num;
    std::string hex = stream.str();
    if (hex.length() % 2) { hex = "0" + hex; }

    std::vector<unsigned char> result(hex.length() / 2);
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
        result[i / 2] = byte;
    }

    return result;
}
