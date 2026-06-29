#include "NavigationNode.h"

using namespace NCL;
using namespace NCL::CSC8503;

NavigationNode::NavigationNode(const Vector3& pos) {

    position = pos;

    gCost = 0.0f;
    hCost = 0.0f;
    fCost = 0.0f;

    parent = nullptr;
}