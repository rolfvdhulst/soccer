//
// Created by rolf on 19-4-19.
//

#include <gtest/gtest.h>
#include <math/geometry/Line.h>
#include <math/geometry/LineSegment.h>
#include <math/geometry/Ray.h>
#include <math/geometry/BoundingBox2D.h>

struct LineTest{
  LineTest(Line a, Line b,  std::optional<Vector2> intersect=std::nullopt) :
      first{a}, second{b},  intersect{intersect}{}
  Line first;
  Line second;
  std::optional<Vector2> intersect;
};
//rotates the lines by 180 degrees to generate 4 test cases out of one.
std::vector<LineTest> swappedTests(const LineTest& test){
  LineTest copy = test;
  std::vector<LineTest> tests = {copy};
  copy.first = Line(copy.first.end(),copy.first.start());
  tests.push_back(copy);
  copy.second = Line(copy.second.end(),copy.second.start());
  tests.push_back(copy);
  copy.first = test.first;
  tests.push_back(copy);
  return tests;
};

//colinear tests
// CL= colinear, P = Parallel,
// OL = OVERLAPPING, NOL = not overlaping, T= touching. Only OL tests are not invariant under swaps, rest should be.
// H=Horizontal, V = Vertical
const static LineTest CL_OL{Line(Vector2(0,0),Vector2(2,2)),Line(Vector2(1,1),Vector2(3,3))};
const static LineTest CL_NOL{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(2,2),Vector2(3,3))};
const static LineTest CL_T{Line(Vector2(0,0),Vector2(2,2)),Line(Vector2(2,2),Vector2(3,3)),Vector2(2,2)};
const static LineTest CL_OL_H{Line(Vector2(1,1),Vector2(3,1)),Line(Vector2(2,1),Vector2(4,1))};
const static LineTest CL_NOL_H{Line(Vector2(1,1),Vector2(2,1)),Line(Vector2(3,1),Vector2(4,1))};
const static LineTest CL_T_H{Line(Vector2(1,1),Vector2(3,1)),Line(Vector2(3,1),Vector2(4,1)),Vector2(3,1)};
const static LineTest CL_OL_V{Line(Vector2(1,1),Vector2(1,3)),Line(Vector2(1,2),Vector2(1,4))};
const static LineTest CL_NOL_V{Line(Vector2(1,1),Vector2(1,2)),Line(Vector2(1,3),Vector2(1,4))};
const static LineTest CL_T_V{Line(Vector2(1,1),Vector2(1,3)),Line(Vector2(1,3),Vector2(1,4)),Vector2(1,3)};
//parralel tests NP = not parralel, P = parralel
const static LineTest NP{Line(Vector2(1,0),Vector2(2,1)),Line(Vector2(0,0),Vector2(2,3))};
const static LineTest NP_H{Line(Vector2(1,0),Vector2(2,1)),Line(Vector2(2,5),Vector2(3,5))};
const static LineTest NP_V{Line(Vector2(1,0),Vector2(2,1)),Line(Vector2(5,2),Vector2(5,3))};
const static LineTest NP_HV{Line(Vector2(1,0),Vector2(2,0)),Line(Vector2(5,2),Vector2(5,3))};

const static LineTest P{Line(Vector2(1,0),Vector2(2,1)),Line(Vector2(-1,0),Vector2(0,1))};
const static LineTest P_H{Line(Vector2(1,1),Vector2(2,1)),Line(Vector2(2,2),Vector2(3,2))};
const static LineTest P_V{Line(Vector2(1,1),Vector2(1,2)),Line(Vector2(2,2),Vector2(2,3))};


//Segment segment tests
const static LineTest SS1{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(1,0),Vector2(0,1)),Vector2(0.5,0.5)};
const static LineTest SS2{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(2,0),Vector2(0,2)),Vector2(1,1)};
const static LineTest SS3{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(2,0),Vector2(1,1)),Vector2(1,1)};

const static LineTest SS4{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(0.2,0.8),Vector2(0,1)),std::nullopt};//intersects in extension
const static LineTest SS5{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(1.2,1.8),Vector2(1,2)),std::nullopt};//intersects in extension
const static LineTest SS6{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(0,2),Vector2(0.5,1.5)),std::nullopt};

