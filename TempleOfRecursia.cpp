#include "TempleOfRecursia.h"
#include "vector.h"
#include "error.h"
using namespace std;

Vector<Rectangle> makeTemple(const Rectangle& bounds, const TempleParameters& params) {
    Vector<Rectangle> temple;

    if (params.order < 0) error("Order cannot be negative");
    if (params.order == 0) return temple;

    int boundX = bounds.x;
    int boundY = bounds.y;
    int boundWidth = bounds.width;
    int boundHeight = bounds.height;

    // Create base (centered horizontally, flush with bottom)
    int baseWidth = static_cast<int>(boundWidth * params.baseWidth);
    int baseHeight = static_cast<int>(boundHeight * params.baseHeight);
    int baseX = boundX + (boundWidth - baseWidth) / 2;
    int baseY = boundY + boundHeight - baseHeight;  // Base at the bottom
    temple.add(Rectangle{baseX, baseY, baseWidth, baseHeight});

    // Create column (centered horizontally, on top of base)
    int columnWidth = static_cast<int>(boundWidth * params.columnWidth);
    int columnHeight = static_cast<int>(boundHeight * params.columnHeight);
    int columnX = boundX + (boundWidth - columnWidth) / 2;
    int columnY = baseY - columnHeight;  // Column directly above the base
    temple.add(Rectangle{columnX, columnY, columnWidth, columnHeight});

    // Recursively create upper temple
    if (params.order > 1) {
        int upperHeight = static_cast<int>(boundHeight * params.upperTempleHeight);
        Rectangle upperBounds{columnX, columnY - upperHeight, columnWidth, upperHeight};

        TempleParameters upperParams = params;
        upperParams.order = params.order - 1;

        Vector<Rectangle> upperTemple = makeTemple(upperBounds, upperParams);
        for (const Rectangle& rect : upperTemple) {
            temple.add(rect);
        }
    }

    // Create smaller temples (evenly spaced along base)
    if (params.order > 1) {
        int smallWidth = static_cast<int>(boundWidth * params.smallTempleWidth);
        int smallHeight = static_cast<int>(boundHeight * params.smallTempleHeight);

        int totalWidth = params.numSmallTemples * smallWidth;
        int availableSpace = baseWidth - totalWidth;
        int spacing = availableSpace / (params.numSmallTemples + 1);

        for (int i = 0; i < params.numSmallTemples; i++) {
            int templeX = baseX + spacing + i * (smallWidth + spacing);
            int templeY = baseY - smallHeight;  // Ensure alignment
            Rectangle smallBounds{templeX, templeY, smallWidth, smallHeight};

            TempleParameters smallParams = params;
            smallParams.order = params.order - 1;

            Vector<Rectangle> smallTemple = makeTemple(smallBounds, smallParams);
            for (const Rectangle& rect : smallTemple) {
                temple.add(rect);
            }
        }
    }

    return temple;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * */
/*                TEST CASES                        */
/* * * * * * * * * * * * * * * * * * * * * * * * * */

#include "GUI/SimpleTest.h"

PROVIDED_TEST("Basic temple construction") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 1;

    auto temple = makeTemple(bounds, params);
    EXPECT_EQUAL(temple.size(), 2);
}

PROVIDED_TEST("Multiple levels of recursion") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 3;

    auto temple = makeTemple(bounds, params);
    EXPECT(temple.size() > 6);
}

PROVIDED_TEST("Small temples creation") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 2;
    params.numSmallTemples = 4;

    auto temple = makeTemple(bounds, params);
    EXPECT(temple.size() > 10);
}

PROVIDED_TEST("Zero order returns empty") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 0;

    auto temple = makeTemple(bounds, params);
    EXPECT_EQUAL(temple.size(), 0);
}

PROVIDED_TEST("Negative order throws error") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = -1;

    EXPECT_ERROR(makeTemple(bounds, params));
}

PROVIDED_TEST("Precise positioning check") {
    Rectangle bounds = {100, 200, 400, 800};
    TempleParameters params;
    params.order = 2;
    params.baseWidth = 0.8;
    params.baseHeight = 0.2;
    params.columnWidth = 0.4;
    params.columnHeight = 0.3;
    params.upperTempleHeight = 0.5;
    params.numSmallTemples = 3;
    params.smallTempleWidth = 0.15;
    params.smallTempleHeight = 0.25;

    auto temple = makeTemple(bounds, params);

    EXPECT_EQUAL(temple[0], (Rectangle{140, 840, 320, 160}));
    EXPECT_EQUAL(temple[1], (Rectangle{220, 600, 160, 240}));
    EXPECT_EQUAL(temple[2].x, 236);
    EXPECT_EQUAL(temple[2].width, 128);
}

