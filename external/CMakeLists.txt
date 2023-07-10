add_subdirectory(cpr)
set(SECP256K1_ENABLE_MODULE_RECOVERY ON CACHE BOOL "" FORCE)
add_subdirectory(secp256k1)
add_subdirectory(Catch2)
add_subdirectory(bls)


# RLPValue is built using autotools
include(ExternalProject)

set(RLP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rlpvalue)
set(RLP_BIN ${CMAKE_CURRENT_BINARY_DIR}/librlpvalue)
set(RLP_SHARED_LIB ${RLP_BIN}/lib/librlpvalue.so)
set(RLP_INCLUDES ${RLP_BIN}/include)

file(MAKE_DIRECTORY ${RLP_INCLUDES})

ExternalProject_Add(
    librlpvalue
    PREFIX ${RLP_BIN}
    SOURCE_DIR ${RLP_DIR}
    DOWNLOAD_COMMAND cd ${RLP_DIR} && git clean -dfX && ${RLP_DIR}/autogen.sh
    CONFIGURE_COMMAND ${RLP_DIR}/configure --srcdir=${RLP_DIR} --prefix=${RLP_BIN} --enable-shared=yes --disable-static
    BUILD_COMMAND make
    INSTALL_COMMAND make install
    BUILD_BYPRODUCTS ${RLP_SHARED_LIB}
)

add_library(rlpvalue SHARED IMPORTED GLOBAL)

add_dependencies(rlpvalue librlpvalue)

set_target_properties(rlpvalue PROPERTIES IMPORTED_LOCATION ${RLP_SHARED_LIB})
set_target_properties(rlpvalue PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${RLP_INCLUDES})