//Line Segment tests. The second line is assumed to be a segment every time
const static LineTest LS1{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(3,0),Vector2(0,3)),Vector2(1.5,1.5)};
const static LineTest LS1_r{Line(Vector2(3,0),Vector2(0,3)),Line(Vector2(0,0),Vector2(1,1)),std::nullopt};
const static LineTest LS2{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(1,2),Vector2(0,3)),std::nullopt}; //intersects too short

const static LineTest LR1{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(0,4),Vector2(1,3)),Vector2(2,2)};
const static LineTest LR1_r{Line(Vector2(0,0),Vector2(1,1)),Line(Vector2(1,3),Vector2(0,4)),std::nullopt};
double invalidConstructor(){
  Line line(Vector2(0,1),Vector2(0,1));
  return line.start().x();
}
TEST(LineTests, constructor){
  Vector2 A(1,1), B(3,2);
  Line line(A,B);
  Ray ray(A,B);
  LineSegment segment(A,B);
  EXPECT_DOUBLE_EQ(line.start().x(),1);
  EXPECT_DOUBLE_EQ(line.start().y(),1);
  EXPECT_DOUBLE_EQ(line.end().x(),3);
  EXPECT_DOUBLE_EQ(line.end().y(),2);

  EXPECT_EQ(line.start(),ray.start());
  EXPECT_EQ(ray.start(),segment.start());
  EXPECT_EQ(segment.start(),line.start());
  EXPECT_EQ(line.end(),ray.end());
  EXPECT_EQ(ray.end(),segment.end());
  EXPECT_EQ(segment.end(),line.end());

  EXPECT_EQ(line.direction(),B-A);

  //invalid constructor should throw
  EXPECT_EXIT(invalidConstructor(),::testing::KilledBySignal(SIGABRT),"");
}
TEST(LineTests, move){
  Line line(Vector2(0,1),Vector2(2,3));
  line.move(Vector2(2,1));
  EXPECT_EQ(line.start().x(),2);
  EXPECT_EQ(line.start().y(),2);
  EXPECT_EQ(line.end().x(),4);
  EXPECT_EQ(line.end().x(),4);
}
TEST(LineTests, boundingBoxes){
  LineSegment lineSegment(Vector2(0,1),Vector2(2,3));
  for (const auto& segment : {lineSegment,lineSegment.reversed()}){
    BoundingBox2D box = segment.boundingBox();
    EXPECT_EQ(box.xMin(),0);
    EXPECT_EQ(box.xMax(),2);
    EXPECT_EQ(box.yMin(),1);
    EXPECT_EQ(box.yMax(),3);
  }
  Line line(lineSegment);
  BoundingBox2D infiniteBox = line.boundingBox();
  EXPECT_EQ(infiniteBox.xMin(),-std::numeric_limits<double>::infinity());
  EXPECT_EQ(infiniteBox.xMax(),std::numeric_limits<double>::infinity());
  EXPECT_EQ(infiniteBox.yMin(),-std::numeric_limits<double>::infinity());
  EXPECT_EQ(infiniteBox.yMax(),std::numeric_limits<double>::infinity());

  Ray ray(lineSegment);
  infiniteBox = ray.boundingBox();
  EXPECT_EQ(infiniteBox.xMin(),0);
  EXPECT_EQ(infiniteBox.xMax(),std::numeric_limits<double>::infinity());
  EXPECT_EQ(infiniteBox.yMin(),1);
  EXPECT_EQ(infiniteBox.yMax(),std::numeric_limits<double>::infinity());
  infiniteBox = Ray(lineSegment.reversed()).boundingBox();
  EXPECT_EQ(infiniteBox.xMin(),-std::numeric_limits<double>::infinity());
  EXPECT_EQ(infiniteBox.xMax(),2);
  EXPECT_EQ(infiniteBox.yMin(),-std::numeric_limits<double>::infinity());
  EXPECT_EQ(infiniteBox.yMax(),3);

  Line horizontal(Vector2(0,1),Vector2(1,1));
  BoundingBox2D horizontalBox = horizontal.boundingBox();
  EXPECT_EQ(horizontalBox.xMin(),-std::numeric_limits<double>::infinity());
  EXPECT_EQ(horizontalBox.xMax(),std::numeric_limits<double>::infinity());
  EXPECT_EQ(horizontalBox.yMin(),1);
  EXPECT_EQ(horizontalBox.yMax(),1);

  Ray horizontalRay(horizontal);
  horizontalBox = horizontalRay.boundingBox();
  EXPECT_EQ(horizontalBox.xMin(),0);
  EXPECT_EQ(horizontalBox.xMax(),std::numeric_limits<double>::infinity());
  EXPECT_EQ(horizontalBox.yMin(),1);
  EXPECT_EQ(horizontalBox.yMax(),1);

  Line vertical(Vector2(1,1),Vector2(1,8));
  BoundingBox2D verticalBox = vertical.boundingBox();
  EXPECT_EQ(verticalBox.xMin(),1);
  EXPECT_EQ(verticalBox.xMax(),1);
  EXPECT_EQ(verticalBox.yMin(),-std::numeric_limits<double>::infinity());
  EXPECT_EQ(verticalBox.yMax(),std::numeric_limits<double>::infinity());

  Ray verticalRay(vertical);
  verticalBox = verticalRay.boundingBox();
  EXPECT_EQ(verticalBox.xMin(),1);
  EXPECT_EQ(verticalBox.xMax(),1);
  EXPECT_EQ(verticalBox.yMin(),1);
  EXPECT_EQ(verticalBox.yMax(),std::numeric_limits<double>::infinity());
}
TEST(LineTests, colinear){
  std::vector<LineTest> colinears = {CL_OL,CL_OL,CL_NOL,CL_T,CL_OL_H,CL_NOL_H,CL_T_H,CL_OL_V,CL_NOL_V,CL_T_V};
  std::vector<LineTest> nonColinears = {NP,NP_H,NP_V,NP_HV,P,P_H,P_V};
  for (const auto& startTest : colinears) {
    for(const auto& test : swappedTests(startTest)){
      EXPECT_TRUE(test.first.isColinear(test.second));
      EXPECT_TRUE(test.second.isColinear(test.first));
    }
  }
  for (const auto& startTest : nonColinears) {
    for(const auto& test : swappedTests(startTest)){
      EXPECT_FALSE(test.first.isColinear(test.second));
      EXPECT_FALSE(test.second.isColinear(test.first));
    }
  }
}
TEST(LineTests, horizontal) {
  std::vector<LineTest> horizontals = {CL_OL_H,CL_NOL_H,CL_T_H};
  std::vector<LineTest> nonHorizontals={CL_OL,CL_NOL,CL_T,CL_OL_V,CL_NOL_V,CL_T_V};
  for (const auto& startTest : horizontals) {
    for(const auto& test : swappedTests(startTest)){
      EXPECT_TRUE(test.first.isHorizontal());
      EXPECT_TRUE(test.second.isHorizontal());
    }
  }
  for (const auto& startTest : nonHorizontals) {
    for(const auto& test : swappedTests(startTest)){
      EXPECT_FALSE(test.first.isHorizontal());
      EXPECT_FALSE(test.second.isHorizontal());
    }
  }
}
TEST(LineTests, vertical) {
  std::vector<LineTest> verticals = {CL_OL_V,CL_NOL_V,CL_T_V};
  std::vector<LineTest> nonVerticals={CL_OL,CL_NOL,CL_T,CL_OL_H,CL_NOL_H,CL_T_H};
  for (const auto& startTest : verticals) {
    for(const auto& test : swappedTests(startTest)){
      EXPECT_TRUE(test.first.isVertical());
      EXPECT_TRUE(test.second.isVertical());
    }
  }
  for (const auto& startTest : nonVerticals) {
    for(const auto& test : swappedTests(startTest)){
      EXPECT_FALSE(test.first.isVertical());
      EXPECT_FALSE(test.second.isVertical());
    }
  }
}
TEST(LineTests, parallel){
  std::vector<LineTest> parralels = {P,P_H,P_V,CL_OL,CL_OL,CL_NOL,CL_T,CL_OL_H,CL_NOL_H,CL_T_H,CL_OL_V,CL_NOL_V,CL_T_V};
  std::vector<LineTest> nonParralels = {NP,NP_H,NP_V,NP_HV};
  for (const auto& startTest : parralels) {
    for(const auto& test : swappedTests(startTest)){
      EXPECT_TRUE(test.first.isParallel(test.second));
      EXPECT_TRUE(test.second.isParallel(test.first));
    }
  }
  for (const auto& startTest : nonParralels) {
    for(const auto& test : swappedTests(startTest)){
      EXPECT_FALSE(test.first.isParallel(test.second));
      EXPECT_FALSE(test.second.isParallel(test.first));
    }
  }
}
TEST(LineTests, point){
  Line pointLine; //should default construct without throwing an error.
  EXPECT_TRUE(pointLine.isPoint());
  EXPECT_FALSE(NP_HV.second.isPoint());
  EXPECT_FALSE(NP.first.isPoint());
  EXPECT_FALSE(NP_HV.first.isPoint());
}

