add_subdirectory(cpr)
set(SECP256K1_ENABLE_MODULE_RECOVERY ON CACHE BOOL "" FORCE)
add_subdirectory(secp256k1)
add_subdirectory(Catch2)
