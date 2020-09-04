//
// Created by rolf on 24-4-19.
//

#ifndef SOCCER_POLYGON_H
#define SOCCER_POLYGON_H

#include <numeric>
#include "Shape.h"
#include "Vector2.h"
#include "LineSegment.h"
#include "BoundingBox2D.h"
#include "Ray.h"

template<std::size_t N> //templated with the amount of vertices
class Polygon : public Shape {
  static_assert(N>2);// Polygon's of size 2 or smaller do not make sense.
  static_assert(N<=50);// This class is not built for large number of vertices. Some functions do well but other's don't.
 private:
  std::array<Vector2,N> vertices;
  //TODO: perhaps precompute bounding box and boundary?
 public:
  /**
   * @brief Construct a new Polygon object
   * @param vertices Vertices that will literally be copied to this->vertices
   */
  explicit constexpr Polygon(const std::vector<Vector2>& _vertices){
    assert(_vertices.size() == N);
    std::copy_n(std::make_move_iterator(_vertices.begin()),N,vertices.begin());
  }
  template<class ... Args>
  explicit constexpr Polygon(Args &&... args) :vertices{Vector2(args)...}{
    static_assert(sizeof...(Args) == N, "Invalid number of constructor arguments");
  }
  /**
   * @brief Moves a polygon
   *
   * @param moveBy Moves the polygon by this much
   */
  void move(const Vector2& by) override{
    for(Vector2& vertex : vertices){
      vertex += by;
    }
  }
  /**
   * @brief Checks whether a point is contained within this polygon. Points on the boundary are included into the polygon
   * TODO: On the boundary this function does not work!! see documentation if you are interested
   * Note: If your polygon is not simple this function is ill-defined as there is no good definition of 'inside'
   * @param point Point to check
   * @return true True if contained
   * @return false False if not contained
   */
  // https://wrf.ecse.rpi.edu/Research/Short_Notes/pnpoly.html
  // this is black magic but if it works it works
  //TODO: fix boundary? Might cost a lot of performance, though.
  [[nodiscard]] bool contains(const Vector2 &point) const override{
    //Efficient implementation for triangle, also counts boundary
    if(N==3){
      Vector2 as = point - vertices[0];
      bool s_ab = (vertices[1]-vertices[0]).cross(as) >= 0;
      if (((vertices[2]-vertices[0]).cross(as)> 0) == s_ab) {
        return false;
      }
      return ((vertices[2]-vertices[1]).cross(point-vertices[1])>=0) == s_ab;
    }else if(N==4){
      //Divide quadrilateral up into two triangles. This does assume that the quadrilateral is simple
      Polygon<3> triangleOne(vertices[0],vertices[1],vertices[2]);
      Polygon<3> triangleTwo(vertices[0],vertices[3],vertices[2]);
      return triangleOne.contains(point) || triangleTwo.contains(point);
    }
    else{
      bool c = false;
      for (std::size_t i = 0, j = N - 1; i < N; j = i++) {
        if (((vertices[i].y() > point.y()) != (vertices[j].y() > point.y())) &&
            (point.x() < (vertices[j].x() - vertices[i].x()) * (point.y() - vertices[i].y()) / (vertices[j].y() - vertices[i].y()) + vertices[i].x()))
          c = !c;
      }
      return c;
    }
  }
  /**
   * @brief Returns the vector at index \ref idx
   *
   * @param idx Index in vector
   * @return Vector2 Vector that is at index \ref
   */
  [[nodiscard]] const Vector2& operator[](size_t idx) const{
    return vertices[idx];
  }
  /**
   * Computes the bounding box of the polygon
   * @return
   */
  [[nodiscard]] BoundingBox2D boundingBox() const override{
    double minX = std::numeric_limits<double>::infinity();
    double minY = std::numeric_limits<double>::infinity();
    double maxX = -std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();
    for (const auto& vertex :vertices){
      minX = fmin(vertex.x(),minX);
      minY = fmin(vertex.y(),minY);
      maxX = fmax(vertex.x(),maxX);
      maxY = fmax(vertex.y(),maxY);
    }
    return BoundingBox2D(minX,minY,maxX,maxY);
  }
  /**
   * @brief Checks whether a Line intersects a polygon
   *
   * @param line Line to check
   * @return true True if it's intersecting
   * @return false False if it's not intersecting
   */
  [[nodiscard]] bool doesIntersect(const LineSegment &line) const override{
    for (size_t i = 0; i < N; i++) {
      LineSegment segment(vertices[i], vertices[(i + 1) % N]);
      if (line.doesIntersect(segment)) {
        return true;
      }
    }
    return false;
  }
  /**
   * @brief Gets intersections with a polygon and line
   *
   * @param line Line to get intersections with
   * @return std::vector<Vector2> Vector filled with vector representations of the intersections
   */
  [[nodiscard]] std::vector<Vector2> intersects(const LineSegment &line) const override{
    std::vector<Vector2> intersections;
    for (size_t i = 0; i < N; i++) {
      LineSegment segment(vertices[i], vertices[(i + 1) % N]);
      // maybe there is a nice way to do this 'circular' access with iterators?
      // the nonSimple intersects does not count any intersection that only touch the start or end of the line
      auto intersect = line.nonSimpleIntersects(segment);
      if (intersect) {
        intersections.push_back(*intersect);
      }
    }
    return intersections;
  }
  /**
 * @brief Checks whether a Ray intersects a polygon
 *
 * @param ray Ray to check
 * @return true True if it's intersecting
 * @return false False if it's not intersecting
 */
  [[nodiscard]] bool doesIntersect(const Ray &ray) const override{
    for (size_t i = 0; i < N; i++) {
      LineSegment segment(vertices[i], vertices[(i + 1) % N]);
      if (ray.doesIntersect(segment)) {
        return true;
      }
    }
    return false;
  }
  /**
   * @brief Gets intersections with a polygon and line
   *
   * @param line Line to get intersections with
   * @return std::vector<Vector2> Vector filled with vector representations of the intersections
   */
  [[nodiscard]] std::vector<Vector2> intersects(const Ray &ray) const override{
    std::vector<Vector2> intersections;
    for (size_t i = 0; i < N; i++) {
      LineSegment segment(vertices[i], vertices[(i + 1) % N]);
      // maybe there is a nice way to do this 'circular' access with iterators?
      // the nonSimple intersects does not count any intersection that only touch the start or end of the line
      auto intersect = segment.nonSimpleIntersects(ray);
      if (intersect) {
        intersections.push_back(*intersect);
      }
    }
    return intersections;
  }
  /**
   * @brief Gets the centroid of this polygon
   *
   * @return Vector2 Vector representation of the centroid
   */
  [[nodiscard]] Vector2 centroid() const{
    if (N <4){
      return verticeCentroid();
    }
    double signedAreaTwice = doubleSignedArea();
    // calculation can still make sense in a geometric way if area is 0, e.g. it becomes finding the average of points on a line
    Vector2 sum = {0, 0};
    for (std::size_t i = 0; i < N; i++) {
      sum += (vertices[i] + vertices[(i + 1) % N]) * vertices[i].cross(vertices[(i + 1) % N]);
    }
    return sum /= (3 * signedAreaTwice);
  }
  /**
   * @brief The centroid of the set of vertices.
   *
   * Is generally NOT the same as centroid for polygons with more than 3 sides
   *
   * @return Vector2 Vector representation of the centroid
   */
  [[nodiscard]] Vector2 verticeCentroid() const{
    return std::accumulate(vertices.begin(), vertices.end(), Vector2(0, 0)) /= N;
  }