TEST(LineTests, reverse) {
  Vector2 A(1, 1), B(2, 2);
  LineSegment x(A, B);
  EXPECT_EQ(x.start(), A);
  EXPECT_EQ(x.end(), B);
  x.reverse();
  EXPECT_EQ(x.start(), B);
  EXPECT_EQ(x.end(), A);

  LineSegment y(A, B);
  EXPECT_EQ(y.start(), A);
  EXPECT_EQ(y.end(), B);
  LineSegment z = y.reversed();
  EXPECT_EQ(z.start(), B);
  EXPECT_EQ(z.end(), A);
}
TEST(LineTests, center) {
  Vector2 A(1, 2), B(3, 6);
  LineSegment line(A, B);
  EXPECT_EQ(line.center().x(), 2);
  EXPECT_EQ(line.center().y(), 4);
}

TEST(LineTests, slopeAndIntercept) {
    Vector2 Av(1.0, 1.0), Bv(2.0, 2.0), Cv(2.0, 3.0), Dv(1.0, 4.0), Ev(2.0, 0.0);
    Line D(Av, Bv), E(Av, Cv), H(Av, Ev);
    LineSegment F(Av, Dv), G(Dv, Av);
    Line Dcopy(Bv, Av), Ecopy(Cv, Av), Hcopy(Ev, Av);

    // test normal slopes
    EXPECT_DOUBLE_EQ(D.slope(), 1.0);
    EXPECT_DOUBLE_EQ(E.slope(), 2.0);
    EXPECT_DOUBLE_EQ(H.slope(), -1.0);
    // test vertical lines giving back numeric limits
    EXPECT_DOUBLE_EQ(F.slope(), std::numeric_limits<double>::infinity());
    EXPECT_DOUBLE_EQ(G.slope(), -std::numeric_limits<double>::infinity());

    // make sure the functions are commutative
    EXPECT_DOUBLE_EQ(D.slope(), Dcopy.slope());
    EXPECT_DOUBLE_EQ(E.slope(), Ecopy.slope());
    EXPECT_DOUBLE_EQ(H.slope(), Hcopy.slope());

    // calculate the intercept for normal cases and check commutativity
    EXPECT_DOUBLE_EQ(D.intercept(), 0.0);
    EXPECT_DOUBLE_EQ(E.intercept(), -1.0);
    EXPECT_DOUBLE_EQ(H.intercept(), 2.0);
    EXPECT_DOUBLE_EQ(D.intercept(), Dcopy.intercept());
    EXPECT_DOUBLE_EQ(E.intercept(), Ecopy.intercept());
    EXPECT_DOUBLE_EQ(H.intercept(), Hcopy.intercept());

    EXPECT_DOUBLE_EQ(F.intercept(), -std::numeric_limits<double>::infinity());
    EXPECT_DOUBLE_EQ(G.intercept(), std::numeric_limits<double>::infinity());

    EXPECT_DOUBLE_EQ(D.coefficients().first, D.slope());
    EXPECT_DOUBLE_EQ(E.coefficients().first, E.slope());
    EXPECT_DOUBLE_EQ(F.coefficients().first, F.slope());
    EXPECT_DOUBLE_EQ(G.coefficients().first, G.slope());
    EXPECT_DOUBLE_EQ(H.coefficients().first, H.slope());
    EXPECT_DOUBLE_EQ(D.coefficients().second, D.intercept());
    EXPECT_DOUBLE_EQ(E.coefficients().second, E.intercept());
    EXPECT_DOUBLE_EQ(F.coefficients().second, F.intercept());
    EXPECT_DOUBLE_EQ(G.coefficients().second, G.intercept());
    EXPECT_DOUBLE_EQ(H.coefficients().second, H.intercept());
}

