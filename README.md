# Ethereum BLS 

A prototype to generate a large amount of simulated service nodes. Each service node joins through a list maintained in a ethereum smart contract and maintains a bls key. When a service node leaves the majority of other service nodes have to sign a message using an aggregated BLS signature.

## Building the project

```bash
mkdir build/ && cd build/
cmake ..
make -j16
```

## Running the tests

```bash
cd build            # if not in the build directory already
make test
```

## Deploy contract
```
cd hardhat          # if not in the hardhat directory already
make deploy
```

which will deploy a contract to the sepolia network for BLS Validators. It will save an artifact of the contracts address to
./hardhat/artifacts/BLS.address

this is currently
```
0x13f87B92D8fEB908C20978C6A4424444a3F6d801
```

You can follow this up with 
```
make update-config
```

which will update the /include/eth-bls/config.hpp file to use the correct deployed address

## Useful links
https://sepolia.etherscan.io/pushTx
https://blog.quarkslab.com/technical-assessment-of-the-herumi-libraries.html

