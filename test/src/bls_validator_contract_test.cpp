#include <iostream>

#include "eth-bls/bls_validator_contract.hpp"
#include "eth-bls/config.hpp"
#include "eth-bls/service_node_list.hpp"
#include "eth-bls/signer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

inline constexpr std::string_view PRIVATE_KEY = "96a656cbd64281ea82257ca9978093b25117592287e4e07f5be660d1701f03e9";
const auto& config = ethbls::get_config(ethbls::network_type::SEPOLIA);
auto provider = std::make_shared<Provider>("Sepolia Client", std::string(config.RPC_URL));
BLSValidatorsContract bls_contract(std::string(config.BLS_CONTRACT_ADDRESS), provider);
Signer signer(provider);
std::vector<unsigned char> seckey = utils::fromHexString(std::string(PRIVATE_KEY));
     
TEST_CASE( "Add Public Keys to contract", "[bls contract]" ) {
    ServiceNodeList snl(3);
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        auto tx = bls_contract.addValidator(pubkey);
        const auto hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
    }
}

