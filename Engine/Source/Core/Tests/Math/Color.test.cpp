#include "../../../Testing/Public/TestingMinimal.hpp"
#include "Core/Math/Color.hpp"

using namespace Oggle;

TEST_CASE("Color Space Conversions")
{
    SUBCASE("sRGB to Linear")
    {
        for (uint32 i = 0; i < 256; ++i)
        {
            const auto Value = static_cast<uint8>(i);
            const auto FastValue = ColorSpaceConversions::sRGBByteToLinearFloatFast(Value);
            const auto SlowValue = ColorSpaceConversions::sRGBByteToLinearFloatSlow(Value);

            CHECK(FastValue == SlowValue);
        }
    }

    SUBCASE("Linear to sRGB")
    {
        for (uint32 i = 0; i < 4096; ++i)
        {
            const auto Value = 4096.f / static_cast<float>(i);
            const auto FastValue = ColorSpaceConversions::LinearFloatTosRGBByteFast(Value);
            const auto SlowValue = ColorSpaceConversions::LinearFloatTosRGBByteSlow(Value);
            CHECK(FastValue == SlowValue);
        }
    }
}