#include <rlpvalue.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE( "Test if RLP is working", "[rlp]" ) {
    RLPValue v("foo");
    v.clear();
    REQUIRE(v.getValStr() == "");

    v.setArray();
    REQUIRE(v.isArray() == true);
    REQUIRE(v.size() == 0);

    v.assign("zum");
    REQUIRE(v.isBuffer() == true);
    REQUIRE(v.getValStr() == "zum");
}
