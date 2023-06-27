// Copyright (c) 2014-2019, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ratio>
#include <stdexcept>
#include <string>
#include <string_view>

using namespace std::literals;

namespace ethbls {

/// global related constants:

inline constexpr uint64_t MAX_BLOCK_NUMBER = 500000000;
inline constexpr std::string_view BULLETPROOF_EXPONENT = "bulletproof"sv;

// filename constants:
inline constexpr auto DATA_DIRNAME =
#ifdef _WIN32
        "eth-bls"sv;  // Buried in some windows filesystem maze location
#else
        ".eth-bls"sv;      // ~/.oxen
#endif
inline constexpr auto DATA_FILENAME = "sqlite.db"sv;

enum class network_type : uint8_t { ARBITRUM = 0, UNDEFINED = 255 };

constexpr network_type network_type_from_string(std::string_view s) {
    if (s == "arbitrum")
        return network_type::ARBITRUM;

    return network_type::UNDEFINED;
}

constexpr std::string_view network_type_to_string(network_type t) {
    switch (t) {
        case network_type::ARBITRUM: return "arbitrum";
        default: return "undefined";
    }
    return "undefined";
}

// Various configuration defaults and network-dependent settings
namespace config {
    namespace arbitrum {
        inline constexpr std::string_view RPC_URL = "https://arb1.arbitrum.io/rpc";
        inline constexpr uint32_t CHAIN_ID = 42161;
        inline constexpr std::string_view BLOCK_EXPLORER_URL = "https://arbiscan.io";
        inline constexpr std::string_view OFFICIAL_WEBSITE = "https://portal.arbitrum.one";
    }  // namespace arbitrum
}  // namespace config

struct network_config {
    std::string_view RPC_URL;
    uint32_t CHAIN_ID;
    std::string_view BLOCK_EXPLORER_URL;
    std::string_view OFFICIAL_WEBSITE;
};

inline constexpr network_config arbitrum_config{
        config::arbitrum::RPC_URL,
        config::arbitrum::CHAIN_ID,
        config::arbitrum::BLOCK_EXPLORER_URL,
        config::arbitrum::OFFICIAL_WEBSITE,
};

inline constexpr const network_config& get_config(network_type nettype) {
    switch (nettype) {
        case network_type::ARBITRUM: return arbitrum_config;
        default: throw std::runtime_error{"Invalid network type"};
    }
}

}  // namespace ethbls