PROVIDED_TEST("Small temple spacing") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 2;
    params.baseWidth = 1.0;
    params.numSmallTemples = 4;
    params.smallTempleWidth = 0.1;

    auto temple = makeTemple(bounds, params);
    EXPECT_EQUAL(temple[4].x, 120);
    EXPECT_EQUAL(temple[6].x, 340);
}

PROVIDED_TEST("Deep recursion test") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 5;

    auto temple = makeTemple(bounds, params);
    EXPECT(temple.size() > 20);
}

PROVIDED_TEST("Edge case proportions") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 1;
    params.baseWidth = 0.01;
    params.baseHeight = 0.01;
    params.columnWidth = 0.01;
    params.columnHeight = 0.01;

    auto temple = makeTemple(bounds, params);
    EXPECT_EQUAL(temple[0].width, 10);
    EXPECT_EQUAL(temple[1].width, 10);
}

PROVIDED_TEST("Maximum proportions") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 1;
    params.baseWidth = 1.0;
    params.baseHeight = 1.0;
    params.columnWidth = 1.0;
    params.columnHeight = 1.0;

    auto temple = makeTemple(bounds, params);
    EXPECT_EQUAL(temple[0], bounds);
    EXPECT_EQUAL(temple[1], { 0, -1000, 1000, 1000 });
}

PROVIDED_TEST("Small temple recursion") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 3;
    params.numSmallTemples = 2;

    auto temple = makeTemple(bounds, params);
    EXPECT(temple.size() >= 8);
}

PROVIDED_TEST("Complex temple structure") {
    Rectangle bounds = {500, 500, 2000, 2000};
    TempleParameters params;
    params.order = 4;
    params.baseWidth = 0.8;
    params.baseHeight = 0.15;
    params.columnWidth = 0.5;
    params.columnHeight = 0.2;
    params.upperTempleHeight = 0.6;
    params.numSmallTemples = 3;
    params.smallTempleWidth = 0.15;
    params.smallTempleHeight = 0.3;

    auto temple = makeTemple(bounds, params);
    EXPECT(temple.size() > 20);
    EXPECT_EQUAL(temple[0].width, 1600);
    EXPECT_EQUAL(temple[1].width, 1000);
}

PROVIDED_TEST("Temple component ordering") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 2;

    auto temple = makeTemple(bounds, params);
    EXPECT_EQUAL(temple[0].height + temple[0].y, 1000);
    EXPECT_EQUAL(temple[1].y + temple[1].height, temple[0].y);
    EXPECT_EQUAL(temple[2].y + temple[2].height, temple[1].y);
}

PROVIDED_TEST("Small temple positioning") {
    Rectangle bounds = {0, 0, 1000, 1000};
    TempleParameters params;
    params.order = 2;
    params.numSmallTemples = 2;

    auto temple = makeTemple(bounds, params);
    EXPECT(temple.size() >= 6);
    EXPECT_EQUAL(temple[4].y, 850);
    EXPECT_EQUAL(temple[5].y, 700);
}

PROVIDED_TEST("Extreme recursion depth") {
    Rectangle bounds = {0, 0, 10000, 10000};
    TempleParameters params;
    params.order = 6;

    auto temple = makeTemple(bounds, params);
    EXPECT(temple.size() > 30);
}

PROVIDED_TEST("Non-square bounds") {
    Rectangle bounds = {0, 0, 500, 2000};
    TempleParameters params;
    params.order = 2;

    auto temple = makeTemple(bounds, params);
    EXPECT_EQUAL(temple[0].width, static_cast<int>(500 * params.baseWidth));
    EXPECT_EQUAL(temple[0].height, static_cast<int>(2000 * params.baseHeight));
}

PROVIDED_TEST("Off-center bounds") {
    Rectangle bounds = {300, 700, 800, 600};
    TempleParameters params;
    params.order = 1;

    auto temple = makeTemple(bounds, params);
    EXPECT_EQUAL(temple[0].x, 300 + (800 - static_cast<int>(800 * params.baseWidth)) / 2);
    EXPECT_EQUAL(temple[0].y, 700 + 600 - static_cast<int>(600 * params.baseHeight));
}
