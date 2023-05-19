
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
    const char* testName = "Test One";
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
    const char* testName = "Test Two";
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
    const char* testName = "Test Three";
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
    const char* testName = "Test Four";
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
    const char* testName = "Test Five";
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
      r1.area() + r2.area() + r3.area() - 5*5 - 10*10 - 60*60);
  }

  TestEntry testSix()
  {
    const char* testName = "Test Six";
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
    const char* testName = "Test Seven";
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
    const char* testName = "Test Eight";
    std::vector<rfb::Rect> intersections;
    rfb::Rect r1(0,0,0,0);
    rfb::Rect r2(1,1,1,1);
    rfb::Rect r3(0,0,1,1);

    intersections.push_back(r1);
    intersections.push_back(r2);
    intersections.push_back(r3);

    return TestEntry(testName, intersections, r3.area());
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
}
