set(sources
    src/tmp.cpp
    src/utils.cpp
    src/EthereumClient.cpp
    src/signer.cpp
)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
    include/eth-bls/tmp.hpp
    include/eth-bls/utils.hpp
    include/eth-bls/config.hpp
    include/eth-bls/EthereumClient.hpp
    include/eth-bls/signer.hpp
    include/eth-bls/ecdsa_util.h
)

set(test_sources
  src/tmp_test.cpp
  src/ethereum_client_test.cpp
)
