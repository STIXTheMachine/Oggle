#pragma once
#include "CoreMinimal.hpp"

namespace Oggle
{

struct Renderer;

struct RendererCreateInfo
{

};

struct RendererCreateError
{

};

using RendererCreateResult = Result<Unique<Renderer>, RendererCreateError>;

struct Renderer
{
    static RendererCreateResult Create(RendererCreateInfo);

private:
    Renderer();
};
}
