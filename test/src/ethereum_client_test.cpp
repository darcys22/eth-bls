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
    std::cout << hash_hello_world << '\n';
    REQUIRE( hash_hello_world == "57caa176af1ac0433c5df30e8dabcd2ec1af1e92a26eced5f719b88458777cd6" );
}

inline constexpr std::string_view PRIVATE_KEY = "96a656cbd64281ea82257ca9978093b25117592287e4e07f5be660d1701f03e9";
TEST_CASE( "SigningTest", "[utils]" ) {
    std::vector<unsigned char> seckey = utils::fromHexString(std::string(PRIVATE_KEY));
    Signer signer;
    std::string hash_hello_world = utils::toHexString(utils::hash("Hello World!\n"));
    std::cout << hash_hello_world << '\n';
    const auto signature_bytes = signer.sign("Hello World!", seckey);
    std::string signature_hex = utils::toHexString(signature_bytes);
    std::cout << signature_hex << '\n';
    REQUIRE( signature_hex == "35f409302082e02b5126c82be93a3946d30e93722ce3ff87bdb01fc385fe312054f3fade7fab80dcabadabf96af75577327dfd064abd47a36543a475e04840e71c" );
}
