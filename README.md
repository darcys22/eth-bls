[![Actions Status](https://github.com/filipdutescu/modern-cpp-template/workflows/MacOS/badge.svg)](https://github.com/filipdutescu/modern-cpp-template/actions)
[![Actions Status](https://github.com/filipdutescu/modern-cpp-template/workflows/Windows/badge.svg)](https://github.com/filipdutescu/modern-cpp-template/actions)
[![Actions Status](https://github.com/filipdutescu/modern-cpp-template/workflows/Ubuntu/badge.svg)](https://github.com/filipdutescu/modern-cpp-template/actions)
[![codecov](https://codecov.io/gh/filipdutescu/modern-cpp-template/branch/master/graph/badge.svg)](https://codecov.io/gh/filipdutescu/modern-cpp-template)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/filipdutescu/modern-cpp-template)](https://github.com/filipdutescu/modern-cpp-template/releases)

# Ethereum BLS 

## Useful links
https://sepolia.etherscan.io/pushTx

## Building the project

### Dependencies
```
sudo apt-get update
sudo apt-get install -y automake cmake build-essential libprocps-dev libtool\
                        pkg-config yasm texinfo autoconf flex bison clang-format-6.0
```

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
