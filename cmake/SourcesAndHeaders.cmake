set(sources
    src/tmp.cpp
    src/EthereumClient.cpp
)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
    include/eth-bls/tmp.hpp
    include/eth-bls/config.hpp
    include/eth-bls/EthereumClient.hpp
)

set(test_sources
  src/tmp_test.cpp
  src/ethereum_client_test.cpp
)
