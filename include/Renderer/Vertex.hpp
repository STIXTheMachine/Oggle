#pragma once
#include <Core/MathTypes.hpp>
#include <Core/Color.hpp>

struct Vertex_P   { Vec3 Position; };
struct Vertex_PN  { Vec3 Position; Vec3 Normal; };
struct Vertex_PC  { Vec3 Position; FloatColor Color; };
struct Vertex_PNC { Vec3 Position; Vec3 Normal; FloatColor Color; };