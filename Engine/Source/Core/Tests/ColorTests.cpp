// #include "TestMinimal.hpp"
//
// #define TEST_NAME ColorTest
//
// #define TEST_LOG(Verbosity, Message) LOG(Testing, Verbosity, FMT("[{}] {}", TEST_NAME, Message))
//
// int main()
// {
//     for (uint32 i = 0; i < 256; ++i)
//     {
//         const auto Value = static_cast<uint8>(i);
//         const auto FastValue = Oggle::ColorSpaceConversions::sRGBByteToLinearFloatFast(Value);
//         const auto SlowValue = Oggle::ColorSpaceConversions::sRGBByteToLinearFloatSlow(Value);
//         if (FastValue != SlowValue)
//         {
//             TEST_LOG(Testing, Error, FMT("[{}] {}", TEST_NAME, "Foo"))
//         }
//     }
//
//     for (uint32 i = 0; i < 4096; ++i)
//     {
//         const auto Value = 4096.f / static_cast<float>(i);
//         const auto FastValue = Oggle::ColorSpaceConversions::LinearFloatTosRGBByteFast(Value);
//         const auto SlowValue = Oggle::ColorSpaceConversions::LinearFloatTosRGBByteSlow(Value);
//
//         if (FastValue != SlowValue)
//         {
//             TEST_LOG(Testing, Error, FMT("[{}] {}", TEST_NAME, "Foo"))
//         }
//     }
// }
//
// #undef TEST_LOG