TEST(LineTests, distanceToPoint) {
    Vector2 A(0.0, 0.0), B(2.0, 2.0);
    Vector2 point1(2.0, 0.0), point2(4.0, 6.0);
    Line l1(A, B);
    LineSegment l2(A, B);

    EXPECT_EQ(l1.distanceTo(point1), sqrt(2.0));
    EXPECT_EQ(l2.distanceTo(point1), sqrt(2.0));
    EXPECT_EQ(l1.distanceTo(point2), sqrt(2.0));
    EXPECT_EQ(l2.distanceTo(point2), sqrt(20.0));

    Vector2 shouldProj1(1.0, 1.0), shouldProj2(5.0, 5.0);
    EXPECT_EQ(l1.project(point1), shouldProj1);
    EXPECT_EQ(l2.project(point1), shouldProj1);
    EXPECT_EQ(l1.project(point2), shouldProj2);
    EXPECT_EQ(l2.project(point2), B);
}
TEST(LineTests, pointOnLine) {
    Vector2 A(1.0, 1.0), B(3.0, 1.0), C(1.0, 3.0), D(3.0, 3.0);
    Line l1(A, B), l2(A, C), l3(A, D);
    LineSegment ls1(A, B), ls2(A, C), ls3(A, D);
    Vector2 point1(2.0, 1.0), point2(4.0, 1.0);
    EXPECT_TRUE(l1.hits(point1));
    EXPECT_TRUE(l1.hits(point2));
    EXPECT_TRUE(ls1.hits(point1));
    EXPECT_FALSE(ls1.hits(point2));

    // check the original points
    EXPECT_TRUE(l1.hits(B));
    EXPECT_TRUE(ls1.hits(B));
    EXPECT_TRUE(l1.hits(A));
    EXPECT_TRUE(ls1.hits(A));

    Vector2 point3(1.0, 2.0), point4(1.0, 4.0);
    EXPECT_TRUE(l2.hits(point3));
    EXPECT_TRUE(l2.hits(point4));
    EXPECT_TRUE(ls2.hits(point3));
    EXPECT_FALSE(ls2.hits(point4));  // check the original points

    EXPECT_TRUE(l2.hits(C));
    EXPECT_TRUE(ls2.hits(C));
    EXPECT_TRUE(l2.hits(A));
    EXPECT_TRUE(ls2.hits(A));

    Vector2 point5(2.0, 2.0), point6(4.0, 4.0);
    EXPECT_TRUE(l3.hits(point5));
    EXPECT_TRUE(l3.hits(point6));
    EXPECT_TRUE(ls3.hits(point5));
    EXPECT_FALSE(ls3.hits(point6));  // check the original points

    EXPECT_TRUE(l3.hits(D));
    EXPECT_TRUE(ls3.hits(D));
    EXPECT_TRUE(l3.hits(A));
    EXPECT_TRUE(ls3.hits(A));

    Vector2 point7(0.0, 1.0), point8(0.0, 2.0);
    EXPECT_FALSE(ls1.hits(point7));
    EXPECT_FALSE(ls1.hits(point8));
}

