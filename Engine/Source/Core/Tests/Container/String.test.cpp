#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#include <cstring>
#include "Core/Container/String.hpp"
#include "TestingMinimal.hpp"

using namespace Oggle;

// But also once it does we'll still need this to "bootstrap" test cases until operator== can be shown to work, from which point operator== can be relied on
#define IS_EQUAL_IMPL(String, CString) (strncmp(String.CStr(), CString, String.Length() < strlen(CString) ? String.Length() : strlen(CString)) == 0)
#define CHECK_IS_EQUAL(String, CString) CHECK(IS_EQUAL_IMPL(String, CString))
#define CHECK_IS_SMALL(String) CHECK_EQ(String.Capacity(), 15)
#define CHECK_IS_LARGE(String) CHECK_GE(String.Capacity(), 16)
#define CHECK_IS_EMPTY(String) CHECK((String.IsEmpty() && String.Length() == 0))
#define CHECK_NOT_EMPTY(String) CHECK((!String.IsEmpty() && String.Length() != 0))

#define CHECK_MATCHES_REFERENCE(String, Reference) \
    CHECK_NOT_EMPTY(String)                        \
    CHECK_IS_EQUAL(String, Reference.String)       \
    CHECK_EQ(String.Length(), Reference.Length)    \
    CHECK_GE(String.Capacity(), Reference.Length)


constexpr size_t LargeStringThreshold = 16;

struct ReferenceString
{
    const char* String;
    size_t Length;
};

constexpr ReferenceString VeryShortReference     { .String = "A",                .Length = 1  };
constexpr ReferenceString ShortReference         { .String = "Hello",            .Length = 5  };
constexpr ReferenceString LargestSmallReference  { .String = "15   characters",  .Length = 15 };
constexpr ReferenceString SmallestLargeReference { .String = "16    characters", .Length = 16 };
constexpr ReferenceString VeryLongReference      { .String = " According to all known laws of aviation, there is no way a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyway because bees don't care what humans think is impossible.", .Length = 243 };

TEST_SUITE_BEGIN("String");

TEST_CASE("Construction")
{
    SUBCASE("String()")
    {
        String Default {};
        CHECK_IS_EMPTY(Default);
        CHECK_IS_SMALL(Default);
        CHECK_IS_EQUAL(Default, "");
    }

    SUBCASE("String(size_t)")
    {

        SUBCASE("Small String")
        {
            for (size_t i = 0; i < LargeStringThreshold; i++)
            {
                String Small { i };
                CHECK_IS_EMPTY(Small);
                CHECK_IS_SMALL(Small);
                CHECK_IS_EQUAL(Small, "");
            }
        }

        SUBCASE("Large String")
        {
            for (size_t i = LargeStringThreshold; i < 64; i++)
            {
                String Large { i };
                CHECK_IS_EMPTY(Large);
                CHECK_IS_LARGE(Large);
                CHECK_IS_EQUAL(Large, "");
            }
        }
    }

    SUBCASE("String(size_t, Char)")
    {
        SUBCASE("Small String")
        {
            String VeryShort { 3, 'A' };
            CHECK_NOT_EMPTY(VeryShort);
            CHECK_IS_SMALL(VeryShort);
            CHECK_EQ(VeryShort.Length(), 3);
            CHECK_IS_EQUAL(VeryShort, "AAA");

            String Short { 5, 'A' };
            CHECK_NOT_EMPTY(Short);
            CHECK_IS_SMALL(Short);
            CHECK_EQ(Short.Length(), 5);
            CHECK_IS_EQUAL(Short, "AAAAA");

            String LargestSmall { 15, 'A' };
            CHECK_NOT_EMPTY(LargestSmall);
            CHECK_IS_SMALL(LargestSmall);
            CHECK_EQ(LargestSmall.Length(), 15);
            CHECK_IS_EQUAL(LargestSmall, "AAAAAAAAAAAAAAA");
        }
        SUBCASE("Large String")
        {
            String SmallestLarge { 16, 'A' };
            CHECK_NOT_EMPTY(SmallestLarge);
            CHECK_IS_LARGE(SmallestLarge);
            CHECK_EQ(SmallestLarge.Length(), 16);
            CHECK_IS_EQUAL(SmallestLarge, "AAAAAAAAAAAAAAAA");

            String Large { 256, 'A' };
            CHECK_NOT_EMPTY(SmallestLarge);
            CHECK_IS_LARGE(SmallestLarge);
            CHECK_EQ(SmallestLarge.Length(), 256);
            CHECK_IS_EQUAL(SmallestLarge, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
        }
    }

    SUBCASE("String(const char*)")
    {
        SUBCASE("Small String")
        {
            String Hello { "Hello" };
            CHECK(!Hello.IsEmpty());
            CHECK_EQ(Hello.Length(), 5);
            CHECK_GE(Hello.Capacity(), 15);
            CHECK_IS_EQUAL(Hello, "Hello");
        }

        SUBCASE("Large String")
        {
            String Long { "According to all known laws of aviation, there is no way a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyway because bees don't care what humans think is impossible." };
            CHECK(!Long.IsEmpty());
            CHECK_EQ(Long.Length(), 242);
            CHECK_GE(Long.Capacity(), 242);
            CHECK_IS_EQUAL(Long, "According to all known laws of aviation, there is no way a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyway because bees don't care what humans think is impossible.");
        }
    }
}

TEST_SUITE_END();