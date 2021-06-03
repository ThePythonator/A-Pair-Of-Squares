#include "Bezier.hpp"

//template<uint8_t N>
//Node bezier_curve(std::array<Node, N> nodes, float ratio) {
//    if (nodes.size() == 0) {
//        return Node{ 0, 0 };
//    }
//    else if (nodes.size() == 1) {
//        return nodes[0];
//    }
//
//    std::array<Node, N - 1> new_nodes;
//
//    for (uint8_t i = 0; i < nodes.size() - 1; i++) {
//        Node new_node{ 0, 0 };
//        new_node.first = nodes[i].first + (nodes[i + 1].first - nodes[i].first) * ratio;
//        new_node.second = nodes[i].second + (nodes[i + 1].second - nodes[i].second) * ratio;
//        new_nodes[i] = new_node;
//    }
//
//    return bezier_curve<N - 1>(new_nodes, ratio);
//}

Node bezier_curve(std::vector<Node> nodes, float ratio) {
    if (nodes.size() == 0) {
        return Node{ 0, 0 };
    }
    else if (nodes.size() == 1) {
        return nodes[0];
    }

    std::vector<Node> new_nodes;

    for (uint8_t i = 0; i < nodes.size() - 1; i++) {
        Node new_node{ 0, 0 };
        new_node.first = nodes[i].first + (nodes[i + 1].first - nodes[i].first) * ratio;
        new_node.second = nodes[i].second + (nodes[i + 1].second - nodes[i].second) * ratio;
        new_nodes.push_back(new_node);
    }

    return bezier_curve(new_nodes, ratio);
}