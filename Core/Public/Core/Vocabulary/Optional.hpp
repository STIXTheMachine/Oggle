#pragma once

namespace Oggle
{
    enum class EOptionalState
    {
        Empty,
        Valid
    };

    template<typename Value>
    struct Optional
    {
        Optional();

    private:
        Value InternalValue;
        EOptionalState State { EOptionalState::Empty };
    };

    template <typename Value>
    Optional<Value>::Optional()
    {

    }
}
