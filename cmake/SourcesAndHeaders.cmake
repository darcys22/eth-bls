set(sources
    src/tmp.cpp
    src/utils.cpp
    src/ec_utils.cpp
    src/provider.cpp
    src/signer.cpp
    src/transaction.cpp
    src/service_node_list.cpp
    src/bls_validator_contract.cpp
)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
    include/eth-bls/tmp.hpp
    include/eth-bls/utils.hpp
    include/eth-bls/ec_utils.hpp
    include/eth-bls/config.hpp
    include/eth-bls/provider.hpp
    include/eth-bls/signer.hpp
    include/eth-bls/transaction.hpp
    include/eth-bls/service_node_list.hpp
    include/eth-bls/bls_validator_contract.hpp
    include/eth-bls/ecdsa_util.h
)

set(test_sources
  src/tmp_test.cpp
  src/ethereum_client_test.cpp
  src/rlp_test.cpp
  src/initial_bls_library_test.cpp
  src/mcl_test.cpp
  src/service_node_list_test.cpp
  src/bls_validator_contract_test.cpp
  src/verify.cpp
)
