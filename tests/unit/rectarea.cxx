
#include "../suite/recorder/util.h"
#include "rfb/Rect.h"
#include <cstdio>


namespace rectUnionArea {
  struct TestEntry {
    TestEntry(const char* testName, std::vector<rfb::Rect> intersections,
              int expectedArea) : testName(testName),
                                  intersections(intersections),
                                  expectedArea(expectedArea)
    {
    }

    const char* testName;
    std::vector<rfb::Rect> intersections;
    int expectedArea;
  };

  TestEntry testOne()
  {
    const char* testName = "Union Test One";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(50,50,100,100);

    intersections.push_back(r1);
    intersections.push_back(r2);

    return TestEntry(
      testName,
      intersections,
      r1.area() + r2.area()
    );
  }

  TestEntry testTwo()
  {
    const char* testName = "Union Test Two";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(45,45,100,100);

    intersections.push_back(r1);
    intersections.push_back(r2);

    return TestEntry(
      testName,
      intersections,
      r1.area() + r2.area() - 5*5
    );
  }

  TestEntry testThree()
  {
    const char* testName = "Union Test Three";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(50,50,100,100);
    rfb::Rect r3(100,100,150,150);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);

    return TestEntry(
      testName,
      intersections,
      r1.area() + r2.area() + r3.area()
    );
  }

  TestEntry testFour()
  {
    const char* testName = "Union Test Four";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(45,45,105,105);
    rfb::Rect r3(100,100,150,150);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);

    return TestEntry(
      testName,
      intersections, 
      r1.area() + r2.area() + r3.area() - 5*5 - 5*5);
  }

  TestEntry testFive()
  {
    const char* testName = "Union Test Five";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(45,45,105,105);
    rfb::Rect r3(40,40,150,150);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);

    return TestEntry(
      testName,
      intersections,
      r1.area() + r2.area() + r3.area() - 10*10 - 60*60);
  }

  TestEntry testSix()
  {
    const char* testName = "Union Test Six";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(45,45,105,105);
    rfb::Rect r3(40,40,150,150);
    rfb::Rect r4(0,0,200,200);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);
    intersections.push_back(r4);

    return TestEntry(testName, intersections, r4.area());
  }

  TestEntry testSeven()
  {
    const char* testName = "Union Test Seven";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(45,45,105,105);
    rfb::Rect r3(40,40,150,150);
    rfb::Rect r4(0,0,200,200);
    rfb::Rect r5(150,150,250,250);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);
    intersections.push_back(r4);
    intersections.push_back(r5);

    return TestEntry(testName, intersections, r4.area() + r5.area() - 50*50);
  }

  TestEntry testEight()
  {
    const char* testName = "Union Test Eight";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,0,0);
    rfb::Rect r2(1,1,1,1);
    rfb::Rect r3(0,0,1,1);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);

    return TestEntry(testName, intersections, r3.area());
  }

  TestEntry testNine()
  {
    const char* testName = "Union Test Nine";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(20,30,110,90);
    rfb::Rect r3(40,40,150,150);
    rfb::Rect r4(75,10,170,170);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);
    intersections.push_back(r4);

    return TestEntry(testName, intersections, (50*30+20*20) + (20*60+35*10)
                                            + 110*110
                                            + (95*30+20*130+75*20));
  }

  TestEntry testTen()
  {
    const char* testName = "Intersect Stats Test Ten";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(70,60,160,120);

    intersections.push_back(r1);
    intersections.push_back(r2);

    return TestEntry(testName, intersections,
                    r1.area() + r2.area());
  }
}

namespace rectIntersectionStats {
  struct TestEntry {
    TestEntry(const char* testName, std::vector<rfb::Rect> intersections,
              int expectedLostDataArea, int expectedOverDimensionedArea)
              : testName(testName), intersections(intersections),
                expectedLostDataArea(expectedLostDataArea),
                expectedOverdimensionedArea(expectedOverDimensionedArea)
    {
    }

    const char* testName;
    std::vector<rfb::Rect> intersections;
    int expectedLostDataArea;
    int expectedOverdimensionedArea;
  };

  TestEntry testOne()
  {
    const char* testName = "Intersect Stats Test One";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(50,50,100,100);
    intersections.push_back(r1);
    intersections.push_back(r2);

    return TestEntry(testName,
                     intersections,
                     0,
                     50*50*2);
  }

  TestEntry testTwo()
  {
    const char* testName = "Intersect Stats Test Two";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(45,45,100,100);

    intersections.push_back(r1);
    intersections.push_back(r2);

    return TestEntry(
      testName,
      intersections,
      5*5,
      45*50 + 50*45
    );
  }

  TestEntry testThree()
  {
    const char* testName = "Intersect Stats Test Three";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(50,50,100,100);
    rfb::Rect r3(100,100,150,150);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);

