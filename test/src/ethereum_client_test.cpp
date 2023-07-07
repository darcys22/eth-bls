#include <iostream>

#include "eth-bls/EthereumClient.hpp"
#include "eth-bls/signer.hpp"
#include "eth-bls/config.hpp"
#include "eth-bls/utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE( "Get balance from Arbitrum network", "[ethereum]" ) {
    // Get the arbitrum config
    const auto& config = ethbls::get_config(ethbls::network_type::ARBITRUM);

    // Construct the client with the Arbitrum RPC URL
    EthereumClient client("Arbitrum Client", std::string(config.RPC_URL));

    // Get the balance of the test address
    uint64_t balance = client.getBalance("0x000433708645EaaD9f65687CDbe4033d92f6A6d2");

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
    const auto signature_bytes = signer.sign("Hello World!", seckey);
    std::string signature_hex = utils::toHexString(signature_bytes);
    REQUIRE( signature_hex == "35f409302082e02b5126c82be93a3946d30e93722ce3ff87bdb01fc385fe312054f3fade7fab80dcabadabf96af75577327dfd064abd47a36543a475e04840e71c" );
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
    Transaction tx(1, "0xA6C077fd9283421C657EcEa8a9c1422cc6CEbc80", 1000000000000000000, 21000);
    std::string raw_tx = tx.serialized();
    std::string correct_raw_tx = "0x02e70101808082520894a6c077fd9283421c657ecea8a9c1422cc6cebc80880de0b6b3a764000080c0";
    REQUIRE(raw_tx == correct_raw_tx);
}

TEST_CASE( "Hashes an unsigned transaction correctly", "[transaction]" ) {
    Transaction tx(1, "0xA6C077fd9283421C657EcEa8a9c1422cc6CEbc80", 1000000000000000000, 21000);
    std::string unsigned_hash = tx.hash();
    std::string correct_hash = "0xf81a17092cfb066efa3ff6ef92016adc06ff66a64327359c4003d215d56128b3";
    REQUIRE(unsigned_hash == correct_hash);
}
