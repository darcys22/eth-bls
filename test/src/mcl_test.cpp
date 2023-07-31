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
    }
    ~BLSInitializer() {
    }
};

BLSInitializer blsInitializer;

TEST_CASE( "hashToG2", "[mcl]" ) {
    const std::string message = "You are allowed to leave";
    std::array<unsigned char, 32> hash = utils::hash(message);
    mcl::bn::G2 Q;
    mcl::bn::Fp x;
    x.setArrayMask(hash.data(), hash.size());
    std::array<unsigned char, 32> serialized_hash;
    uint8_t *hdst = serialized_hash.data();
    mclSize serializedSignatureSize = 32;
    if (x.serialize(hdst, serializedSignatureSize, mcl::IoSerialize) == 0)
        throw std::runtime_error("size of x is zero");
    mcl::ec::tryAndIncMapTo<mcl::bn::G2>(Q, mcl::bn::Fp2(x,0));

    std::vector<unsigned char> serialized_signature(serializedSignatureSize*4);
    uint8_t *dst = serialized_signature.data();
    if (Q.x.a.serialize(dst, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.a is zero");
    if (Q.x.b.serialize(dst + serializedSignatureSize, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.b is zero");
    if (Q.y.a.serialize(dst + serializedSignatureSize * 2, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of y.a is zero");
    if (Q.y.b.serialize(dst + serializedSignatureSize * 3, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of y.b is zero");

}

TEST_CASE( "SquareRootG2", "[mcl]" ) {
    SKIP("Works");
    const std::string message = "You are allowed to leave";
    std::array<unsigned char, 32> hash = utils::hash(message);
    mcl::bn::Fp x;
    x.setArrayMask(hash.data(), hash.size());
    std::array<unsigned char, 64> serialized_hash;
    uint8_t *hdst = serialized_hash.data();
    mclSize serializedSignatureSize = 32;
    mcl::bn::Fp2 y = mcl::bn::Fp2(x,0);
    if (y.a.serialize(hdst, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.a is zero");
    if (y.b.serialize(hdst + serializedSignatureSize, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.b is zero");
    if(mcl::bn::Fp2::squareRoot(y, y))
        throw std::runtime_error("failed to sqroot");
    std::array<unsigned char, 64> serialized_hash2;
    uint8_t *hdst2 = serialized_hash2.data();
    if (y.a.serialize(hdst2, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.a is zero");
    if (y.b.serialize(hdst2 + serializedSignatureSize, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.b is zero");
}

TEST_CASE( "root a simple imaginary number", "[mcl]" ) {
    SKIP("Works");
    mcl::bn::Fp2 y = mcl::bn::Fp2(3,4);
    REQUIRE(mcl::bn::Fp2::squareRoot(y, y));
}

TEST_CASE( "Weierstrass", "[mcl]" ) {
    const std::string message = "You are allowed to leave";
    std::array<unsigned char, 32> hash = utils::hash(message);
    mcl::bn::Fp x;
    x.setArrayMask(hash.data(), hash.size());
    std::array<unsigned char, 64> serialized_hash;
    uint8_t *hdst = serialized_hash.data();
    mclSize serializedSignatureSize = 32;
    mcl::bn::Fp2 y = mcl::bn::Fp2(x,0);
    if (y.a.serialize(hdst, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.a is zero");
    if (y.b.serialize(hdst + serializedSignatureSize, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.b is zero");
    std::array<unsigned char, 32> serialized_hashx;
    uint8_t *hdstx = serialized_hashx.data();
    if (y.a.serialize(hdstx, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.a is zero");
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - hash sent to contract: " << utils::toHexString(serialized_hashx) << " - debug\n";
    mcl::bn::Fp2 z;
    mcl::bn::G2::getWeierstrass(z, y);
    std::array<unsigned char, 64> serialized_hash2;
    uint8_t *hdst2 = serialized_hash2.data();
    if (z.a.serialize(hdst2, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.a is zero");
    if (z.b.serialize(hdst2 + serializedSignatureSize, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.b is zero");
    REQUIRE(mcl::bn::Fp2::squareRoot(z, z));
    std::array<unsigned char, 64> serialized_hash3;
    uint8_t *hdst3 = serialized_hash3.data();
    if (z.a.serialize(hdst3, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.a is zero");
    if (z.b.serialize(hdst3 + serializedSignatureSize, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.b is zero");

    mcl::bn::G2 Q;
    mcl::ec::tryAndIncMapTo<mcl::bn::G2>(Q, mcl::bn::Fp2(x,0));
    std::vector<unsigned char> serialized_signature5(serializedSignatureSize*4);
    uint8_t *dst5 = serialized_signature5.data();
    if (Q.x.a.serialize(dst5, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.a is zero");
    if (Q.x.b.serialize(dst5 + serializedSignatureSize, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.b is zero");
    if (Q.y.a.serialize(dst5 + serializedSignatureSize * 2, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of y.a is zero");
    if (Q.y.b.serialize(dst5 + serializedSignatureSize * 3, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of y.b is zero");
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - " << utils::toHexString(serialized_signature5) << " - debug\n";

    mcl::bn::G2 Hm;
    bool r;
    mcl::bn::mapToG2(&r, Hm, mcl::bn::Fp2(x,0));
    std::vector<unsigned char> serialized_signature6(serializedSignatureSize*4);
    uint8_t *dst6 = serialized_signature6.data();
    if (Hm.x.a.serialize(dst6, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.a is zero");
    if (Hm.x.b.serialize(dst6 + serializedSignatureSize, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of x.b is zero");
    if (Hm.y.a.serialize(dst6 + serializedSignatureSize * 2, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of y.a is zero");
    if (Hm.y.b.serialize(dst6 + serializedSignatureSize * 3, serializedSignatureSize, mcl::IoSerialize | mcl::IoBigEndian) == 0)
        throw std::runtime_error("size of y.b is zero");
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - " << utils::toHexString(serialized_signature6) << " - debug\n";

}
