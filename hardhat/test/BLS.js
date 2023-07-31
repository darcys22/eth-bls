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
            //hre.tracer.enabled = false;
            //const { validators, owner } = await loadFixture(deployFixture);
            //const thing = Math.floor(Math.random() * 100000) + 1;
            //await validators.addValidatorTest(15000, thing, 5);
            //const thing = Math.floor(Math.random() * 100000) + 1;
            //await validators.addValidatorTest(15000, thing, 300);
            //const thing2 = Math.floor(Math.random() * 100000) + 1;
            //await validators.addValidatorTest(15000, thing2, 300);
            //const thing3 = Math.floor(Math.random() * 100000) + 1;
            //await validators.addValidatorTest(15000, thing3, 300);
            //const thing4 = Math.floor(Math.random() * 100000) + 1;
            //await validators.addValidatorTest(15000, thing4, 300);

            //hre.tracer.enabled = true;
            //await expect(validators.testCheckSigAGG()).to.not.be.reverted;
            //hre.tracer.enabled = false;
        });
    });

    describe("Hash to G2", function () {
        it("should be able to map to a point", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            await expect(validators.mapToG2("0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38")).to.not.be.reverted;
        });
        it("mapped point should be correct", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            const [x, y] = await validators.mapToG2("0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38");
            expect(x[1]).to.equal("0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38")
            expect(x[0]).to.equal("0x0000000000000000000000000000000000000000000000000000000000000000")
            expect(y[1]).to.equal("0x025545ea777066d1ab51277be1f1a545dc613876c91f6c499b9fccc01c2f526d")
            expect(y[0]).to.equal("0x24ecc35294ecac042dc5cab3c6a06668361183a6d7535ca788a7f7365e8aba65")

        });
        it("another mapped point should be correct", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            const [x, y] = await validators.mapToG2("0x1ac11af4c60d15ae7d40cdaea97c363b247ab2d8d69868cf5715e8ae9a164ec5");
            expect(x[1]).to.equal("0x1ac11af4c60d15ae7d40cdaea97c363b247ab2d8d69868cf5715e8ae9a164ec5")
            expect(x[0]).to.equal("0x0000000000000000000000000000000000000000000000000000000000000000")
            expect(y[1]).to.equal("0x112b6261c30071bc994b4344badeb99abe4485b74a49c70f95668624994d659d")
            expect(y[0]).to.equal("0x09dc620697a7c0f846d0faf33039943c354cc1864d40b4dda7729c957695a5d9")

        });
        it("should be able to hash to a point", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            await expect(validators.hashToG2("0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38")).to.not.be.reverted;
        });
        it("hashed point should be correct", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            const [x, y] = await validators.hashToG2("0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38");
            expect(x[1]).to.equal("0x1d41a2e4a8736e2c714b15dee463d6cda4e1f2c1322be4b8d1bbf96b733a4e18")
            expect(x[0]).to.equal("0x3061f0216797fe6c86cb1a2270bcd7207295e9a84f3326e2c2e31484b44f8971")
            expect(y[1]).to.equal("0x1efc2846ee4d94c41c377155bbe16eed9a6ae8a89178ddb83467c6de6fbeba52")
            expect(y[0]).to.equal("0x21f63950d1d4fbe1d0b2dfc519553f952c52112b6310cbdc05aa13be001fcdba")

        });
        it("should be to root a point", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            const x = "0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38"
            const y = "0x0000000000000000000000000000000000000000000000000000000000000000"
            const [rootx, rooty] = await validators.rootFp2(x, y);
            expect(rootx).to.equal("0x1c4ff272a534026113b5556a27df71f52eab0997aa2d1e9be229a243d7f1fc6a")
            expect(rooty).to.equal(y)
        });
        it("should be to root a simple imaginary number", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            const x = 3
            const y = 4
            const [rootx, rooty] = await validators.rootFp2(x, y);
            expect(rootx).to.equal(2)
            expect(rooty).to.equal(1)
        });
        it("should be to root a point without a zero y", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            const x = "0x148462dac6853fa3dcae9c5a365b00d89df221a6bd1c31e16c9e1317ed882142"
            const y = "0x009713b03af0fed4cd2cafadeed8fdf4a74fa084e52d1852e4a2bd0685c315d2"
            const [rootx, rooty] = await validators.rootFp2(x, y);
            expect(rootx).to.equal("0x025545ea777066d1ab51277be1f1a545dc613876c91f6c499b9fccc01c2f526d")
            expect(rooty).to.equal("0x24ecc35294ecac042dc5cab3c6a06668361183a6d7535ca788a7f7365e8aba65")
        });
        it("should be get yy coordinate", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            const x = "0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38"
            const y = "0x0000000000000000000000000000000000000000000000000000000000000000"
            const [yyx, yyy] = await validators.getWeierstrass(x, y);
            const expectedx = "0x148462dac6853fa3dcae9c5a365b00d89df221a6bd1c31e16c9e1317ed882142"
            const expectedy = "0x009713b03af0fed4cd2cafadeed8fdf4a74fa084e52d1852e4a2bd0685c315d2"
            expect(yyx).to.equal(expectedx)
            expect(yyy).to.equal(expectedy)
        });
        it("should be get yy coordinate and root it", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            const x = "0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38"
            const y = "0x0000000000000000000000000000000000000000000000000000000000000000"
            const [yyx, yyy] = await validators.getWeierstrass(x, y);
            const expectedx = "0x148462dac6853fa3dcae9c5a365b00d89df221a6bd1c31e16c9e1317ed882142"
            const expectedy = "0x009713b03af0fed4cd2cafadeed8fdf4a74fa084e52d1852e4a2bd0685c315d2"
            expect(yyx).to.equal(expectedx)
            expect(yyy).to.equal(expectedy)
            const [rootx, rooty] = await validators.rootFp2(yyx, yyy);
            expect(rootx).to.equal("0x025545ea777066d1ab51277be1f1a545dc613876c91f6c499b9fccc01c2f526d")
            expect(rooty).to.equal("0x24ecc35294ecac042dc5cab3c6a06668361183a6d7535ca788a7f7365e8aba65")
        });
    });
// End        
});
