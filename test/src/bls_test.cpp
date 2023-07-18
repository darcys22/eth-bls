//#include <bls/bls.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

struct BLSInitializer {
    BLSInitializer() {
        // init library once before calling the other APIs
        //bls::init();
    }
    ~BLSInitializer() {
    }
};

BLSInitializer blsInitializer;

TEST_CASE( "k of n test", "[bls]" ) {
}
