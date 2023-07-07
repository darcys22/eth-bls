#include <iostream>

#include "eth-bls/provider.hpp"
#include "eth-bls/signer.hpp"
#include "eth-bls/config.hpp"
#include "eth-bls/utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE( "Get balance from sepolia network", "[ethereum]" ) {
    // Get the sepolia config
    const auto& config = ethbls::get_config(ethbls::network_type::SEPOLIA);

    // Construct the client with the sepolia RPC URL
    Provider client("Sepolia Client", std::string(config.RPC_URL));

    // Get the balance of the test address
    uint64_t balance = client.getBalance("0x2Ccb8b65024E4aA9615a8E704DFb11BE76674f1F");

    // Check that the balance is greater than zero
    REQUIRE( balance > 0 );
}

TEST_CASE( "HashTest", "[utils]" ) {
    std::string hash_hello_world = utils::toHexString(utils::hash("hello world!"));
    REQUIRE( hash_hello_world == "57caa176af1ac0433c5df30e8dabcd2ec1af1e92a26eced5f719b88458777cd6" );
}

inline constexpr std::string_view PRIVATE_KEY = "96a656cbd64281ea82257ca9978093b25117592287e4e07f5be660d1701f03e9";
TEST_CASE( "SigningTest", "[signer]" ) {
    std::vector<unsigned char> seckey = utils::fromHexString(std::string(PRIVATE_KEY));
    Signer signer;
    std::string hash_hello_world = utils::toHexString(utils::hash("Hello World!\n"));
    const auto signature_bytes = signer.signMessage("Hello World!", seckey);
    std::string signature_hex = utils::toHexString(signature_bytes);
    REQUIRE( signature_hex == "35f409302082e02b5126c82be93a3946d30e93722ce3ff87bdb01fc385fe312054f3fade7fab80dcabadabf96af75577327dfd064abd47a36543a475e04840e701" );
}


//Raw transaction data
//{
  //type: null,
  //to: '0xA6C077fd9283421C657EcEa8a9c1422cc6CEbc80',
  //data: '0x',
  //nonce: 1,
  //gasLimit: '21000',
  //gasPrice: null,
  //maxPriorityFeePerGas: null,
  //maxFeePerGas: null,
  //value: '1000000000000000000',
  //chainId: '1',
  //sig: null,
  //accessList: null
//}
//0x02e70101808082520894a6c077fd9283421c657ecea8a9c1422cc6cebc80880de0b6b3a764000080c0
TEST_CASE( "Serialise a raw transaction correctly", "[transaction]" ) {
    Transaction tx("0xA6C077fd9283421C657EcEa8a9c1422cc6CEbc80", 1000000000000000000, 21000);
    tx.nonce = 1;
    std::string raw_tx = tx.serialized();
    std::string correct_raw_tx = "0x02e70101808082520894a6c077fd9283421c657ecea8a9c1422cc6cebc80880de0b6b3a764000080c0";
    REQUIRE(raw_tx == correct_raw_tx);
}

TEST_CASE( "Hashes an unsigned transaction correctly", "[transaction]" ) {
    Transaction tx("0xA6C077fd9283421C657EcEa8a9c1422cc6CEbc80", 1000000000000000000, 21000);
    tx.nonce = 1;
    std::string unsigned_hash = tx.hash();
    std::string correct_hash = "0xf81a17092cfb066efa3ff6ef92016adc06ff66a64327359c4003d215d56128b3";
    REQUIRE(unsigned_hash == correct_hash);
}

TEST_CASE( "Signs an unsigned transaction correctly", "[transaction]" ) {
    std::vector<unsigned char> seckey = utils::fromHexString(std::string(PRIVATE_KEY));
    Signer signer;
    Transaction tx("0xA6C077fd9283421C657EcEa8a9c1422cc6CEbc80", 1000000000000000000, 21000);
    tx.nonce = 1;
    const auto signature_hex_string = signer.signTransaction(tx, seckey);
    REQUIRE( signature_hex_string == "0x02f86a0101808082520894a6c077fd9283421c657ecea8a9c1422cc6cebc80880de0b6b3a764000080c080a084987299f8dd115333356ab03430ca8de593e03ba03d4ecd72daf15205119cf8a0216c9869da3497ae96dcb98713908af1a0abf866c12d51def821caf0374cccbb" );
}

TEST_CASE( "Does a self transfer on Sepolia", "[transaction]" ) {
    const auto& config = ethbls::get_config(ethbls::network_type::SEPOLIA);
    auto provider = std::make_shared<Provider>("Sepolia Client", std::string(config.RPC_URL));
    Signer signer(provider);

    Transaction tx("0x2Ccb8b65024E4aA9615a8E704DFb11BE76674f1F", 100000000000000, 21000);
    tx.chainId = config.CHAIN_ID;
    tx.nonce = provider->getTransactionCount("0x2Ccb8b65024E4aA9615a8E704DFb11BE76674f1F", "pending");
    const auto feeData = provider->getFeeData();
    tx.maxFeePerGas = feeData.maxFeePerGas;
    tx.maxPriorityFeePerGas = feeData.maxPriorityFeePerGas;
    std::cout << "Nonce: " << tx.nonce << '\n';
    std::vector<unsigned char> seckey = utils::fromHexString(std::string(PRIVATE_KEY));
    const auto signature_hex_string = signer.signTransaction(tx, seckey);
    auto hash = provider->sendTransaction(tx);
    std::cout << "Transaction Hash: " << hash << '\n';
}
