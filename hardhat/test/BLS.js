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
        it("Should be able to verify an index aggregate signature", async function () {
            //const { validators, owner } = await loadFixture(deployFixture);
            //const indexes = [1,3,5];
            //await expect(validators.checkSigAGGIndices(
                //"0x16e8866fe2a68fc5337e943f4681559e733cefbde3ec224578b02b4649fda60c",
                //"0x289e64119f011110f48ca1b3046e0fadae137876f0a10d6ba20a86754526cd0d",
                //"0x10d53acf55962565c3140e19d3eaecbf480786e45119b29cf8935b21699b2051",
                //"0x107a0a2b2d7c2164f65490e841e41ecd98fca3cd4ec32798c569eef042245498",
                //"0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38",
                //indexes
            //)).to.not.be.reverted;
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
        it("yet another mapped point should be correct", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            const [x, y] = await validators.mapToG2("0x15bd3ce911dc2977dc7bd2be78bf7a3c076f6d4870752ae6ca8ee08c32eb6028");
            expect(x[1]).to.equal("0x15bd3ce911dc2977dc7bd2be78bf7a3c076f6d4870752ae6ca8ee08c32eb602b")
            expect(x[0]).to.equal("0x000000000000000000000000000000000000000000000000000000000000000")
            expect(y[1]).to.equal("0x1ccfbab47118bd8920571f0c2c252f9313a832ee87852d6b3bc4c9e80140ae1")
            expect(y[0]).to.equal("0x11d819688fc0d759a1ef9394195c564b361c5a4a3a66c436c6991907aab8c6f")

        });
        it("should be able to hash to a point", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            await expect(validators.hashToG2("0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38")).to.not.be.reverted;
        });
        it("hashed point should be correct", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            const [x, y] = await validators.hashToG2("0x2a1e00efd15f90ee1e701f07eefc54ebe4ea96adb82c3112ae21140d3c529a38");
            expect(x[1]).to.equal("0x0109cf20e6670180cb7cc1d82133b8048ba9bc470a67bfca753a2eeb121149cb")
            expect(x[0]).to.equal("0x20fc77309d388d60c855607e59c085148d1b3a51bac8da75374318304763794e")
            expect(y[1]).to.equal("0x24e3d166dbfbd1866cb98b706de8b97dcbb05d751b6615a0a40a018b6aaa25d3")
            expect(y[0]).to.equal("0x25617fbf911ddde56a0bfd3c681cdc360fbaa70dd2e653c22d6f7f3c562e65f5")

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

    describe("Hash to Modulus", function () {
        it("should be able to mask bits", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            await expect(await validators.maskBits("0x06b3dfaec148fb1bb2b066f10ec285e7c9bf402ab32aa78a5d38e34566810cd2")).to.equal("0x06b3dfaec148fb1bb2b066f10ec285e7c9bf402ab32aa78a5d38e34566810c12");
        });
        it("should be able to mask bits with modulus", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            await expect(await validators.maskBits("0x594bdadfed01a197b7fec21d5164d80b03510415d678e5a0791257ade8ba7433")).to.equal("0x594bdadfed01a197b7fec21d5164d80b03510415d678e5a0791257ade8ba7413");
        });
        it("should be able to reverse bytes", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            await expect(await validators.byteSwap("0x06b3dfaec148fb1bb2b066f10ec285e7c9bf402ab32aa78a5d38e34566810c12")).to.equal("0x120c816645e3385d8aa72ab32a40bfc9e785c20ef166b0b21bfb48c1aedfb306");
        });
        it("should be able to hash modulus", async function () {
            const { validators, owner } = await loadFixture(deployFixture);
            await expect(await validators.hashToField("Hello")).to.equal("0x120c816645e3385d8aa72ab32a40bfc9e785c20ef166b0b21bfb48c1aedfb306");
        });
    });
// End        
});
