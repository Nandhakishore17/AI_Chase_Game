#include "Pathfinding.h"

#include <algorithm>
#include <limits>

using namespace NCL;
using namespace NCL::CSC8503;
using namespace NCL::Maths;

float Pathfinding::Heuristic(
    NavigationNode* a,
    NavigationNode* b
) {
    return Vector::Length(a->position - b->position);
}

std::vector<NavigationNode*> Pathfinding::FindPath(
    NavigationNode* start,
    NavigationNode* goal
) {

    std::vector<NavigationNode*> openList;
    std::vector<NavigationNode*> closedList;

    openList.push_back(start);

    start->gCost = 0;
    start->hCost = Heuristic(start, goal);
    start->fCost = start->hCost;
    start->parent = nullptr;

    while (!openList.empty()) {

        NavigationNode* current = openList[0];

        for (auto node : openList) {
            if (node->fCost < current->fCost)
                current = node;
        }

        if (current == goal) {

            std::vector<NavigationNode*> path;

            while (current != nullptr) {
                path.push_back(current);
                current = current->parent;
            }

            std::reverse(path.begin(), path.end());

            return path;
        }

        openList.erase(
            std::remove(openList.begin(), openList.end(), current),
            openList.end()
        );

        closedList.push_back(current);

        for (auto neighbour : current->neighbours) {

            bool inClosed = std::find(
                closedList.begin(),
                closedList.end(),
                neighbour
            ) != closedList.end();

            if (inClosed)
                continue;

            float newCost =
                current->gCost +
                Vector::Length(
                    neighbour->position -
                    current->position
                );

            bool inOpen = std::find(
                openList.begin(),
                openList.end(),
                neighbour
            ) != openList.end();

            if (!inOpen || newCost < neighbour->gCost) {

                neighbour->gCost = newCost;

                neighbour->hCost =
                    Heuristic(neighbour, goal);

                neighbour->fCost =
                    neighbour->gCost +
                    neighbour->hCost;

                neighbour->parent = current;

                if (!inOpen)
                    openList.push_back(neighbour);
            }
        }
    }

    return {};
}