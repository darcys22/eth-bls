#define MCLBN_FP_UNIT_SIZE 4
#define MCLBN_FR_UNIT_SIZE 4
#define BLS_ETH
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <bls/bls.hpp>
#include <mcl/bn.hpp>
#undef MCLBN_NO_AUTOLINK
#pragma GCC diagnostic pop

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

#include "eth-bls/service_node_list.hpp"
#include "eth-bls/utils.hpp"
#include "eth-bls/ec_utils.hpp"

struct BLSInitializer {
    BLSInitializer() {
        // init library once before calling the other APIs
        bls::init(mclBn_CurveSNARK1);
        mclBn_setMapToMode(MCL_MAP_TO_MODE_TRY_AND_INC);
    }
    ~BLSInitializer() {
    }
};

BLSInitializer blsInitializer;

TEST_CASE( "root a simple imaginary number", "[mcl]" ) {
    mcl::bn::Fp2 y = mcl::bn::Fp2(3,4);
    REQUIRE(mcl::bn::Fp2::squareRoot(y, y));
}

TEST_CASE( "Weierstrass", "[mcl]" ) {
    const std::string message = "You are allowed to leave";
    std::array<unsigned char, 32> hash = utils::hash(message);
    mcl::bn::Fp x;
    x.setArrayMask(hash.data(), hash.size());
    mcl::bn::Fp2 y = mcl::bn::Fp2(x,0);
    mcl::bn::Fp2 z;
    mcl::bn::G2::getWeierstrass(z, y);
    REQUIRE(mcl::bn::Fp2::squareRoot(z, z));

    mcl::bn::G2 Q;
    mcl::ec::tryAndIncMapTo<mcl::bn::G2>(Q, mcl::bn::Fp2(x,0));

    mcl::bn::G2 Hm;
    bool r;
    mcl::bn::mapToG2(&r, Hm, mcl::bn::Fp2(x,0));
    REQUIRE(r);
    Hm.normalize();
}
