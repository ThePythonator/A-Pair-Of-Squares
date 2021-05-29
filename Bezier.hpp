#pragma once

#include <utility>
#include <array>
#include <cstdint>

typedef std::pair<float, float> Node;

template<uint8_t N>
Node bezier_curve(std::array<Node, N> nodes, float ratio);