TEST(LineTests, LineLineIntersection) {
  //Line line intersections
  std::vector<LineTest> intersections{NP,NP_H,NP_V,NP_HV,CL_OL,CL_NOL,CL_T,CL_OL_H,CL_NOL_H,CL_T_H,CL_OL_V,CL_NOL_V,CL_T_V};
  std::vector<LineTest> notIntersections{P,P_H,P_V};
  for (const auto& startTest : intersections) {
    for(const auto& test : swappedTests(startTest)){
      EXPECT_TRUE(test.first.doesIntersect(test.second));
      EXPECT_TRUE(test.second.doesIntersect(test.first));
      EXPECT_TRUE(test.first.intersects(test.second) != std::nullopt);
      EXPECT_TRUE(test.second.intersects(test.first) != std::nullopt);
      //If lines are colinear we define the intersection point as the start of the first line passed
      if(test.first.isColinear(test.second)){
        EXPECT_EQ(test.first.intersects(test.second),test.first.start());
        EXPECT_EQ(test.second.intersects(test.first),test.second.start());
      }
      if(test.intersect != std::nullopt && ! test.first.isColinear(test.second)){
          //If not, the provided argument should be correct
          EXPECT_EQ(test.intersect,test.first.intersects(test.second));
          EXPECT_EQ(test.intersect,test.second.intersects(test.first));
      }
    }
  }
  for (const auto& startTest : notIntersections) {
    for(const auto& test : swappedTests(startTest)){
      EXPECT_FALSE(test.first.doesIntersect(test.second));
      EXPECT_FALSE(test.second.doesIntersect(test.first));
      EXPECT_TRUE(test.first.intersects(test.second) == std::nullopt);
      EXPECT_TRUE(test.second.intersects(test.first) == std::nullopt);
    }
  }
}
TEST(LineTests, SegmentSegmentIntersection) {
  //Line line intersections
  std::vector<LineTest> overlappingCL{CL_OL,CL_OL_H,CL_OL_V};
  std::vector<LineTest> notIntersectingParralel{CL_NOL,CL_NOL_H,CL_NOL_V,P,P_H,P_V};
  std::vector<LineTest> touchingCL{CL_T,CL_T_H,CL_T_V};
  std::vector<LineTest> intersections{SS1,SS2,SS3,SS4,SS5,SS6};
  for (const auto& startTest : overlappingCL) {
    for(const auto& test : swappedTests(startTest)){
      LineSegment first(test.first);
      LineSegment second(test.second);
      EXPECT_TRUE(first.doesIntersect(second));
      EXPECT_TRUE(second.doesIntersect(first));
    }
  }
  //TODO: verify that segment segment on colinear line overlap returns the closest point to the start.

  int i = 0;
  for (const auto& startTest : touchingCL) {
    i++;
    for(const auto& test : swappedTests(startTest)){
      LineSegment first(test.first);
      LineSegment second(test.second);
      EXPECT_TRUE(first.doesIntersect(second));
      EXPECT_TRUE(second.doesIntersect(first));
      EXPECT_EQ(first.intersects(second),test.intersect);
      EXPECT_EQ(second.intersects(first),test.intersect);
    }
  }
  for (const auto& startTest : notIntersectingParralel) {
    for(const auto& test : swappedTests(startTest)){
      LineSegment first(test.first);
      LineSegment second(test.second);
      EXPECT_FALSE(first.doesIntersect(second));
      EXPECT_FALSE(second.doesIntersect(first));
      EXPECT_EQ(first.intersects(second),std::nullopt);
      EXPECT_EQ(second.intersects(first),std::nullopt);
    }
  }
  for (const auto& startTest : intersections) {
    for(const auto& test : swappedTests(startTest)){
      LineSegment first(test.first);
      LineSegment second(test.second);
      bool intersects = test.intersect != std::nullopt;
      EXPECT_TRUE(first.doesIntersect(second) == intersects);
      EXPECT_TRUE(second.doesIntersect(first) == intersects);
      EXPECT_EQ(first.intersects(second),test.intersect);
      EXPECT_EQ(second.intersects(first),test.intersect);
    }
  }
}
TEST(LineTests, LineSegmentIntersection){
  std::vector<LineTest> overlappingCL{CL_OL,CL_OL_H,CL_OL_V};
  std::vector<LineTest> notOverlappingCL{CL_NOL,CL_NOL_H,CL_NOL_V,CL_T,CL_T_H,CL_T_V};
  std::vector<LineTest> notIntersectingParralel{P,P_H,P_V};
  std::vector<LineTest> intersections{SS1,SS2,SS3,LS1,LS1_r,LS2};
  for (const auto& startTest : overlappingCL) {
    for(const auto& test : swappedTests(startTest)){
      Line first(test.first);
      LineSegment second(test.second);
      EXPECT_TRUE(first.doesIntersect(second));
      EXPECT_TRUE(second.doesIntersect(first));
      //TODO: intersection is closest to start as it is on the segment, test this
      EXPECT_TRUE(first.intersects(second)!= std::nullopt);
      EXPECT_TRUE(second.intersects(first)!= std::nullopt);
    }
  }
  for (const auto& startTest : notOverlappingCL) {
    for(const auto& test : swappedTests(startTest)){
      Line first(test.first);
      LineSegment second(test.second);
      EXPECT_TRUE(first.doesIntersect(second));
      EXPECT_TRUE(second.doesIntersect(first));
      //intersection is closest to start
      if ((first.start()-second.start()).length2()<((first.start()-second.end()).length2())){
        EXPECT_EQ(*first.intersects(second),second.start());
      }else{
        EXPECT_EQ(*first.intersects(second),second.end());
      }
      EXPECT_EQ(*second.intersects(first),second.start());
    }
  }
  for (const auto& startTest : notIntersectingParralel) {
    for(const auto& test : swappedTests(startTest)){
      Line first(test.first);
      LineSegment second(test.second);
      EXPECT_FALSE(first.doesIntersect(second));
      EXPECT_FALSE(second.doesIntersect(first));
      EXPECT_EQ(first.intersects(second),std::nullopt);
      EXPECT_EQ(second.intersects(first),std::nullopt);
    }
  }
  for (const auto& startTest : intersections) {
    for(const auto& test : swappedTests(startTest)){
      Line first(test.first);
      LineSegment second(test.second);
      bool intersects = test.intersect != std::nullopt;
      EXPECT_TRUE(first.doesIntersect(second) == intersects);
      EXPECT_TRUE(second.doesIntersect(first) == intersects);
      EXPECT_EQ(first.intersects(second),test.intersect);
      EXPECT_EQ(second.intersects(first),test.intersect);
    }
  }
}
TEST(LineTests, LineRayIntersection){
  std::vector<LineTest> overlappingCL{CL_OL,CL_OL_H,CL_OL_V,CL_NOL,CL_NOL_H,CL_NOL_V,CL_T,CL_T_H,CL_T_V};
  std::vector<LineTest> notIntersectingParralel{P,P_H,P_V};
  std::vector<LineTest> symmetricIntersections{SS1,SS2,SS3};
  std::vector<LineTest> intersections{LR1,LR1_r};
  for (const auto& startTest : overlappingCL) {
    for(const auto& test : swappedTests(startTest)){
      Line first(test.first);
      Ray second(test.second);
      EXPECT_TRUE(first.doesIntersect(second));
      EXPECT_TRUE(second.doesIntersect(first));
      //TODO: intersection is closest to start as it is on the segment, test this
      EXPECT_TRUE(first.intersects(second)!= std::nullopt);
      EXPECT_TRUE(second.intersects(first)!= std::nullopt);
    }
  }

  for (const auto& startTest : notIntersectingParralel) {
    for(const auto& test : swappedTests(startTest)){
      Line first(test.first);
      Ray second(test.second);
      EXPECT_FALSE(first.doesIntersect(second));
      EXPECT_FALSE(second.doesIntersect(first));
      EXPECT_EQ(first.intersects(second),std::nullopt);
      EXPECT_EQ(second.intersects(first),std::nullopt);
    }
  }
  for (const auto& startTest : symmetricIntersections) {
    for(const auto& test : swappedTests(startTest)){
      Line first(test.first);
      Ray second(test.second);
      bool intersects = test.intersect != std::nullopt;
      EXPECT_TRUE(first.doesIntersect(second) == intersects);
      EXPECT_TRUE(second.doesIntersect(first) == intersects);
      EXPECT_EQ(first.intersects(second),test.intersect);
      EXPECT_EQ(second.intersects(first),test.intersect);
    }
  }
  for (const auto& test : intersections){
    Line first(test.first);
    Ray second(test.second);
    bool intersects = test.intersect != std::nullopt;
    EXPECT_TRUE(first.doesIntersect(second) == intersects);
    EXPECT_TRUE(second.doesIntersect(first) == intersects);
    EXPECT_EQ(first.intersects(second),test.intersect);
    EXPECT_EQ(second.intersects(first),test.intersect);
  }

}
