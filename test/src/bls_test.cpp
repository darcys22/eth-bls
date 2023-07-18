#define MCLBN_FP_UNIT_SIZE 4
#define MCLBN_FR_UNIT_SIZE 4
#define BLS_ETH
#include <bls/bls.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

struct BLSInitializer {
    BLSInitializer() {
        // init library once before calling the other APIs
        bls::init();
    }
    ~BLSInitializer() {
    }
};

BLSInitializer blsInitializer;

TEST_CASE( "k of n test", "[bls]" ) {
	const std::string m = "abc";
	const int n = 5;
	const int k = 3;
	bls::SecretKey sec0;
	sec0.init();
	bls::Signature sig0;
	sec0.sign(sig0, m);
	bls::PublicKey pub0;
	sec0.getPublicKey(pub0);
	REQUIRE(sig0.verify(pub0, m));

	bls::SecretKeyVec msk;
	sec0.getMasterSecretKey(msk, k);

	bls::SecretKeyVec allPrvVec(n);
	bls::IdVec allIdVec(n);
	for (unsigned int i = 0; i < n; i++) {
		unsigned int id = i + 1;
		allPrvVec[i].set(msk, id);
		allIdVec[i] = id;

		bls::SecretKey p;
		p.set(msk.data(), k, id);
		REQUIRE(allPrvVec[i] == p);
	}

	bls::SignatureVec allSigVec(n);
	for (size_t i = 0; i < n; i++) {
		REQUIRE(allPrvVec[i] != sec0);
		allPrvVec[i].sign(allSigVec[i], m);
		bls::PublicKey pub;
		allPrvVec[i].getPublicKey(pub);
		REQUIRE(pub != pub0);
		REQUIRE(allSigVec[i].verify(pub, m));
	}

	/*
		3-out-of-n
		can recover
	*/
	bls::SecretKeyVec secVec(3);
	bls::IdVec idVec(3);
	for (size_t a = 0; a < n; a++) {
		secVec[0] = allPrvVec[a];
		idVec[0] = allIdVec[a];
		for (size_t b = a + 1; b < n; b++) {
			secVec[1] = allPrvVec[b];
			idVec[1] = allIdVec[b];
			for (size_t c = b + 1; c < n; c++) {
				secVec[2] = allPrvVec[c];
				idVec[2] = allIdVec[c];
				bls::SecretKey sec;
				sec.recover(secVec, idVec);
				REQUIRE(sec == sec0);
				bls::SecretKey sec2;
				sec2.recover(secVec.data(), idVec.data(), secVec.size());
				REQUIRE(sec == sec2);
			}
		}
	}
	{
		secVec[0] = allPrvVec[0];
		secVec[1] = allPrvVec[1];
		secVec[2] = allPrvVec[0]; // same of secVec[0]
		idVec[0] = allIdVec[0];
		idVec[1] = allIdVec[1];
		idVec[2] = allIdVec[0];
		bls::SecretKey sec;
		REQUIRE_THROWS_WITH(sec.recover(secVec, idVec), "blsSecretKeyRecover:same id");
	}
	{
		/*
			n-out-of-n
			can recover
		*/
		bls::SecretKey sec;
		sec.recover(allPrvVec, allIdVec);
		REQUIRE(sec == sec0);
	}
	/*
		2-out-of-n
		can't recover
	*/
	secVec.resize(2);
	idVec.resize(2);
	for (size_t a = 0; a < n; a++) {
		secVec[0] = allPrvVec[a];
		idVec[0] = allIdVec[a];
		for (size_t b = a + 1; b < n; b++) {
			secVec[1] = allPrvVec[b];
			idVec[1] = allIdVec[b];
			bls::SecretKey sec;
			sec.recover(secVec, idVec);
			REQUIRE(sec != sec0);
		}
	}
	/*
		3-out-of-n
		can recover
	*/
	bls::SignatureVec sigVec(3);
	idVec.resize(3);
	for (size_t a = 0; a < n; a++) {
		sigVec[0] = allSigVec[a];
		idVec[0] = allIdVec[a];
		for (size_t b = a + 1; b < n; b++) {
			sigVec[1] = allSigVec[b];
			idVec[1] = allIdVec[b];
			for (size_t c = b + 1; c < n; c++) {
				sigVec[2] = allSigVec[c];
				idVec[2] = allIdVec[c];
				bls::Signature sig;
				sig.recover(sigVec, idVec);
				REQUIRE(sig == sig0);
			}
		}
	}
	{
		sigVec[0] = allSigVec[1]; idVec[0] = allIdVec[1];
		sigVec[1] = allSigVec[4]; idVec[1] = allIdVec[4];
		sigVec[2] = allSigVec[3]; idVec[2] = allIdVec[3];
	}
	{
		/*
			n-out-of-n
			can recover
		*/
		bls::Signature sig;
		sig.recover(allSigVec, allIdVec);
		REQUIRE(sig == sig0);
	}
	/*
		2-out-of-n
		can't recover
	*/
	sigVec.resize(2);
	idVec.resize(2);
	for (size_t a = 0; a < n; a++) {
		sigVec[0] = allSigVec[a];
		idVec[0] = allIdVec[a];
		for (size_t b = a + 1; b < n; b++) {
			sigVec[1] = allSigVec[b];
			idVec[1] = allIdVec[b];
			bls::Signature sig;
			sig.recover(sigVec, idVec);
			REQUIRE(sig != sig0);
		}
	}
	// return same value if n = 1
	sigVec.resize(1);
	idVec.resize(1);
	sigVec[0] = allSigVec[0];
	idVec[0] = allIdVec[0];
	{
		bls::Signature sig;
		sig.recover(sigVec, idVec);
		REQUIRE(sig == sigVec[0]);
	}
	// share and recover publicKey
	{
		bls::PublicKeyVec pubVec(k);
		idVec.resize(k);
		// select [0, k) publicKey
		for (size_t i = 0; i < k; i++) {
			allPrvVec[i].getPublicKey(pubVec[i]);
			idVec[i] = allIdVec[i];
		}
		bls::PublicKey pub;
		pub.recover(pubVec, idVec);
		REQUIRE(pub == pub0);
		bls::PublicKey pub2;
		pub2.recover(pubVec.data(), idVec.data(), pubVec.size());
		REQUIRE(pub == pub2);
	}
}
