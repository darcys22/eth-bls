#include <iostream> 

#include "eth-bls/tmp.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}

TEST_CASE( "TempAddTest", "[tmp]" ) {
    REQUIRE( tmp::add(1,2) == 3 );
}

TEST_CASE( "TempRequestTest", "[tmp]" ) {
    REQUIRE( tmp::request() == 200 );
}

TEST_CASE( "TempECDSATest", "[tmp]" ) {
    REQUIRE( tmp::ecdsa() == 0 );
}

TEST_CASE( "TempHashTest", "[tmp]" ) {

    std::string hash_hello_world = tmp::toHexString(tmp::hash("hello world!"));
    std::cout << hash_hello_world << '\n';
    REQUIRE( hash_hello_world == "57caa176af1ac0433c5df30e8dabcd2ec1af1e92a26eced5f719b88458777cd6" );
}
