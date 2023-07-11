require("@nomicfoundation/hardhat-toolbox");
require("@nomicfoundation/hardhat-ethers");

require("hardhat-gas-reporter")
require("hardhat-tracer");

// Sepolia private key
const accounts = ["96a656cbd64281ea82257ca9978093b25117592287e4e07f5be660d1701f03e9"];

/** @type import('hardhat/config').HardhatUserConfig */
module.exports = {
  solidity: "0.8.18",
  gasReporter: {
    gasPrice: 1
  },
  namedAccounts: {
    deployer: {
      default: 1,
    },
  },
  paths: {
    sources: "./contracts",
  },
  networks: {
    mainnet: {
      url: process.env.MAINNET_RPC_URL ?? "",
      chainId: 1,
      accounts,
    },
    sepolia: {
        url: "https://rpc.sepolia.org",
        chainId: 11155111,
        accounts,
    },
    hardhat: {
        chainId: 1337,
    },
  },

};
