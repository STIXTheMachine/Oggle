#include "Core/Utilities/Assert.hpp"

void Fronch()
{
    OGGLE_ENSURE_MSG(false, "Raggle Fraggle");
}

void Grimble()
{
    Fronch();
}

void Flargus()
{
    Grimble();
}

int main()
{
    Flargus();
}