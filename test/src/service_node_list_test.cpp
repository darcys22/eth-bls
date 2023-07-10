#include <iostream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

#include "eth-bls/service_node_list.hpp"

TEST_CASE( "Creates a Service Node List", "[service node list]" ) {
    ServiceNodeList snl(5);
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        REQUIRE(pubkey != "");
    }
}

TEST_CASE( "Creates an aggregate signature", "[service node list]" ) {
    ServiceNodeList snl(5);
    const auto sig = snl.aggregateSignatures("Hello World");
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - " << sig << " - debug\n";
    REQUIRE(sig != "");
}
