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

#include "eth-bls/utils.hpp"
#include "eth-bls/ec_utils.hpp"

struct BLSInitializer {
    BLSInitializer() {
        // init library once before calling the other APIs
        bls::init(mclBn_CurveSNARK1);
        mclBn_setMapToMode(MCL_MAP_TO_MODE_TRY_AND_INC);
        mcl::bn::G1 gen;
        bool b;
        mcl::bn::mapToG1(&b, gen, 1);
        blsPublicKey publicKey;
        publicKey.v = *reinterpret_cast<const mclBnG1*>(&gen); // Cast gen to mclBnG1 and assign it to publicKey.v

        blsSetGeneratorOfPublicKey(&publicKey);
    }
    ~BLSInitializer() {
    }
};

BLSInitializer blsInitializer;

TEST_CASE( "verify aggregate signature", "[mcl]" ) {
    const std::string message = "You are allowed to leave";
    std::array<unsigned char, 32> hash = utils::hash(message);

    bls::SignatureVec sigs;
    bls::PublicKeyVec pubs;

    bls::SecretKey secretKey;
    bls::PublicKey publicKey;
    bls::Signature sig;
    size_t nodes = 5;
    for(size_t i = 0; i < nodes; i++) {
        secretKey.init();
        secretKey.getPublicKey(publicKey);
        pubs.push_back(publicKey);
        secretKey.signHash(sig, hash.data(), hash.size());
        sigs.push_back(sig);
    }

    publicKey.clear();
    sig.clear();
	for (mclSize i = 1; i < nodes; i++) {
        publicKey.add(pubs[i]);
        sig.add(sigs[i]);
	}

    mcl::bn::G2 Hm;
    mcl::bn::Fp t;
	t.setArrayMask(hash.data(), hash.size());
	bool b;
    mcl::bn::mapToG2(&b, Hm, mcl::bn::Fp2(t, 0));
    REQUIRE(b);
    /*
        e(P, sHm) == e(sP, Hm)
        <=> finalExp(ML(P, sHm) * e(-sP, Hm)) == 1
    */
    mcl::bn::GT e;
    mcl::bn::G1 v1[2];
    mcl::bn::G2 v2[2] = { *reinterpret_cast<const mcl::bn::G2*>(&(sig.getPtr()->v)), Hm};
    mcl::bn::mapToG1(&b, v1[0], 1);
    REQUIRE(b);
    mcl::bn::G1::neg(v1[1], *reinterpret_cast<const mcl::bn::G1*>(&(publicKey.getPtr()->v)));

    mcl::bn::millerLoopVec(e, v1, v2, 2);
    mcl::bn::finalExp(e, e);
    REQUIRE(e.isOne());
}

TEST_CASE( "verify single signature", "[mcl]" ) {
    const std::string message = "You are allowed to leave";
    std::array<unsigned char, 32> hash = utils::hash(message);

    bls::SecretKey secretKey;
    bls::PublicKey publicKey;
    bls::Signature sig;
    secretKey.init();
    secretKey.getPublicKey(publicKey);
    secretKey.signHash(sig, hash.data(), hash.size());

    mcl::bn::G2 Hm;
    mcl::bn::Fp t;
	t.setArrayMask(hash.data(), hash.size());
	bool b;
    mcl::bn::mapToG2(&b, Hm, mcl::bn::Fp2(t, 0));
    REQUIRE(b);

    /*
        e(P, sHm) == e(sP, Hm)
        <=> finalExp(ML(P, sHm) * e(-sP, Hm)) == 1
    */
    mcl::bn::GT e;
    mcl::bn::G1 v1[2];
    mcl::bn::G2 v2[2] = { *reinterpret_cast<const mcl::bn::G2*>(&(sig.getPtr()->v)), Hm};
    mcl::bn::mapToG1(&b, v1[0], 1);
    REQUIRE(b);
    mcl::bn::G1::neg(v1[1], *reinterpret_cast<const mcl::bn::G1*>(&(publicKey.getPtr()->v)));

    mcl::bn::millerLoopVec(e, v1, v2, 2);
    mcl::bn::finalExp(e, e);
    REQUIRE(e.isOne());
}
