#include <iostream>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

extern "C" {
#include "crypto/keccak.h"
}

#include "eth-bls/config.hpp"
#include "eth-bls/signer.hpp"
#include "eth-bls/utils.hpp"
#include "eth-bls/merkle_tree_creator.hpp"
#include "eth-bls/merklecpp.h"

#if 0 
inline constexpr std::string_view PRIVATE_KEY = "96a656cbd64281ea82257ca9978093b25117592287e4e07f5be660d1701f03e9";
const auto& config = ethbls::get_config(ethbls::network_type::SEPOLIA);
#else
inline constexpr std::string_view PRIVATE_KEY = "ac0974bec39a17e36ba4a6b4d238ff944bacb478cbed5efcae784d7bf4f2ff80";
const auto& config = ethbls::get_config(ethbls::network_type::GANACHE);
#endif

auto provider = std::make_shared<Provider>("Client", std::string(config.RPC_URL));
MerkleTreeCreator merkle_contract(std::string(config.MERKLE_CONTRACT_ADDRESS), provider);
Signer signer(provider);
std::vector<unsigned char> seckey = utils::fromHexString(std::string(PRIVATE_KEY));

std::vector<merkle::Hash> make_hashes(size_t n)
{
  std::vector<merkle::Hash> hashes;
  merkle::Tree::Hash h;
  for (size_t i = 0; i < n; i++)
  {
    hashes.push_back(h);
  }
  return hashes;
}

TEST_CASE( "verify merkle in cpp", "[merkle]" ) {
    merkle::Tree::Hash hash("fa8f44eabb728d4020e7f33d1aa973faaef19de6c06679bccdc5100a3c01f54a");
    merkle::Tree tree;
    tree.insert(hash);
    auto root = tree.root();
    auto path = tree.path(0);
    REQUIRE(path->verify(root));
}

void merkles() {
    const size_t num_leaves = 11;
    {
      auto hashes = make_hashes(num_leaves);
      merkle::TreeT<32, merkle::sha256_compress> tree;
      for (auto h : hashes)
        tree.insert(h);
      auto root = tree.root();
      auto path = tree.path(hashes.size() - 1);
      REQUIRE(path->verify(root));
    }
}

TEST_CASE( "verify more merkles in cpp", "[merkle]" ) {
    REQUIRE_NOTHROW(merkles());
}

static inline void cncrypto_keccak256(
    const merkle::HashT<32>& l,
    const merkle::HashT<32>& r,
    merkle::HashT<32>& out)
{
    uint8_t block[32 * 2];
    memcpy(&block[0], l.bytes, 32);
    memcpy(&block[32], r.bytes, 32);

    // Assuming keccak function signature remains the same as in the provided utils::hash function
    keccak(block, sizeof(block), out.bytes, 32);
}

void merkles_keccak() {
    const size_t num_leaves = 11;
    {
      auto hashes = make_hashes(num_leaves);
      merkle::TreeT<32, cncrypto_keccak256> tree;
      for (auto h : hashes)
      {
        tree.insert(h);
      }
      auto root = tree.root();
      auto path = tree.path(hashes.size() - 1);
      REQUIRE(path->verify(root));
    }
}
TEST_CASE( "verify more merkles using keccak in cpp using actual data", "[merkle]" ) {
    REQUIRE_NOTHROW(merkles_keccak());
}

merkle::Tree::Hash createHashTFromKeccak(const std::string& input) {
    // Compute Keccak hash using utils::hash
    std::array<unsigned char, 32> hash_result = utils::hash(input);
    
    // Convert std::array to std::vector
    std::vector<uint8_t> hash_vector(hash_result.begin(), hash_result.end());
    return merkle::Tree::Hash(hash_vector);
}

void merkles_keccak_with_data() {
    const size_t num_leaves = 6;
    {
      merkle::TreeT<32, cncrypto_keccak256> tree;
      for (size_t i = 0; i < num_leaves; i++)
      {
        auto h = createHashTFromKeccak(std::to_string(i));
        tree.insert(h);
      }
      auto root = tree.root();
      auto path = tree.path(num_leaves - 1);
      REQUIRE(path->verify(root));
    }
}
TEST_CASE( "verify more merkles using keccak in cpp", "[merkle]" ) {
    REQUIRE_NOTHROW(merkles_keccak_with_data());
}

void compare_merkles_keccak_with_data() {
    const size_t num_leaves = 6;
    MerkleTreeCreator merkle_tree(std::string(config.BLS_CONTRACT_ADDRESS), provider);
    {
      merkle::TreeT<32, cncrypto_keccak256> tree;
      for (size_t i = 0; i < num_leaves; i++)
      {
        auto h = createHashTFromKeccak(std::to_string(i));
        tree.insert(h);
        merkle_tree.addLeaf(std::to_string(i));
      }
      auto root = tree.root();
      auto path = tree.path(num_leaves - 1);
      REQUIRE(path->verify(root));
      REQUIRE(merkle_tree.getRoot() == root.to_string());
    }
}
TEST_CASE( "compare merkles using keccak in cpp", "[merkle]" ) {
    REQUIRE_NOTHROW(compare_merkles_keccak_with_data());
}
TEST_CASE("Send Merkle hash to contract") {
    const size_t num_leaves = 6;
    for (size_t i = 0; i < num_leaves; i++)
    {
        merkle_contract.addLeaf(std::to_string(i));
    }
    auto tx = merkle_contract.updateRewardsMerkleRoot();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    REQUIRE(provider->transactionSuccessful(hash));

}

TEST_CASE("Send Merkle hash to contract and validate Proof") {
    GENERATE(range(0, 1000)); // This will run the test multiple times
    merkle_contract.tree = {};
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> num_leaves_dis(1000, 5000);  // adjust range for num_leaves
    std::uniform_int_distribution<> dis(0, 10000);  // adjust range as necessary

    size_t num_leaves = num_leaves_dis(gen);
    size_t start = dis(gen);
    for (size_t i = start; i < start + num_leaves; i++)
    {
        merkle_contract.addLeaf(std::to_string(i));
    }

    std::string our_address = signer.addressFromPrivateKey(seckey);
    merkle_contract.addLeaf(merkle_contract.abiEncode(our_address,10000));
    auto tx = merkle_contract.updateRewardsMerkleRoot();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    REQUIRE(provider->transactionSuccessful(hash));
    size_t id = merkle_contract.findIndex(merkle_contract.abiEncode(our_address,10000));
    tx = merkle_contract.validateProof(id, 10000);
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    REQUIRE(provider->transactionSuccessful(hash));

}

TEST_CASE("Just get a good merkle with lots of nodes") {
    const size_t num_leaves = 50;
    merkle_contract.tree = {};
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dis(0, 40000);  // adjust range as necessary

    size_t start = dis(gen);
    merkle_contract.addLeaf("1");
    for (size_t i = start; i < start + num_leaves; i++)
    {
        merkle_contract.addLeaf(std::to_string(i));
    }

    REQUIRE(merkle_contract.tree.num_leaves() == 51);
    merkle_contract.tree.root();
}

TEST_CASE("ABI encode") {
    std::string our_address = signer.addressFromPrivateKey(seckey);
    REQUIRE(merkle_contract.contractHash(our_address,10000) == "0x" + utils::toHexString(utils::hash(merkle_contract.abiEncode(our_address,10000))));
}
