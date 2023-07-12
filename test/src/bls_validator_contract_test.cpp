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
     
TEST_CASE( "Add Public Keys to contract, test they exist and clear them", "[bls contract]" ) {
    ServiceNodeList snl(3);
    auto tx = bls_contract.clear();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    auto height_included = provider->waitForTransaction(hash);
    REQUIRE(height_included > 0);
    REQUIRE(bls_contract.getValidatorsLength() == 0);
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        tx = bls_contract.addValidator(pubkey);
        hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
    }
    height_included = provider->waitForTransaction(hash);
    REQUIRE(height_included > 0);
    REQUIRE(bls_contract.getValidatorsLength() == 3);
    tx = bls_contract.clear();
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    height_included = provider->waitForTransaction(hash);
    REQUIRE(height_included > 0);
    REQUIRE(bls_contract.getValidatorsLength() == 0);
}