  /**
   * @brief Returns the amount of vertices in this polygon
   *
   * Note: O(1)
   *
   * @return size_t Amount of vertices
   */
  [[nodiscard]] constexpr size_t amountOfVertices() const{
    return N;
  }
  /**
   * @brief Get the boundary of the polygon
   *
   * @return std::vector<LineSegment> A vector of LineSegments which represent the boundary
   */
  [[nodiscard]] std::array<LineSegment,N> getBoundary() const{
    std::array<LineSegment,N> boundary;
    for (size_t i = 0; i < N; i++) {
      boundary[i]=LineSegment(vertices[i], vertices[(i + 1) % N]);
    }
    return boundary;
  }
  /**
   * @brief Gets the length of the perimeter of the current polygon
   *
   * @return double Gets the length of the perimeter
   */
  [[nodiscard]] double perimeterLength() const{
    double totalLength = 0;
    for (size_t i = 0; i < N; i++) {
      totalLength += (vertices[i] - vertices[(i + 1) % N]).length();  // maybe there is a nice way to do this 'circular' access with iterators?
    }
    return totalLength;
  }
  /**
   * @brief Checks whether this is a convex hull
   *
   * NOTE: This function only works if your polygon is already simple!
   * In 90% of the cases when a polygon is not simple, it will not be convex
   *
   * @return true True if it's a convex
   * @return false False if not a convex
   */
  // https://stackoverflow.com/questions/471962/how-do-i-efficiently-determine-if-a-polygon-is-convex-non-convex-or-complex
  [[nodiscard]] bool isConvex() const{
    if(N<4){
      return true;
    }
    //the way this works is essentially we check if the lines keep rotating in the same direction. If not, the polygon is not convex.
    bool sign = false;
    bool signSet = false;
    for (std::size_t i = 0; i < N; i++) {
      Vector2 d1 = vertices[(i + 2) % N] - vertices[(i + 1) % N];
      Vector2 d2 = vertices[i] - vertices[(i + 1) % N];
      double cross = d1.cross(d2);
      // on a crossproduct of zero the points lie in one line and we can simply ignore this point's contribution to the convexity
      if (cross != 0.0) {
        if (!signSet) {
          signSet = true;
          sign = cross > 0;
        } else if (sign != (cross > 0)) {
          return false;
        }
      }
    }
    return true;
  }
  /**
   * @brief Checks whether the polygon is simple
   *
   * There are multiple possible algorithms, see
   * https://www.quora.com/What-is-the-simplest-algorithm-to-know-if-a-polygon-is-simple-or-not
   * this is the 'naive' O(N^2) approach which is fine for small cases (polygons with less than say 8-10 vertices)
   *
   * @return true True if simple
   * @return false False if not simple
   */
  [[nodiscard]]bool isSimple() const{
    // we loop over every unique pair
    std::vector<LineSegment> lines{};
    for (auto first = vertices.begin(); first != vertices.end(); first++) {
      LineSegment boundarySegment;
      if (first == std::prev(vertices.end())) {
        boundarySegment = LineSegment(*first, vertices[0]);
      } else {
        boundarySegment = LineSegment(*first, *(first + 1));
      }
      for (const auto& line : lines) {
        if (boundarySegment.nonSimpleDoesIntersect(line)) {
          return false;
        }
      }
      lines.push_back(boundarySegment);
    }
    return true;
  }

