#pragma once

#include <utility>
//#include <array>
#include <cstdint>

#include <vector>

typedef std::pair<float, float> Node;

//template<uint8_t N>
//Node bezier_curve(std::array<Node, N> nodes, float ratio);

Node bezier_curve(std::vector<Node> nodes, float ratio);