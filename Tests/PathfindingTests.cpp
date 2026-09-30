#include <gtest/gtest.h>

#include "Pathfinding.h"
#include "NavigationNode.h"

using namespace NCL;
using namespace NCL::CSC8503;
using namespace NCL::Maths;

// TC-NAV-001
// Verify that a valid connection between two nodes produces
// the expected path from start to goal.
TEST(PathfindingTests, FindsDirectPathBetweenConnectedNodes) {

    NavigationNode start(Vector3(0.0f, 0.0f, 0.0f));
    NavigationNode goal(Vector3(10.0f, 0.0f, 0.0f));

    start.neighbours.push_back(&goal);

    std::vector<NavigationNode*> path =
        Pathfinding::FindPath(&start, &goal);

    ASSERT_EQ(path.size(), 2);

    EXPECT_EQ(path[0], &start);
    EXPECT_EQ(path[1], &goal);
}

// TC-NAV-002
// Verify that the pathfinder returns an empty path when
// no route exists between start and goal.
TEST(PathfindingTests, ReturnsEmptyPathWhenGoalIsUnreachable) {

    NavigationNode start(Vector3(0.0f, 0.0f, 0.0f));
    NavigationNode isolated(Vector3(5.0f, 0.0f, 0.0f));
    NavigationNode goal(Vector3(10.0f, 0.0f, 0.0f));

    start.neighbours.push_back(&isolated);

    std::vector<NavigationNode*> path =
        Pathfinding::FindPath(&start, &goal);

    EXPECT_TRUE(path.empty());
}

// TC-NAV-003
// Boundary case: start and goal refer to the same node.
// The expected route contains that node only.
TEST(PathfindingTests, ReturnsSingleNodeWhenStartEqualsGoal) {

    NavigationNode startAndGoal(Vector3(0.0f, 0.0f, 0.0f));

    std::vector<NavigationNode*> path =
        Pathfinding::FindPath(&startAndGoal, &startAndGoal);

    ASSERT_EQ(path.size(), 1);

    EXPECT_EQ(path[0], &startAndGoal);
}

// TC-NAV-004
// Regression / state-isolation test.
//
// Pathfinding stores search state directly inside NavigationNode.
// Execute one search and then perform another search over shared
// nodes to verify that the previous execution does not corrupt
// the subsequent path result.
TEST(PathfindingTests, RepeatedExecutionProducesIndependentValidPath) {

    NavigationNode a(Vector3(0.0f, 0.0f, 0.0f));
    NavigationNode b(Vector3(5.0f, 0.0f, 0.0f));
    NavigationNode c(Vector3(10.0f, 0.0f, 0.0f));

    a.neighbours.push_back(&b);
    b.neighbours.push_back(&c);

    std::vector<NavigationNode*> firstPath =
        Pathfinding::FindPath(&a, &c);

    ASSERT_EQ(firstPath.size(), 3);
    EXPECT_EQ(firstPath[0], &a);
    EXPECT_EQ(firstPath[1], &b);
    EXPECT_EQ(firstPath[2], &c);

    std::vector<NavigationNode*> secondPath =
        Pathfinding::FindPath(&b, &c);

    ASSERT_EQ(secondPath.size(), 2);
    EXPECT_EQ(secondPath[0], &b);
    EXPECT_EQ(secondPath[1], &c);
}