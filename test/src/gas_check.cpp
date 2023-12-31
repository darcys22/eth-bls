#include <iostream>

#include "eth-bls/bls_validator_contract.hpp"
#include "eth-bls/config.hpp"
#include "eth-bls/service_node_list.hpp"
#include "eth-bls/signer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

#if 0 
inline constexpr std::string_view PRIVATE_KEY = "96a656cbd64281ea82257ca9978093b25117592287e4e07f5be660d1701f03e9";
const auto& config = ethbls::get_config(ethbls::network_type::SEPOLIA);
#else
inline constexpr std::string_view PRIVATE_KEY = "ac0974bec39a17e36ba4a6b4d238ff944bacb478cbed5efcae784d7bf4f2ff80";
const auto& config = ethbls::get_config(ethbls::network_type::GANACHE);
#endif

auto provider = std::make_shared<Provider>("Client", std::string(config.RPC_URL));
BLSValidatorsContract bls_contract(std::string(config.BLS_CONTRACT_ADDRESS), provider);
Signer signer(provider);
std::vector<unsigned char> seckey = utils::fromHexString(std::string(PRIVATE_KEY));
     
TEST_CASE( "Submits an aggregate signature and prints its gas usage", "[bls contract]" ) {
    SKIP("Expensive test that only highlights gas costs");
    for (size_t non_signers_count = 0; non_signers_count < 5; non_signers_count++) {
        std::cout << "Number of Non Signers: " << non_signers_count << "\n";
        for (size_t count = non_signers_count + 200; count < 501 + non_signers_count; count=count+200) {
            ServiceNodeList snl(count);
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
            const auto indices = snl.randomSigners(snl.nodes.size() - non_signers_count);
            tx = bls_contract.checkSigAGGNegateIndices(snl.aggregateSignaturesFromIndices(message, indices), message, snl.findNonSigners(indices));
            hash = signer.sendTransaction(tx, seckey);
            REQUIRE(hash != "");
            REQUIRE(provider->transactionSuccessful(hash));
            std::cout << "Number of Nodes: " << count << ", Gas Used: " << provider->gasUsed(hash) << "\n";
        }
    }
    auto tx = bls_contract.clear();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
}

TEST_CASE( "Submits an aggregate signature and makes sure its below block gas limit", "[bls contract]" ) {
    ServiceNodeList snl(1800);
    // this adds gas over the block limit, need hardhat to allow for this additinal gas in hardhat config
    auto tx = bls_contract.clear(20000000);
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    const std::string message = "You are allowed to leave";
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        tx = bls_contract.addValidator(pubkey);
        hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
    }
    const auto indices = snl.randomSigners(snl.nodes.size() - 300);
    tx = bls_contract.checkSigAGGNegateIndices(snl.aggregateSignaturesFromIndices(message, indices), message, snl.findNonSigners(indices));
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    REQUIRE(provider->transactionSuccessful(hash));
    REQUIRE(provider->gasUsed(hash) < 30000000);
    tx = bls_contract.clear(20000000);
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
}
