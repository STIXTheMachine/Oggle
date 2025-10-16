#include <Testing/doctest.h>
#include <Core/TypeTraits.hpp>
#include <Renderer/VertexTypes.hpp>

TEST_CASE("Testing TypesAreUnique_v") {
    SUBCASE("Basic types") {
        SUBCASE("Unqiue"){
            CHECK(TypesAreUnique_v<>);
            CHECK(TypesAreUnique_v<int>);
            CHECK(TypesAreUnique_v<int, float>);
            CHECK(TypesAreUnique_v<int, float, double>);
            CHECK(TypesAreUnique_v<char, signed char, unsigned char>);
        }

        SUBCASE("Nonunique") {
            CHECK(!TypesAreUnique_v<int, int>);
            CHECK(!TypesAreUnique_v<int, float, int>);
            CHECK(!TypesAreUnique_v<int, float, int, float>);
        }

        SUBCASE("Qualified types") {
            CHECK(!TypesAreUnique_v<int, const int  >);
            CHECK(!TypesAreUnique_v<int,       int& >);
            CHECK(!TypesAreUnique_v<int, const int& >);
            CHECK(!TypesAreUnique_v<int,       int&&>);
            CHECK(!TypesAreUnique_v<int, const int&&>);
        }

        SUBCASE("Typedefs") {
            using Foo = int;
            using Bar = char;
            using Baz = float;

            CHECK(TypesAreUnique_v<Foo, Bar, Baz>);

            CHECK(TypesAreUnique_v<int, Bar, Baz>);
            CHECK(TypesAreUnique_v<Foo, char, Baz>);
            CHECK(TypesAreUnique_v<Foo, Bar, float>);

            CHECK(TypesAreUnique_v<Foo, char, float>);
            CHECK(TypesAreUnique_v<int, Bar, float>);
            CHECK(TypesAreUnique_v<int, char, Baz>);

            CHECK(!TypesAreUnique_v<Foo, int>);
            CHECK(!TypesAreUnique_v<Bar, char>);
            CHECK(!TypesAreUnique_v<Baz, float>);
        }
    }

    SUBCASE("VertexAttribute types") {
        CHECK(TypesAreUnique_v<VertexPosition>);
        CHECK(TypesAreUnique_v<VertexPosition, VertexNormal>);
        CHECK(TypesAreUnique_v<VertexPosition, VertexColor>);
        CHECK(TypesAreUnique_v<VertexPosition, VertexNormal, VertexColor>);
        CHECK(TypesAreUnique_v<VertexPosition, VertexNormal, VertexTexCoord>);
    }
}