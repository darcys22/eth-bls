const {
  time,
  loadFixture,
} = require("@nomicfoundation/hardhat-toolbox/network-helpers");
const { anyValue } = require("@nomicfoundation/hardhat-chai-matchers/withArgs");
const { expect } = require("chai");

const EthCrypto = require('eth-crypto');
require("@nomicfoundation/hardhat-ethers");

describe("signing", function () {
    it("Should be able to get a private key and sign", async function () {
        const identity = EthCrypto.createIdentity();
        const signerIdentity = EthCrypto.createIdentity();
        //console.log(`identity: ${signerIdentity.privateKey}`);
        const message = EthCrypto.hash.keccak256([
            {type: "string",value: "Hello World!"}
        ]);
        const signature = EthCrypto.sign(signerIdentity.privateKey, message);
        //console.log(`message: ${message}`);
        //console.log(`signature: ${signature}`);
        //console.log(`signer public key: ${signerIdentity.address}`);
    });

    it("Should be able to output raw", async function () {

        var wallet = new ethers.Wallet("96a656cbd64281ea82257ca9978093b25117592287e4e07f5be660d1701f03e9");
        //console.log(wallet.address)

        tx = new ethers.Transaction()
        tx.value = ethers.parseEther("1.0")
        tx.gasLimit = 21000
        tx.nonce = 1
        tx.to = "0xA6C077fd9283421C657EcEa8a9c1422cc6CEbc80"
        tx.chainId = 1
        //console.log(tx.toJSON());
        //console.log(tx.unsignedSerialized);

        //console.log(tx.unsignedHash);

        signature = await wallet.signTransaction(tx);
        //console.log(signature);
        
    });
});
