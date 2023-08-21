const {
  time,
  loadFixture,
} = require("@nomicfoundation/hardhat-toolbox/network-helpers");
const { anyValue } = require("@nomicfoundation/hardhat-chai-matchers/withArgs");
const { expect } = require("chai");

describe("BLS", function () {
    async function deployFixture() {
        // Contracts are deployed using the first signer/account by default
        const [owner, otherAccount] = await ethers.getSigners();

        const Library = await ethers.getContractFactory("BN256G2");
        const library = await Library.deploy();
        await library.waitForDeployment();

        const Validators = await ethers.getContractFactory("BLSValidators", {
            libraries: {
                BN256G2: await library.getAddress()
            }
        });

        const validators = await Validators.deploy();

        return {library, owner};
    }

    describe("Hash to G2 Functions", function () {
        it("Frobenius Fp2 should be correct", async function () {
            const { library, owner } = await loadFixture(deployFixture);
            let P = [
                BigInt("0x0b5e634d0407c021e9e9dd9d03c4965810e236fef0955ab345e1d049a0438ec6"),
                BigInt("0x1dbb7bf2b1f5340d4b5c466a0641b00cd3a9d9588c7bcad1c3158bdcc65c3332"),
            ];
            const Q = await library._FQ2Frobenius(P[0], P[1]);
            expect(Q[0]).to.equal("0x0b5e634d0407c021e9e9dd9d03c4965810e236fef0955ab345e1d049a0438ec6")
            expect(Q[1]).to.equal("0x12a8d2802f3c6c1c6cf3ff4c7b3fa850c3d79138dbf5ffbb790b003a1220ca15")

        });

        it("Frobenius G2 should be correct", async function () {
            const { library, owner } = await loadFixture(deployFixture);
            let P = [
                BigInt("0x0b5e634d0407c021e9e9dd9d03c4965810e236fef0955ab345e1d049a0438ec6"),
                BigInt("0x1dbb7bf2b1f5340d4b5c466a0641b00cd3a9d9588c7bcad1c3158bdcc65c3332"),
                BigInt("0x28ac4e693f8859fd4b63deb41d0c5a1edbb374fc707a11cdb97f1bc879f1fd61"),
                BigInt("0x209697694ff798fbdd9e112d69a54cc2729a16db5919d590425769aa3e4e0576"),
                BigInt("0x2f9cbc2e78ef5446366a205cbfb7c2cb7c89e368b151769037607eb68c460986"),
                BigInt("0x0c3cd26c1916f6e42201395a03acc0cf73865a915fc364179a9348532da36f70"),
            ];
            const Q = await library._ECTwistFrobeniusJacobian(P);
            expect(Q[0]).to.equal("0x04b9ffcb39c1b7636aad16bde8b0debd0145d761b3cda9a938a2288804bbd467")
            expect(Q[1]).to.equal("0x1c6083d80d0693d05ad74516ea1be31cf7b8ba26e7689c497ec55e621c6df17e")
            expect(Q[2]).to.equal("0x06c2cffebb48ee5aeb216a218679e7dc4d5cf86b877432caac58726dfd056be8")
            expect(Q[3]).to.equal("0x25b612c11bace32aa75d108f04b39ee18414b54d689e1c426cd38e1c037a9c38")
            expect(Q[4]).to.equal("0x2f9cbc2e78ef5446366a205cbfb7c2cb7c89e368b151769037607eb68c460986")
            expect(Q[5]).to.equal("0x24277c06c81aa945964f0c5c7dd4978e23fb100008ae6675a18d43c3aad98dd7")

        });

        it("Mul by cofactor should be correct", async function () {
            const { library, owner } = await loadFixture(deployFixture);
            let P = [
                BigInt("0x21de0d424633dcb20a79dcadc33d98bd1db43dfe51ff56fd6fb89be5308785da"),
                BigInt("0x1fbc58ca2fb3c341cf87c20969ef2d34b056174a07ca32af6ab8f073aec962ca"),
                BigInt("0x1ada922e9f7c56a6745756147db7b0b44730ab37f583706f5738e044b232d0bb"),
                BigInt("0x023c8f7cdfccbfdc409efb222b25aaadc26cec63d3fdec567564189674f47748"),
            ];
            const Q = await library.ECTwistMulByCofactor(P[0], P[1], P[2], P[3]);
            expect(Q[0]).to.equal("0x0070228de928d478db66c2b122c92f9261796b3f0ae763127da17b2ed4cafaea")
            expect(Q[1]).to.equal("0x13db1cfa62dd0b4c058f96e52552e70730d094bf97fdd6f9bb65ca7caeb2cf74")
            expect(Q[2]).to.equal("0x2af40700d827a49ff04b6bdfad1434ae7134c3075b75aae39b02f880aec213cb")
            expect(Q[3]).to.equal("0x2e60a76508314426a71c3b67a549317beaaebfcc575143c5238733743a11a1c3")
        });
    });

    describe("sqrt Functions", function () {
        it("sqrt should return root", async function () {
            const { library, owner } = await loadFixture(deployFixture);
            const [rt, has_root] = await library._sqrt("16664866163301480451889907439185309398209184354335903843015254030884734189504");
            expect(has_root).to.equal(true);
        });
    });
// End        
});
