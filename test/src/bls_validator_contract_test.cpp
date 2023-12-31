#include <iostream>

#include "eth-bls/bls_validator_contract.hpp"
#include "eth-bls/config.hpp"
#include "eth-bls/service_node_list.hpp"
#include "eth-bls/signer.hpp"
#include "eth-bls/ec_utils.hpp"

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
     
TEST_CASE( "insert pubkeys check they exist and clear", "[bls contract]" ) {
    ServiceNodeList snl(3);
    auto tx = bls_contract.clear();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    REQUIRE(provider->transactionSuccessful(hash));
    REQUIRE(bls_contract.getValidatorsLength() == 0);
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        tx = bls_contract.addValidator(pubkey);
        hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
    }
    REQUIRE(provider->transactionSuccessful(hash));
    REQUIRE(bls_contract.getValidatorsLength() == 3);
    tx = bls_contract.clear();
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    REQUIRE(provider->transactionSuccessful(hash));
    REQUIRE(bls_contract.getValidatorsLength() == 0);
}

TEST_CASE( "Checks an aggregate pubkey", "[bls contract]" ) {
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
    REQUIRE(provider->transactionSuccessful(hash));
    tx = bls_contract.clear();
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
}

TEST_CASE( "Signs an aggregate signature and checks its valid", "[bls contract]" ) {
    ServiceNodeList snl(20);
    auto tx = bls_contract.clear();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    const std::string message = utils::generateRandomString(20);
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        tx = bls_contract.addValidator(pubkey);
        hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
    }
    auto tx2 = bls_contract.checkSigAGG(snl.aggregateSignatures(message), message);
    hash = signer.sendTransaction(tx2, seckey);
    REQUIRE(hash != "");
    REQUIRE(provider->transactionSuccessful(hash));
    // Modify our list to be different and send a bad signature and expect failure
    snl.addNode();
    auto tx3 = bls_contract.checkSigAGG(snl.aggregateSignatures(message), message);
    // Local chain will throw directly, otherwise it will get into a blockchain but fail
    if (config.CHAIN_ID == 1337) {
        REQUIRE_THROWS(signer.sendTransaction(tx3, seckey));
    } else {
        hash = signer.sendTransaction(tx3, seckey);
        REQUIRE(hash != "");
        REQUIRE(!provider->transactionSuccessful(hash));
    }
    tx = bls_contract.clear();
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
}

TEST_CASE( "Signs an aggregate signature from indices and checks its valid", "[bls contract]" ) {
    ServiceNodeList snl(6);
    auto tx = bls_contract.clear();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    const std::string message = utils::generateRandomString(20);
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        tx = bls_contract.addValidator(pubkey);
        hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
    }
    const std::vector<int64_t> indices = {1,3,5};
    auto tx2 = bls_contract.checkSigAGGIndices(snl.aggregateSignaturesFromIndices(message, indices), message, indices);
    hash = signer.sendTransaction(tx2, seckey);
    REQUIRE(hash != "");
    REQUIRE(provider->transactionSuccessful(hash));
    tx = bls_contract.clear();
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
}

TEST_CASE( "Signs an aggregate signature from indices and sends the non-signers to the contract", "[bls contract]" ) {
    ServiceNodeList snl(6);
    auto tx = bls_contract.clear();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    const std::string message = utils::generateRandomString(20);
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        tx = bls_contract.addValidator(pubkey);
        hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
    }
    const std::vector<int64_t> indices = {0,1,3,5};
    auto tx2 = bls_contract.checkSigAGGNegateIndices(snl.aggregateSignaturesFromIndices(message, indices), message, snl.findNonSigners(indices));
    hash = signer.sendTransaction(tx2, seckey); REQUIRE(hash != "");
    REQUIRE(provider->transactionSuccessful(hash));
    // Modify our list to be different and send a bad indices and expect failure
    const std::vector<int64_t> indices2 = {0,1,2,3,5};
    auto tx3 = bls_contract.checkSigAGGNegateIndices(snl.aggregateSignaturesFromIndices(message, indices), message, snl.findNonSigners(indices2));
    // Local chain will throw directly, otherwise it will get into a blockchain but fail
    if (config.CHAIN_ID == 1337) {
        REQUIRE_THROWS(signer.sendTransaction(tx3, seckey));
    } else {
        hash = signer.sendTransaction(tx3, seckey);
        REQUIRE(hash != "");
        REQUIRE(!provider->transactionSuccessful(hash));
    }
    tx = bls_contract.clear();
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
}

TEST_CASE( "Validates a proof of possession", "[bls contract]" ) {
    ServiceNodeList snl(200);
    size_t count = 0;
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        auto tx = bls_contract.validateProofOfPossession(pubkey, node.proofOfPossession());
        try {
            auto hash = signer.sendTransaction(tx, seckey);
            REQUIRE(hash != "");
            count++;
            std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - Succeeded\n";
        } catch(const std::exception& e) {
            std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - Failed\n";
        }
        //REQUIRE(provider->transactionSuccessful(hash));
    }
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - successful transactions: " << count << "/200 - debug\n";
}

TEST_CASE( "Validates map to field", "[bls contract]" ) {
    ServiceNodeList snl(3);
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        const auto contract = bls_contract.calcField(pubkey);
        const auto cppside = "0x" + utils::toHexString(utils::HashModulus("0x" + pubkey));
        REQUIRE(cppside == contract);
    }
}
