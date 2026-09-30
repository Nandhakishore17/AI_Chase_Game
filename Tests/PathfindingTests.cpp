#include <gtest/gtest.h>

#include "Pathfinding.h"
#include "NavigationNode.h"

using namespace NCL;
using namespace NCL::CSC8503;
using namespace NCL::Maths;

// AT-NAV-001
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

// AT-NAV-002
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

// AT-NAV-003
// Boundary case: start and goal refer to the same node.
// The expected route contains that node only.
TEST(PathfindingTests, ReturnsSingleNodeWhenStartEqualsGoal) {

    NavigationNode startAndGoal(Vector3(0.0f, 0.0f, 0.0f));

    std::vector<NavigationNode*> path =
        Pathfinding::FindPath(&startAndGoal, &startAndGoal);

    ASSERT_EQ(path.size(), 1);

    EXPECT_EQ(path[0], &startAndGoal);
}

// AT-NAV-004
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

// AT-NAV-005
// Verify that when multiple valid routes exist, the pathfinder
// selects the route with the lower geometric travel cost.
TEST(PathfindingTests, SelectsLowerCostRouteWhenMultiplePathsExist) {

    NavigationNode start(Vector3(0.0f, 0.0f, 0.0f));
    NavigationNode upper(Vector3(5.0f, 0.0f, 8.0f));
    NavigationNode lower(Vector3(5.0f, 0.0f, 1.0f));
    NavigationNode goal(Vector3(10.0f, 0.0f, 0.0f));

    // More expensive route:
    // start -> upper -> goal
    start.neighbours.push_back(&upper);
    upper.neighbours.push_back(&goal);

    // Cheaper route:
    // start -> lower -> goal
    start.neighbours.push_back(&lower);
    lower.neighbours.push_back(&goal);

    std::vector<NavigationNode*> path =
        Pathfinding::FindPath(&start, &goal);

    ASSERT_EQ(path.size(), 3);

    EXPECT_EQ(path[0], &start);
    EXPECT_EQ(path[1], &lower);
    EXPECT_EQ(path[2], &goal);
}

// AT-NAV-006
// Stronger state-isolation regression test.
//
// The first search establishes parent/cost state across shared nodes.
// The second search starts from a different node and requires a
// different optimal route through nodes already touched by search 1.
//
// This verifies that stale pathfinding state from a previous execution
// does not incorrectly determine the subsequent result.
TEST(PathfindingTests, RepeatedSearchRecalculatesOptimalRouteAcrossSharedNodes) {

    NavigationNode a(Vector3(0.0f, 0.0f, 0.0f));
    NavigationNode b(Vector3(4.0f, 0.0f, 0.0f));
    NavigationNode c(Vector3(8.0f, 0.0f, 0.0f));
    NavigationNode d(Vector3(12.0f, 0.0f, 0.0f));
    NavigationNode alternate(Vector3(8.0f, 0.0f, 6.0f));

    // First search:
    // A -> B -> C -> D
    a.neighbours.push_back(&b);
    b.neighbours.push_back(&c);
    c.neighbours.push_back(&d);

    std::vector<NavigationNode*> firstPath =
        Pathfinding::FindPath(&a, &d);

    ASSERT_EQ(firstPath.size(), 4);
    EXPECT_EQ(firstPath[0], &a);
    EXPECT_EQ(firstPath[1], &b);
    EXPECT_EQ(firstPath[2], &c);
    EXPECT_EQ(firstPath[3], &d);

    // Introduce another valid route after the first search.
    //
    // Second search from B should still select:
    // B -> C -> D
    //
    // rather than:
    // B -> alternate -> D
    b.neighbours.push_back(&alternate);
    alternate.neighbours.push_back(&d);

    std::vector<NavigationNode*> secondPath =
        Pathfinding::FindPath(&b, &d);

    ASSERT_EQ(secondPath.size(), 3);

    EXPECT_EQ(secondPath[0], &b);
    EXPECT_EQ(secondPath[1], &c);
    EXPECT_EQ(secondPath[2], &d);
}