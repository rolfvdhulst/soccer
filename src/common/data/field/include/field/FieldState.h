//
// Created by rolf on 07-03-20.
//

#ifndef SOCCER_FIELDSTATE_H
#define SOCCER_FIELDSTATE_H

#include <geometry/Circle.h>
#include <geometry/LineSegment.h>
#include <geometry/Rectangle.h>
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

    const Rectangle& getOurDefenceArea();
    const Rectangle& getTheirDefenceArea();
    const Rectangle& getDefenceArea(bool us);

    const Rectangle& getField();
    const Rectangle& getMarginField();
    const Rectangle& getField(bool withMargin);

    const Rectangle& getOurGoalRectangle();
    const Rectangle& getTheirGoalRectangle();
    const Rectangle& getGoalRectangle(bool us);

    const Circle& getCenterCircle();

    const LineSegment& getLeftGoal();
    const LineSegment& getOurGoal();
    const LineSegment& getGoal(bool us);
    const LineSegment& getRightGoal();
    const LineSegment& getTheirGoal();

    const Vector2& getLeftGoalCenter();
    const Vector2& getOurGoalCenter();
    const Vector2& getRightGoalCenter();
    const Vector2& getTheirGoalCenter();
    const Vector2& getGoalCenter(bool us);

    const LineSegment& getTopLine();
    const LineSegment& getLeftLine();
    const LineSegment& getBottomLine();
    const LineSegment& getRightLine();
    const LineSegment& getHalfLine();
    const LineSegment& getCenterLine();
    const LineSegment& getLeftPenaltyLine();
    const LineSegment& getRightPenaltyLine();

    const LineSegment& getTopLeftPenaltyStretch();
    const LineSegment& getBottomLeftPenaltyStretch();
    const LineSegment& getBottomRightPenaltyStretch();
    const LineSegment& getTopRightPenaltyStretch();

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
    friend FieldState flip(const FieldState& field);

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
FieldState flip(const FieldState& field);
#endif  // SOCCER_FIELDSTATE_H
