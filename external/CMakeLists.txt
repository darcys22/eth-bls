add_subdirectory(secp256k1)
add_subdirectory(Catch2)

# cpr configuration.  Ideally we'd just do this via add_subdirectory, but cpr's cmake requires
# 3.15+, and we target lower than that (and this is fairly simple to build).

if(NOT BUILD_STATIC_DEPS)
  find_package(CURL REQUIRED COMPONENTS HTTP HTTPS SSL)

  # CURL::libcurl wasn't added to FindCURL until cmake 3.12, so add it if necessary
  if (CMAKE_VERSION VERSION_LESS 3.12 AND NOT TARGET CURL::libcurl)
    add_library(libcurl UNKNOWN IMPORTED GLOBAL)
    set_target_properties(libcurl PROPERTIES
      IMPORTED_LOCATION ${CURL_LIBRARIES}
      INTERFACE_INCLUDE_DIRECTORIES "${CURL_INCLUDE_DIRS}")
    add_library(CURL_libcurl INTERFACE)
    target_link_libraries(CURL_libcurl INTERFACE libcurl)
    add_library(CURL::libcurl ALIAS CURL_libcurl)
  endif()
endif()

file(GLOB cpr_sources ${conf_depends} cpr/cpr/*.cpp)

add_library(cpr STATIC EXCLUDE_FROM_ALL ${cpr_sources})
target_link_libraries(cpr PUBLIC CURL::libcurl)
target_include_directories(cpr PUBLIC cpr/include)
target_compile_definitions(cpr PUBLIC CPR_CURL_NOSIGNAL)
add_library(cpr::cpr ALIAS cpr)

file(READ cpr/CMakeLists.txt cpr_cmake_head LIMIT 1000)
if(cpr_cmake_head MATCHES "project\\(cpr VERSION ([0-9]+)\.([0-9]+)\.([0-9]+) LANGUAGES CXX\\)")
    set(cpr_VERSION_MAJOR ${CMAKE_MATCH_1})
    set(cpr_VERSION_MINOR ${CMAKE_MATCH_2})
    set(cpr_VERSION_PATCH ${CMAKE_MATCH_3})
    set(cpr_VERSION "${cpr_VERSION_MAJOR}.${cpr_VERSION_MINOR}.${cpr_VERSION_PATCH}")
    set(cpr_VERSION_NUM "(${cpr_VERSION_MAJOR} * 0x10000 + ${cpr_VERSION_MINOR} * 0x100 + ${cpr_VERSION_PATCH})")

    configure_file(cpr/cmake/cprver.h.in "${CMAKE_CURRENT_BINARY_DIR}/cpr_generated_includes/cpr/cprver.h")
    target_include_directories(cpr PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/cpr_generated_includes")
else()
    message(FATAL_ERROR "Could not identify cpr submodule version!")
endif()
