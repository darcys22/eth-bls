#include <iostream>

#include "eth-bls/bls_validator_contract.hpp"
#include "eth-bls/config.hpp"
#include "eth-bls/service_node_list.hpp"
#include "eth-bls/signer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

//inline constexpr std::string_view PRIVATE_KEY = "96a656cbd64281ea82257ca9978093b25117592287e4e07f5be660d1701f03e9";
//const auto& config = ethbls::get_config(ethbls::network_type::SEPOLIA);
inline constexpr std::string_view PRIVATE_KEY = "ac0974bec39a17e36ba4a6b4d238ff944bacb478cbed5efcae784d7bf4f2ff80";
const auto& config = ethbls::get_config(ethbls::network_type::GANACHE);
auto provider = std::make_shared<Provider>("Sepolia Client", std::string(config.RPC_URL));
BLSValidatorsContract bls_contract(std::string(config.BLS_CONTRACT_ADDRESS), provider);
Signer signer(provider);
std::vector<unsigned char> seckey = utils::fromHexString(std::string(PRIVATE_KEY));
     
TEST_CASE( "insert pubkeys check they exist and clear", "[bls contract]" ) {
    //SKIP("slow test");
    SKIP("THIS IS FINE");
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

TEST_CASE( "Checks an aggregate pubkey", "[bls contract]" ) {
    SKIP("THIS IS FINE");
    ServiceNodeList snl(5);
    auto tx = bls_contract.clear();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        tx = bls_contract.addValidator(pubkey);
        hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
    }
    auto tx2 = bls_contract.checkAggPubkey(snl.aggregatePubkeyHex());
    hash = signer.sendTransaction(tx2, seckey);
    REQUIRE(hash != "");
    auto height_included = provider->waitForTransaction(hash);
    REQUIRE(height_included > 0);
    tx = bls_contract.clear();
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
}

TEST_CASE( "Signs an aggregate signature and checks its valid", "[bls contract]" ) {
    //SKIP("Not implemented");
    ServiceNodeList snl(4);
    auto tx = bls_contract.clear();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    const std::string message = "You are allowed to leave";
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        tx = bls_contract.addValidator(pubkey);
        hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
    }
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - aggregateSignature: " << snl.aggregateSignatures(message) << " - debug\n";
    auto tx2 = bls_contract.checkSigAGG(snl.aggregateSignatures(message), message);
    hash = signer.sendTransaction(tx2, seckey);
    REQUIRE(hash != "");
    auto height_included = provider->waitForTransaction(hash);
    REQUIRE(height_included > 0);
    tx = bls_contract.clear();
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
}
