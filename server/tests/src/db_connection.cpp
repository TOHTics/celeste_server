#include "catch.hpp"
#include "db/db.hpp"

// Only valid for local development
// since the password, db name and user might change
SCENARIO( "We can connect to the DB", "[mysql]" ) {

    GIVEN( "Given the connection details." ) {
        std::vector<int> v( 5 );
        
        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 5 );
        
        WHEN( "the size is increased" ) {
            v.resize( 10 );
            
            THEN( "the size and capacity change" ) {
                REQUIRE( v.size() == 10 );
                REQUIRE( v.capacity() >= 10 );
            }
        }
    }
}