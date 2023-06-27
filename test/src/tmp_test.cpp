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
