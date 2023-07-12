[![Actions Status](https://github.com/filipdutescu/modern-cpp-template/workflows/MacOS/badge.svg)](https://github.com/filipdutescu/modern-cpp-template/actions)
[![Actions Status](https://github.com/filipdutescu/modern-cpp-template/workflows/Windows/badge.svg)](https://github.com/filipdutescu/modern-cpp-template/actions)
[![Actions Status](https://github.com/filipdutescu/modern-cpp-template/workflows/Ubuntu/badge.svg)](https://github.com/filipdutescu/modern-cpp-template/actions)
[![codecov](https://codecov.io/gh/filipdutescu/modern-cpp-template/branch/master/graph/badge.svg)](https://codecov.io/gh/filipdutescu/modern-cpp-template)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/filipdutescu/modern-cpp-template)](https://github.com/filipdutescu/modern-cpp-template/releases)

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
0xFea585c8c8F4DDe3ad7a67919589deF8815910a4
```
