#include "Core/Color.hpp"
#include <cmath>

namespace Oggle
{
namespace ColorSpaceConversions
{
    float sRGBToLinearTable[256] = {
        0.0000000000000000f, 0.0003035269910470f, 0.0006070539820939f, 0.0009105809731409f, 0.0012141079641879f, 0.0015176349552348f, 0.0018211619462818f, 0.0021246888209134f,
        0.0024282159283757f, 0.0027317430358380f, 0.0030352699104697f, 0.0033465356100351f, 0.0036765069235116f, 0.0040247170254588f, 0.0043914420530200f, 0.0047769532538950f,
        0.0051815169863403f, 0.0056053916923702f, 0.0060488325543702f, 0.0065120910294354f, 0.0069954101927578f, 0.0074990317225456f, 0.0080231921747327f, 0.0085681248456240f,
        0.0091340569779277f, 0.0097212176769972f, 0.0103298230096698f, 0.0109600936993957f, 0.0116122448816895f, 0.0122864870354533f, 0.0129830306395888f, 0.0137020805850625f,
        0.0144438436254859f, 0.0152085144072771f, 0.0159962922334671f, 0.0168073754757643f, 0.0176419522613287f, 0.0185002181679010f, 0.0193823613226414f, 0.0202885624021292f,
        0.0212190095335245f, 0.0221738833934069f, 0.0231533646583557f, 0.0241576302796602f, 0.0251868572086096f, 0.0262412223964930f, 0.0273208916187286f, 0.0284260381013155f,
        0.0295568332076073f, 0.0307134408503771f, 0.0318960286676884f, 0.0331047624349594f, 0.0343398079276085f, 0.0356013141572475f, 0.0368894450366497f, 0.0382043644785881f,
        0.0395462363958359f, 0.0409151986241341f, 0.0423114113509655f, 0.0437350273132324f, 0.0451862029731274f, 0.0466650836169720f, 0.0481718219816685f, 0.0497065633535385f,
        0.0512694679200649f, 0.0528606548905373f, 0.0544802807271481f, 0.0561284944415092f, 0.0578054338693619f, 0.0595112405717373f, 0.0612460710108280f, 0.0630100294947624f,
        0.0648032799363136f, 0.0666259527206421f, 0.0684781819581985f, 0.0703601092100143f, 0.0722718611359596f, 0.0742135792970657f, 0.0761853903532028f, 0.0781874284148216f,
        0.0802198275923729f, 0.0822827145457268f, 0.0843762159347534f, 0.0865004658699036f, 0.0886556059122086f, 0.0908417329192162f, 0.0930589810013771f, 0.0953074842691422f,
        0.0975873619318008f, 0.0998987406492233f, 0.1022417470812798f, 0.1046164929866791f, 0.1070231124758720f, 0.1094617173075676f, 0.1119324341416359f, 0.1144353821873665f,
        0.1169706732034683f, 0.1195384338498116f, 0.1221387982368469f, 0.1247718408703804f, 0.1274376958608627f, 0.1301364898681641f, 0.1328683346509933f, 0.1356333494186401f,
        0.1384316235780716f, 0.1412633061408997f, 0.1441284865140915f, 0.1470272839069366f, 0.1499598026275635f, 0.1529261618852615f, 0.1559264659881592f, 0.1589608639478683f,
        0.1620294004678726f, 0.1651322245597839f, 0.1682693958282471f, 0.1714410930871964f, 0.1746473908424377f, 0.1778884083032608f, 0.1811642348766327f, 0.1844749897718430f,
        0.1878207623958588f, 0.1912016719579697f, 0.1946178078651428f, 0.1980693042278290f, 0.2015562355518341f, 0.2050787061452866f, 0.2086368501186371f, 0.2122307270765305f,
        0.2158605307340622f, 0.2195262312889099f, 0.2232279777526855f, 0.2269658893346786f, 0.2307400703430176f, 0.2345506548881531f, 0.2383976578712463f, 0.2422811985015869f,
        0.2462013959884644f, 0.2501583695411682f, 0.2541521787643433f, 0.2581829130649567f, 0.2622507214546204f, 0.2663556635379791f, 0.2704978585243225f, 0.2746773660182953f,
        0.2788943350315094f, 0.2831487953662872f, 0.2874408960342407f, 0.2917706966400146f, 0.2961383163928986f, 0.3005438446998596f, 0.3049873709678650f, 0.3094689548015594f,
        0.3139887452125549f, 0.3185468316078186f, 0.3231432437896729f, 0.3277781307697296f, 0.3324515819549561f, 0.3371636569499969f, 0.3419144451618195f, 0.3467040956020355f,
        0.3515326976776123f, 0.3564002513885498f, 0.3613068759441376f, 0.3662526905536652f, 0.3712377846240997f, 0.3762622177600861f, 0.3813261091709137f, 0.3864295184612274f,
        0.3915725648403168f, 0.3967553079128265f, 0.4019778668880463f, 0.4072403013706207f, 0.4125427007675171f, 0.4178851544857025f, 0.4232677519321442f, 0.4286905527114868f,
        0.4341537058353424f, 0.4396572411060333f, 0.4452012479305267f, 0.4507858455181122f, 0.4564110636711121f, 0.4620770514011383f, 0.4677838385105133f, 0.4735315442085266f,
        0.4793202280998230f, 0.4851499795913696f, 0.4910208880901337f, 0.4969330430030823f, 0.5028865933418274f, 0.5088814496994019f, 0.5149177908897400f, 0.5209956765174866f,
        0.5271152257919312f, 0.5332764983177185f, 0.5394796133041382f, 0.5457245707511902f, 0.5520114898681641f, 0.5583404898643494f, 0.5647116303443909f, 0.5711249113082886f,
        0.5775805115699768f, 0.5840784907341003f, 0.5906189084053040f, 0.5972018837928772f, 0.6038274168968201f, 0.6104956269264221f, 0.6172066330909729f, 0.6239604353904724f,
        0.6307572126388550f, 0.6375969648361206f, 0.6444797515869141f, 0.6514056921005249f, 0.6583748459815979f, 0.6653873324394226f, 0.6724432110786438f, 0.6795425415039062f,
        0.6866854429244995f, 0.6938719153404236f, 0.7011020183563232f, 0.7083759307861328f, 0.7156936526298523f, 0.7230552434921265f, 0.7304608821868896f, 0.7379105687141418f,
        0.7454043626785278f, 0.7529423236846924f, 0.7605246305465698f, 0.7681512832641602f, 0.7758223414421082f, 0.7835379242897034f, 0.7912980318069458f, 0.7991028428077698f,
        0.8069523572921753f, 0.8148466944694519f, 0.8227858543395996f, 0.8307699561119080f, 0.8387991189956665f, 0.8468732833862305f, 0.8549926877021790f, 0.8631572723388672f,
        0.8713672161102295f, 0.8796223402023315f, 0.8879231810569763f, 0.8962693810462952f, 0.9046613574028015f, 0.9130986928939819f, 0.9215820431709290f, 0.9301108717918396f,
        0.9386858940124512f, 0.9473065733909607f, 0.9559735059738159f, 0.9646862745285034f, 0.9734454751014709f, 0.9822505712509155f, 0.9911022186279297f, 1.0000000000000000f,
    };