    return TestEntry(
      testName,
      intersections,
      0,
      (100*50 + 50*50) + (50*100 + 50*50)
    );
  }

  TestEntry testFour()
  {
    const char* testName = "Intersect Stats Test Four";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(45,45,105,105);
    rfb::Rect r3(100,100,150,150);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);

    return TestEntry(
      testName,
      intersections, 
      5*5 + 5*5,
      (100*45 + 45*55) + (100*45 + 45*55));
  }

  TestEntry testFive()
  {
    const char* testName = "Intersect Stats Test Five";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(45,45,105,105);
    rfb::Rect r3(40,40,150,150);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);

    return TestEntry(
      testName,
      intersections,
      60*60 + 10*10 + 5*5, (40*100) + 100*40);
  }

  TestEntry testSix()
  {
    const char* testName = "Intersect Stats Test Six";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(45,45,105,105);
    rfb::Rect r3(40,40,150,150);
    rfb::Rect r4(0,0,200,200);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);
    intersections.push_back(r4);

    return TestEntry(testName,
                     intersections,
                     (10*10 + 5*5 + 50*50) + (110*110 + 60*60) + (60*60),
                     0);
  }

  TestEntry testSeven()
  {
    const char* testName = "Intersect Stats Test Seven";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(45,45,105,105);
    rfb::Rect r3(40,40,150,150);
    rfb::Rect r4(0,0,200,200);
    rfb::Rect r5(150,150,250,250);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);
    intersections.push_back(r4);
    intersections.push_back(r5);

    return TestEntry(testName,
                     intersections,
                     (10*10 + 5*5 + 50*50) 
                   + (60*60 + 60*60) + (110*110) + (50*50),
                     150*50 + 50*150);
  }

  TestEntry testEight()
  {
    const char* testName = "Intersect Stats Test Eight";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,0,0);
    rfb::Rect r2(1,1,1,1);
    rfb::Rect r3(0,0,1,1);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);

    return TestEntry(testName, intersections, 0, 0);
  }

  TestEntry testNine()
  {
    const char* testName = "Intersect Stats Test Nine";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(20,30,110,90);
    rfb::Rect r3(40,40,150,150);
    rfb::Rect r4(75,10,170,170);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);
    intersections.push_back(r4);

    return TestEntry(testName, intersections,
                    (30*20 + 10*10)
                  + (70*50 + 35*60)
                  + (75*110),
                    20*40 + 40*80 + 35*20 + 25*30 + 95*10);
  }

  TestEntry testTen()
  {
    const char* testName = "Intersect Stats Test Ten";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,50,50);
    rfb::Rect r2(70,60,160,120);

    intersections.push_back(r1);
    intersections.push_back(r2);

    return TestEntry(testName, intersections,
                    0,
                    110*50 + 90*10 + 70*70);
  }
}


void testTotalArea(rectUnionArea::TestEntry entry)
{
  printf("Running %s...\n", entry.testName);
  int area = rectTotalArea(entry.intersections);
  if (area != entry.expectedArea) {
    printf("TEST %s FAILED. Got: %d, expected: %d\n ", entry.testName,
                                                       area,
                                                       entry.expectedArea);
    return;
  }
  printf("OK!\n");
}

void testIntersectionStats(rectIntersectionStats::TestEntry entry)
{
  bool failed = false;
  printf("Running %s...\n", entry.testName);
  ImageUpdateStats stats = detectInteresctions(entry.intersections);
  if (stats.overDimensionedArea != entry.expectedOverdimensionedArea) {
    printf("TEST %s FAILED. Got overdimensioned area: %d, expected: %d\n ",
            entry.testName, stats.overDimensionedArea,
            entry.expectedOverdimensionedArea);
    failed = true;
  }
  if (stats.lostDataArea != entry.expectedLostDataArea) {
    printf("TEST %s FAILED. Got lost data area: %d, expected: %d\n ",
            entry.testName, stats.lostDataArea,
            entry.expectedLostDataArea);
    failed = true;
  }
  if (!failed)
    printf("OK!\n");
}


int main()
{
  testTotalArea(rectUnionArea::testOne());
  testTotalArea(rectUnionArea::testTwo());
  testTotalArea(rectUnionArea::testThree());
  testTotalArea(rectUnionArea::testFour());
  testTotalArea(rectUnionArea::testFive());
  testTotalArea(rectUnionArea::testSix());
  testTotalArea(rectUnionArea::testSeven());
  testTotalArea(rectUnionArea::testEight());
  testTotalArea(rectUnionArea::testNine());
  testTotalArea(rectUnionArea::testTen());
  testIntersectionStats(rectIntersectionStats::testOne());
  testIntersectionStats(rectIntersectionStats::testTwo());
  testIntersectionStats(rectIntersectionStats::testThree());
  testIntersectionStats(rectIntersectionStats::testFour());
  testIntersectionStats(rectIntersectionStats::testFive());
  testIntersectionStats(rectIntersectionStats::testSix());
  testIntersectionStats(rectIntersectionStats::testSeven());
  testIntersectionStats(rectIntersectionStats::testEight());
  testIntersectionStats(rectIntersectionStats::testNine());
  testIntersectionStats(rectIntersectionStats::testTen());
}
