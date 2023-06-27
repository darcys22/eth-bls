#include "eth-bls/EthereumClient.hpp"
#include "eth-bls/config.hpp"

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
