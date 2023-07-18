#include <iostream>

#include "eth-bls/bls_validator_contract.hpp"
#include "eth-bls/config.hpp"
#include "eth-bls/service_node_list.hpp"
#include "eth-bls/signer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

//inline constexpr std::string_view PRIVATE_KEY = "96a656cbd64281ea82257ca9978093b25117592287e4e07f5be660d1701f03e9";
//const auto& config = ethbls::get_config(ethbls::network_type::SEPOLIA);
inline constexpr std::string_view PRIVATE_KEY = "ac0974bec39a17e36ba4a6b4d238ff944bacb478cbed5efcae784d7bf4f2ff80";
const auto& config = ethbls::get_config(ethbls::network_type::GANACHE);
auto provider = std::make_shared<Provider>("Sepolia Client", std::string(config.RPC_URL));
BLSValidatorsContract bls_contract(std::string(config.BLS_CONTRACT_ADDRESS), provider);
Signer signer(provider);
std::vector<unsigned char> seckey = utils::fromHexString(std::string(PRIVATE_KEY));
     
TEST_CASE( "insert pubkeys check they exist and clear", "[bls contract]" ) {
    SKIP("slow test");
    ServiceNodeList snl(3);
    auto tx = bls_contract.clear();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    auto height_included = provider->waitForTransaction(hash);
    REQUIRE(height_included > 0);
    REQUIRE(bls_contract.getValidatorsLength() == 0);
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        tx = bls_contract.addValidator(pubkey);
        hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
    }
    height_included = provider->waitForTransaction(hash);
    REQUIRE(height_included > 0);
    REQUIRE(bls_contract.getValidatorsLength() == 3);
    tx = bls_contract.clear();
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    height_included = provider->waitForTransaction(hash);
    REQUIRE(height_included > 0);
    REQUIRE(bls_contract.getValidatorsLength() == 0);
}

TEST_CASE( "Prints an aggregate pubkey", "[bls contract]" ) {
    SKIP("unsolvable");
    bls::init();
	bls::SecretKey sec0;
	const int n = 2;
	sec0.init();
	bls::SecretKeyVec msk;
	sec0.getMasterSecretKey(msk, n);
	bls::SecretKeyVec allPrvVec(n);
	bls::IdVec allIdVec(n);
	for (unsigned int i = 0; i < n; i++) {
		unsigned int id = i + 1;
		allPrvVec[i].set(msk, id);
		allIdVec[i] = id;

		bls::SecretKey p;
		p.set(msk.data(), n, id);
		REQUIRE(allPrvVec[i] == p);
	}
	for (size_t i = 0; i < n; i++) {
		bls::PublicKey pub;
		allPrvVec[i].getPublicKey(pub);

        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - pub: " << pub << " - debug\n";
	}
    bls::PublicKey aggpub;
    sec0.getPublicKey(aggpub);
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - agg pub: " << aggpub << " - debug\n";
    bls::SecretKey sec2;
    sec2.recover(allPrvVec, allIdVec);
    bls::PublicKey aggpub2;
    sec2.getPublicKey(aggpub2);
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - agg pub: " << aggpub2 << " - debug\n";
    
    bls::PublicKey aggpubx; 
    bls::PublicKey aggpub3; 
    allPrvVec[0].getPublicKey(aggpub3);
    aggpubx.add(aggpub3);
    bls::PublicKey aggpub4; 
    allPrvVec[1].getPublicKey(aggpub4);
    aggpub3.add(aggpub4);
    aggpubx.add(aggpub4);
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - agg pub: " << aggpub3 << " - debug\n";
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - agg pub: " << aggpubx << " - debug\n";

}

TEST_CASE( "Checks an aggregate pubkey", "[bls contract]" ) {
    bls::init();
    ServiceNodeList snl(1);
    bls::PublicKey aggpubx; 
    //auto tx = bls_contract.clear();
    //auto hash = signer.sendTransaction(tx, seckey);
    //REQUIRE(hash != "");
    const std::string message = "You are allowed to leave";
    for(auto& node : snl.nodes) {
        aggpubx.add(node.getPublicKey());
        const auto pubkey = node.getPublicKeyHex();
        //std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - pub: " << node.getPublicKey().getStr(16) << " - debug\n";
        std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - pub: " << pubkey << " - debug\n";
        //tx = bls_contract.addValidator(pubkey);
        //hash = signer.sendTransaction(tx, seckey);
        //REQUIRE(hash != "");
    }
    //auto tx2 = bls_contract.checkAggPubkey(aggpubx.serializeToHexStr());
    
    //hash = signer.sendTransaction(tx2, seckey);
    //REQUIRE(hash != "");
    //auto height_included = provider->waitForTransaction(hash);
    //REQUIRE(height_included > 0);
    //tx = bls_contract.clear();
    //hash = signer.sendTransaction(tx, seckey);
    //REQUIRE(hash != "");
}

TEST_CASE( "Signs an aggregate signature and checks its valid", "[bls contract]" ) {
    SKIP("Not implemented");
    ServiceNodeList snl(4);
    std::vector<std::string> sigs;
    auto tx = bls_contract.clear();
    auto hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
    const std::string message = "You are allowed to leave";
    for(auto& node : snl.nodes) {
        const auto pubkey = node.getPublicKeyHex();
        tx = bls_contract.addValidator(pubkey);
        hash = signer.sendTransaction(tx, seckey);
        REQUIRE(hash != "");
        sigs.push_back(node.sign(message).serializeToHexStr());
    }
    auto tx2 = bls_contract.checkSigAGG(sigs[0], sigs[1], sigs[2], sigs[3], message);
    
    hash = signer.sendTransaction(tx2, seckey);
    REQUIRE(hash != "");
    std::cout << __FILE__ << ":" << __LINE__ << " (" << __func__ << ") TODO sean remove this - hash: " << hash << " - debug\n";
    auto height_included = provider->waitForTransaction(hash);
    REQUIRE(height_included > 0);
    tx = bls_contract.clear();
    hash = signer.sendTransaction(tx, seckey);
    REQUIRE(hash != "");
}