    float sRGBByteToLinearFloat(const uint8 InSrgb)
    {
        return sRGBToLinearTable[InSrgb];
    }

    uint8 LinearFloatTosRGBByte(float InLinear)
    {
        return InLinear <= 0.0031308f ? InLinear * 12.92f : 1.055f * std::powf(InLinear, 2.4f) - 0.055;
    }

}

namespace Detail
{
    enum ColorHexValues : unsigned int
    {
        White       = 0xFFFFFFFF,
        LightGray   = 0xDFDFDFFF,
        Gray        = 0xBABABAFF,
        DarkGray    = 0x878787FF,
        Black       = 0x000000FF,

        Red         = 0xFF0000FF,
        Green       = 0x00FF00FF,
        Blue        = 0x0000FFFF,

        Yellow      = 0xFFFF00FF,
        Magenta     = 0xFF00FFFF,
        Cyan        = 0x00FFFFFF,

        Emerald     = 0x2ECC71FF,
        Orange      = 0xF39C12FF,
        Purple      = 0xA907FFFF,
        Silver      = 0xBDC3C7FF,
        Turquoise   = 0x1ABC9CFF,

        Transparent = 0x00000000,
    };
}



const ByteColor ByteColor::White       = FromHexRGBA(Detail::ColorHexValues::White);
const ByteColor ByteColor::LightGray   = FromHexRGBA(Detail::ColorHexValues::LightGray);
const ByteColor ByteColor::Gray        = FromHexRGBA(Detail::ColorHexValues::Gray);
const ByteColor ByteColor::DarkGray    = FromHexRGBA(Detail::ColorHexValues::DarkGray);
const ByteColor ByteColor::Black       = FromHexRGBA(Detail::ColorHexValues::Black);

const ByteColor ByteColor::Red         = FromHexRGBA(Detail::ColorHexValues::Red);
const ByteColor ByteColor::Green       = FromHexRGBA(Detail::ColorHexValues::Green);
const ByteColor ByteColor::Blue        = FromHexRGBA(Detail::ColorHexValues::Blue);

const ByteColor ByteColor::Yellow      = FromHexRGBA(Detail::ColorHexValues::Yellow);
const ByteColor ByteColor::Magenta     = FromHexRGBA(Detail::ColorHexValues::Magenta);
const ByteColor ByteColor::Cyan        = FromHexRGBA(Detail::ColorHexValues::Cyan);

const ByteColor ByteColor::Emerald     = FromHexRGBA(Detail::ColorHexValues::Emerald);
const ByteColor ByteColor::Orange      = FromHexRGBA(Detail::ColorHexValues::Orange);
const ByteColor ByteColor::Purple      = FromHexRGBA(Detail::ColorHexValues::Purple);
const ByteColor ByteColor::Silver      = FromHexRGBA(Detail::ColorHexValues::Silver);
const ByteColor ByteColor::Turquoise   = FromHexRGBA(Detail::ColorHexValues::Turquoise);

const ByteColor ByteColor::Transparent = FromHexRGBA(Detail::ColorHexValues::Transparent);



const FloatColor FloatColor::White       = FromHexRGBA(Detail::ColorHexValues::White);
const FloatColor FloatColor::LightGray   = FromHexRGBA(Detail::ColorHexValues::LightGray);
const FloatColor FloatColor::Gray        = FromHexRGBA(Detail::ColorHexValues::Gray);
const FloatColor FloatColor::DarkGray    = FromHexRGBA(Detail::ColorHexValues::DarkGray);
const FloatColor FloatColor::Black       = FromHexRGBA(Detail::ColorHexValues::Black);

const FloatColor FloatColor::Red         = FromHexRGBA(Detail::ColorHexValues::Red);
const FloatColor FloatColor::Green       = FromHexRGBA(Detail::ColorHexValues::Green);
const FloatColor FloatColor::Blue        = FromHexRGBA(Detail::ColorHexValues::Blue);

const FloatColor FloatColor::Yellow      = FromHexRGBA(Detail::ColorHexValues::Yellow);
const FloatColor FloatColor::Magenta     = FromHexRGBA(Detail::ColorHexValues::Magenta);
const FloatColor FloatColor::Cyan        = FromHexRGBA(Detail::ColorHexValues::Cyan);

const FloatColor FloatColor::Emerald     = FromHexRGBA(Detail::ColorHexValues::Emerald);
const FloatColor FloatColor::Orange      = FromHexRGBA(Detail::ColorHexValues::Orange);
const FloatColor FloatColor::Purple      = FromHexRGBA(Detail::ColorHexValues::Purple);
const FloatColor FloatColor::Silver      = FromHexRGBA(Detail::ColorHexValues::Silver);
const FloatColor FloatColor::Turquoise   = FromHexRGBA(Detail::ColorHexValues::Turquoise);

const FloatColor FloatColor::Transparent = FromHexRGBA(Detail::ColorHexValues::Transparent);

// =====================================================================================================================
// ===================================================== ByteColor =====================================================
// =====================================================================================================================
ByteColor::ByteColor() : R(0), G(0), B(0), A(0) {};
ByteColor::ByteColor(ComponentType R, ComponentType G, ComponentType B, ComponentType A)
    : R(R), G(G), B(B), A(A) {};
ByteColor::ByteColor(ComponentType R, ComponentType G, ComponentType B)
    : R(R), G(G), B(B), A(std::numeric_limits<ComponentType>::max()) {};

ByteColor::operator FloatColor() const
{
    static constexpr float ConversionFactor = 1.f / std::numeric_limits<ComponentType>::max();

    return FloatColor {
        ColorSpaceConversions::sRGBByteToLinearFloat(R),
        ColorSpaceConversions::sRGBByteToLinearFloat(G),
        ColorSpaceConversions::sRGBByteToLinearFloat(R),
        ColorSpaceConversions::sRGBByteToLinearFloat(A),
    };
}

FloatColor ByteColor::ToFloatColor() const
{
    return FloatColor { *this };
}

ByteColor ByteColor::FromHexRGB(unsigned int HexRGB)
{
    return FromHexRGBA((HexRGB << 8) | 0xFF);
}

ByteColor ByteColor::FromHexRGBA(unsigned int HexRGBA)
{
    const ComponentType R = (HexRGBA >> 24) & 0xFF;
    const ComponentType G = (HexRGBA >> 16) & 0xFF;
    const ComponentType B = (HexRGBA >> 8)  & 0xFF;
    const ComponentType A = (HexRGBA >> 0)  & 0xFF;

    return ByteColor { R, G, B, A };
}

// =====================================================================================================================
// ==================================================== FloatColor =====================================================
// =====================================================================================================================
FloatColor::FloatColor(ComponentType R, ComponentType G, ComponentType B, ComponentType A)
    : R(R), G(G), B(B), A(A) {};
FloatColor::FloatColor(ComponentType R, ComponentType G, ComponentType B)
    : R(R), G(G), B(B), A(1) {}

FloatColor FloatColor::FromHexRGB(unsigned int HexRGB)
{
    return FromHexRGBA((HexRGB << 8) | 0xFF);
}
FloatColor FloatColor::FromHexRGBA(unsigned int HexRGBA)
{
    return FloatColor { ByteColor::FromHexRGBA(HexRGBA) };
};

FloatColor::operator ByteColor() const
{
    constexpr auto ConversionFactor = std::numeric_limits<ByteColor::ComponentType>::max();
    return ByteColor {
        ColorSpaceConversions::LinearFloatTosRGBByte(R),
        ColorSpaceConversions::LinearFloatTosRGBByte(G),
        ColorSpaceConversions::LinearFloatTosRGBByte(B),
        ColorSpaceConversions::LinearFloatTosRGBByte(A),
    };
}

ByteColor FloatColor::ToByteColor() const
{
    return ByteColor { *this };
}

}
