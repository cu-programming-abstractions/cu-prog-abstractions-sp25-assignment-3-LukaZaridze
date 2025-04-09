#include "MountainsOfRecursia.h"
#include "random.h"
#include "error.h"
using namespace std;

Vector<Point> makeMountainRange(const Point& left,
                                const Point& right,
                                int amplitude,
                                double decayRate) {
    if (left.x > right.x) error("Left point is to the right of the right point.");
    if (amplitude < 0) error("Amplitude cannot be negative.");
    if (decayRate < 0 || decayRate > 1) error("Decay rate must be in [0, 1].");

    if (right.x - left.x <= 3) {
        return {left, right};
    }

    int midX = (left.x + right.x) / 2;
    int midY = (left.y + right.y) / 2 + randomInteger(-amplitude, amplitude);
    Point mid = {midX, midY};

    auto leftHalf = makeMountainRange(left, mid, int(amplitude * decayRate), decayRate);
    auto rightHalf = makeMountainRange(mid, right, int(amplitude * decayRate), decayRate);

    leftHalf.remove(leftHalf.size() - 1); // removes the midpoint
    leftHalf += rightHalf;

    return leftHalf;
}



/* * * * * Test Cases Below This Point * * * * */
#include "GUI/SimpleTest.h"
#include "Demos/ChiSquaredTesting.h"

PROVIDED_TEST("Handles invalid inputs.") {
    EXPECT_ERROR(makeMountainRange({0, 0}, {-1, 0}, 10, 1));
    EXPECT_ERROR(makeMountainRange({0, 0}, {10, 10}, -137, 1));
    EXPECT_ERROR(makeMountainRange({0, 0}, {10, 10}, 137, -0.1));
    EXPECT_ERROR(makeMountainRange({0, 0}, {10, 10}, 137, +1.1));
}

PROVIDED_TEST("Works on points that are very close together.") {
    auto mountain = makeMountainRange({0, 0}, {1, 0}, 100, 0.1);
    EXPECT_EQUAL(mountain, {{ 0, 0 }, { 1, 0 }});
}

PROVIDED_TEST("Works with close points and amplitude zero.") {
    auto mountain = makeMountainRange({0, 0}, {6, 6}, 0, 1);
    EXPECT_EQUAL(mountain, {{ 0, 0 }, { 3, 3 }, { 6, 6 }});
}

PROVIDED_TEST("Works with far points and zero amplitude.") {
    const int kNumPoints = 33;
    Vector<Point> points;
    for (int i = 0; i < kNumPoints; i++) {
        points += Point{ 3 * i, 3 * i };
    }
    EXPECT_EQUAL(makeMountainRange(points[0], points[points.size() - 1], 0, 1), points);
}

PROVIDED_TEST("Works with close points and amplitude 1.") {
    if (!ChiSquaredTesting::isClose({ 1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0 }, [] {
            auto mountain = makeMountainRange({10, 0}, {16, 0}, 1, 1);
            EXPECT_EQUAL(mountain.size(), 3);
            EXPECT_EQUAL(mountain[0], {10, 0});
            EXPECT_EQUAL(mountain[2], {16, 0});
            EXPECT_EQUAL(mountain[1].x, 13);
            EXPECT_GREATER_THAN_OR_EQUAL_TO(mountain[1].y, -1);
            EXPECT_LESS_THAN_OR_EQUAL_TO(mountain[1].y, 1);
            return mountain[1].y + 1;
        })) {
        SHOW_ERROR("Not all midpoint displacements are equally likely.");
    }
}

PROVIDED_TEST("Works with close points and amplitude 10.") {
    if (!ChiSquaredTesting::isClose(vector<double>(21, 1.0 / 21), [] {
            auto mountain = makeMountainRange({10, 0}, {16, 0}, 10, 1);
            EXPECT_EQUAL(mountain.size(), 3);
            EXPECT_EQUAL(mountain[0], {10, 0});
            EXPECT_EQUAL(mountain[2], {16, 0});
            EXPECT_EQUAL(mountain[1].x, 13);
            EXPECT_GREATER_THAN_OR_EQUAL_TO(mountain[1].y, -10);
            EXPECT_LESS_THAN_OR_EQUAL_TO(mountain[1].y, 10);
            return mountain[1].y + 10;
        })) {
        SHOW_ERROR("Not all midpoint displacements are equally likely.");
    }
}

PROVIDED_TEST("Works on faraway points with decaying amplitude.") {
    if (!ChiSquaredTesting::isClose(vector<double>(5, 1.0 / 5), [] {
            auto mountain = makeMountainRange({10, 0}, {22, 12}, 2, 0.5);
            EXPECT_EQUAL(mountain.size(), 5);
            EXPECT_EQUAL(mountain[0], {10, 0});
            EXPECT_EQUAL(mountain[4], {22, 12});
            EXPECT_EQUAL(mountain[1].x, 13);
            EXPECT_EQUAL(mountain[2].x, 16);
            EXPECT_EQUAL(mountain[3].x, 19);
            EXPECT_GREATER_THAN_OR_EQUAL_TO(mountain[2].y, 4);
            EXPECT_LESS_THAN_OR_EQUAL_TO(mountain[2].y, 8);
            return mountain[2].y - 4;
        })) {
        SHOW_ERROR("Not all central midpoint displacements are equally likely.");
    }

    if (!ChiSquaredTesting::isClose({ 2.0 / 15, 4.0 / 15, 5.0 / 15, 3.0 / 15, 1.0 / 15 }, [] {
            auto mountain = makeMountainRange({10, 0}, {22, 12}, 2, 0.5);
            EXPECT_EQUAL(mountain.size(), 5);
            EXPECT_EQUAL(mountain[0], {10, 0});
            EXPECT_EQUAL(mountain[4], {22, 12});
            EXPECT_EQUAL(mountain[1].x, 13);
            EXPECT_EQUAL(mountain[2].x, 16);
            EXPECT_EQUAL(mountain[3].x, 19);
            EXPECT_GREATER_THAN_OR_EQUAL_TO(mountain[1].y, 1);
            EXPECT_LESS_THAN_OR_EQUAL_TO(mountain[1].y, 5);
            return mountain[1].y - 1;
        })) {
        SHOW_ERROR("Not all left midpoint displacements are equally likely.");
    }

    if (!ChiSquaredTesting::isClose({ 2.0 / 15, 4.0 / 15, 5.0 / 15, 3.0 / 15, 1.0 / 15 }, [] {
            auto mountain = makeMountainRange({10, 0}, {22, 12}, 2, 0.5);
            EXPECT_EQUAL(mountain.size(), 5);
            EXPECT_EQUAL(mountain[0], {10, 0});
            EXPECT_EQUAL(mountain[4], {22, 12});
            EXPECT_EQUAL(mountain[1].x, 13);
            EXPECT_EQUAL(mountain[2].x, 16);
            EXPECT_EQUAL(mountain[3].x, 19);
            EXPECT_GREATER_THAN_OR_EQUAL_TO(mountain[3].y, 7);
            EXPECT_LESS_THAN_OR_EQUAL_TO(mountain[3].y, 11);
            return mountain[3].y - 7;
        })) {
        SHOW_ERROR("Not all right midpoint displacements are equally likely.");
    }
}
