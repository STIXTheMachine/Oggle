//
// Created by jsoar on 8/30/2026.
//

#pragma once

namespace Oggle
{
class ModuleInterface
{
public:
    virtual ~ModuleInterface() = default;

private:
    friend struct Application;
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
};
}
