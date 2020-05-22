//
// Created by rolf on 07-03-20.
//

#ifndef SOCCER_FIELDSTATE_H
#define SOCCER_FIELDSTATE_H

#include <math/geometry/Circle.h>
#include <math/geometry/LineSegment.h>
#include <math/geometry/Rectangle.h>
#include <protobuf/messages_robocup_ssl_geometry.pb.h>

// All things with 'our' are named for convenience.
// All of these may be wrong if FieldState is not flipped correctly yet!
enum class DefaultField { DivisionA, DivisionB, DemoField };
class FieldState {
    // The public block below are all getters that do not explicitly copy (you can copy though) and should all be very simple after initialization
   public:
    [[nodiscard]] double getFieldLength() const;
    [[nodiscard]] double getFieldWidth() const;

    [[nodiscard]] double getGoalWidth() const;
    [[nodiscard]] double getGoalDepth() const;
    [[nodiscard]] double getGoalHeight() const;
    [[nodiscard]] double getGoalWallThickness() const;

    [[nodiscard]] double getLineThickness() const;
    [[nodiscard]] double getBoundaryWidth() const;

    [[nodiscard]] double getLeftX() const;
    [[nodiscard]] double getRightX() const;
    [[nodiscard]] double getTopY() const;
    [[nodiscard]] double getBottomY() const;
    [[nodiscard]] double getLeftMarginX() const;
    [[nodiscard]] double getRightMarginX() const;
    [[nodiscard]] double getTopMarginY() const;
    [[nodiscard]] double getBottomMarginY() const;

    [[nodiscard]] const Rectangle& getOurDefenceArea() const;
    [[nodiscard]] const Rectangle& getTheirDefenceArea() const;
    [[nodiscard]] const Rectangle& getDefenceArea(bool us) const;
    [[nodiscard]] const Rectangle& getField() const;
    [[nodiscard]] const Rectangle& getMarginField() const;
    [[nodiscard]] const Rectangle& getField(bool withMargin) const;

    [[nodiscard]] const Rectangle& getOurGoalRectangle() const;
    [[nodiscard]] const Rectangle& getTheirGoalRectangle() const;
    [[nodiscard]] const Rectangle& getGoalRectangle(bool us) const;

    [[nodiscard]] const Circle& getCenterCircle() const;

    [[nodiscard]] const LineSegment& getLeftGoal() const;
    [[nodiscard]] const LineSegment& getOurGoal() const;
    [[nodiscard]] const LineSegment& getGoal(bool us) const;
    [[nodiscard]] const LineSegment& getRightGoal() const;
    [[nodiscard]] const LineSegment& getTheirGoal() const;

    [[nodiscard]] const Vector2& getLeftGoalCenter() const;
    [[nodiscard]] const Vector2& getOurGoalCenter() const;
    [[nodiscard]] const Vector2& getRightGoalCenter() const;
    [[nodiscard]] const Vector2& getTheirGoalCenter() const;
    [[nodiscard]] const Vector2& getGoalCenter(bool us) const;

    [[nodiscard]] const LineSegment& getTopLine() const;
    [[nodiscard]] const LineSegment& getLeftLine() const;
    [[nodiscard]] const LineSegment& getBottomLine() const;
    [[nodiscard]] const LineSegment& getRightLine() const;
    [[nodiscard]] const LineSegment& getHalfLine() const;
    [[nodiscard]] const LineSegment& getCenterLine() const;
    [[nodiscard]] const LineSegment& getLeftPenaltyLine() const;
    [[nodiscard]] const LineSegment& getRightPenaltyLine() const;

    [[nodiscard]] const LineSegment& getTopLeftPenaltyStretch() const;
    [[nodiscard]] const LineSegment& getBottomLeftPenaltyStretch() const;
    [[nodiscard]] const LineSegment& getBottomRightPenaltyStretch() const;
    [[nodiscard]] const LineSegment& getTopRightPenaltyStretch() const;

   private:
    double fieldLength;
    double fieldWidth;
    double goalWidth;
    double goalDepth;
    double boundaryWidth;

    // The following block is defined by just us and not by SSL
    double leftX;
    double rightX;
    double bottomY;
    double topY;
    double leftMarginX;
    double rightMarginX;
    double bottomMarginY;
    double topMarginY;

    double goalHeight;
    double wallThickness;

    // all SSL line data
    double lineThickness;

    LineSegment topLine;
    LineSegment leftLine;
    LineSegment bottomLine;
    LineSegment rightLine;
    LineSegment leftPenaltyLine;
    LineSegment rightPenaltyLine;

    // We generally consider the below 'unimportant' thus they are not checked for in construction.
    // We can interpolate these from the top 6 lines.
    LineSegment halfLine;
    LineSegment centerLine;
    LineSegment topLeftPenaltyStretch;
    LineSegment bottomLeftPenaltyStretch;
    LineSegment bottomRightPenaltyStretch;
    LineSegment topRightPenaltyStretch;

    Circle centerCircle;

    // Below can be computed from the above (e.g. are redundant)
    // We precompute them for speed (see accessors)

    Rectangle leftDefenceArea;
    Rectangle rightDefenceArea;

    Rectangle field;
    Rectangle fieldWithMargin;

    LineSegment leftGoal;
    LineSegment rightGoal;
    Rectangle leftGoalRectangle;
    Rectangle rightGoalRectangle;
    Vector2 leftGoalCenter;
    Vector2 rightGoalCenter;

   public:
    explicit FieldState(const proto::SSL_GeometryFieldSize& sslGeometry, DefaultField fieldBackup = DefaultField::DivisionA);
    explicit FieldState(DefaultField fieldType);
    FieldState();

   private:
        // The following are all used in the constructor...
        // rant why is this data type so complicated ugh.
    static double mmToM(double value);
    static Vector2 mmToM(const Vector2& value);
    static void mmToM(LineSegment& line);
    void fixOutsideFieldLines(bool topLineSet, bool leftLineSet, bool bottomLineSet, bool rightLineSet);
    void fixOneOutsideLineMissing(bool topLineSet, bool leftLineSet, bool bottomLineSet, bool rightLineSet);
    static bool threeTrue(bool a, bool b, bool c, bool d);
    void makeOutsideLines();
    void fixPenaltyFieldLines(bool leftPenaltyLineSet, bool rightPenaltyLineSet, DefaultField backupField);
    static double getLineThickness(const proto::SSL_GeometryFieldSize& sslGeometry);
    /* Below static functions are just Default definitions for each FieldType.
     */
    [[nodiscard]] static double getFieldLength(const DefaultField& field);
    [[nodiscard]] static double getFieldWidth(const DefaultField& field);
    [[nodiscard]] static double getGoalDepth(const DefaultField& field);
    [[nodiscard]] static double getGoalWidth(const DefaultField& field);
    [[nodiscard]] static double getBoundaryWidth(const DefaultField& field);
    [[nodiscard]] static double getLineThickness(const DefaultField& field);
    void setCoreLines(const proto::SSL_GeometryFieldSize& sslGeometry, const DefaultField& backupField);
    void setOtherLines(const proto::SSL_GeometryFieldSize& sslGeometry);
    void setCenterCircle(const proto::SSL_GeometryFieldSize& sslGeometry);
    void defineDefenceRectangles();
    void defineFieldRectangles();
    void defineGoals();
    void makeOtherLines(bool halfLineSet, bool centerLineSet, bool bottomLeftPenaltyStretchSet, bool topLeftPenaltyStretchSet, bool bottomRightPenaltyStretchSet,
                        bool topRightPenaltyStretchSet);
};
#endif  // SOCCER_FIELDSTATE_H
