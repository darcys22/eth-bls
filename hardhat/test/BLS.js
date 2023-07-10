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

        return { validators, owner, otherAccount };
    }

    describe("Aggregate Signatures", function () {
        //it("Should be able to sign an aggregate signature", async function () {
            //const { validators, owner } = await loadFixture(deployFixture);
            //await expect(validators.testCheckSigAGG()).to.not.be.reverted;
        //});

        it("Should be able to verify an aggregate signature", async function () {
            hre.tracer.enabled = false;
            const { validators, owner } = await loadFixture(deployFixture);
            const thing = Math.floor(Math.random() * 100000) + 1;
            await validators.addValidatorTest(15000, thing, 5);
            //const thing = Math.floor(Math.random() * 100000) + 1;
            //await validators.addValidatorTest(15000, thing, 300);
            //const thing2 = Math.floor(Math.random() * 100000) + 1;
            //await validators.addValidatorTest(15000, thing2, 300);
            //const thing3 = Math.floor(Math.random() * 100000) + 1;
            //await validators.addValidatorTest(15000, thing3, 300);
            //const thing4 = Math.floor(Math.random() * 100000) + 1;
            //await validators.addValidatorTest(15000, thing4, 300);

            hre.tracer.enabled = true;
            await expect(validators.testCheckSigAGG()).to.not.be.reverted;
            hre.tracer.enabled = false;
        });
    });
});
