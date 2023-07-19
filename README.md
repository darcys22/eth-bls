# Ethereum BLS 

## Useful links
https://sepolia.etherscan.io/pushTx

## Building the project

```bash
mkdir build/ && cd build/
cmake ..
make -j16
```

## Running the tests

```bash
cd build          # if not in the build directory already
make test
```

## Deploy contract
In the hardhat folder you can call 
```
make deploy
```

which will deploy a contract to the sepolia network for BLS Validators. It will save an artifact of the contracts address to
./hardhat/artifacts/BLS.address

this is currently
```
0x13f87B92D8fEB908C20978C6A4424444a3F6d801
```