  /**
   * @brief Checks whether a point is on the boundary of a polygon
   *
   * @param point Point to check
   * @return true True if it's on boundary
   * @return false False if it's not on boundary
   */
  [[nodiscard]] bool isOnBoundary(const Vector2 &point) const{
    for (size_t i = 0; i < N; i++) {
      LineSegment segment(vertices[i],
                          vertices[(i + 1) % N]);  // maybe there is a nice way to do this 'circular' access with iterators?
      if (segment.hits(point)) {
        return true;
      }
    }
    return false;
  }
  /**
   * @brief Gets the area of a polygon
   *
   * @return double Area of polygon
   */
  [[nodiscard]] double area() const{
    return 0.5*abs(doubleSignedArea());
  }

  /**
   * @brief Gets the area of a polygon
   *
   * @return double Area of the polygon
   */
  // https://en.wikipedia.org/wiki/Shoelace_formula
  // area is well defined only for simple polygons
  [[nodiscard]] double doubleSignedArea() const{
    double sum = 0;
    for (std::size_t i = 0; i < N; i++) {
      sum += vertices[i].cross(vertices[(i + 1) % N]);
    }
    return sum;
  }

  [[nodiscard]] std::array<Vector2,N> corners() const{
    return vertices;
  }
};
typedef Polygon<3> Triangle;
typedef Polygon<4> Quadrilateral;
#endif  // SOCCER_POLYGON_H
