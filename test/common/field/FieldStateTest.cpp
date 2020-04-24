//
// Created by rolf on 18-03-20.
//

#include <field/FieldState.h>
#include <gtest/gtest.h>
#include "ProtoExamples.h"

bool checkConsistency(const FieldState &field) {
    bool consistent = true;
    consistent &= field.getLeftX() == field.getFieldLength() * -0.5;
    consistent &= field.getRightX() == field.getFieldLength() * 0.5;
    consistent &= field.getBottomY() == field.getFieldWidth() * -0.5;
    consistent &= field.getTopY() == field.getFieldWidth() * 0.5;
    consistent &= field.getLeftMarginX() == (field.getFieldLength() + 2 * field.getBoundaryWidth()) * -0.5;
    consistent &= field.getRightMarginX() == (field.getFieldLength() + 2 * field.getBoundaryWidth()) * 0.5;
    consistent &= field.getBottomMarginY() == (field.getFieldWidth() + 2 * field.getBoundaryWidth()) * -0.5;
    consistent &= field.getTopMarginY() == (field.getFieldWidth() + 2 * field.getBoundaryWidth()) * 0.5;
    return consistent;
}
bool lineHorizontal(const LineSegment &line) { return line.start.y() == line.end.y(); }
bool lineVertical(const LineSegment &line) { return line.isVertical(); }
bool leftToRight(const LineSegment &line) { return line.start.x() < line.end.x(); }
bool rightToLeft(const LineSegment &line) { return !leftToRight(line); }
bool bottomToTop(const LineSegment &line) { return line.start.y() < line.end.y(); }
bool topToBottom(const LineSegment &line) { return !bottomToTop(line); }
bool checkConsistency2(const FieldState &field) {
    bool consistent = true;
    consistent &= field.getTopLine().start.y() == field.getFieldWidth() * 0.5;
    consistent &= field.getTopLine().start.x() == -0.5 * field.getFieldLength();
    consistent &= field.getTopLine().end.x() == 0.5 * field.getFieldLength();
    consistent &= lineHorizontal(field.getTopLine());
    consistent &= leftToRight(field.getTopLine());

    consistent &= field.getBottomLine().start.y() == field.getFieldWidth() * -0.5;
    consistent &= field.getBottomLine().start.x() == 0.5 * field.getFieldLength();
    consistent &= field.getBottomLine().end.x() == -0.5 * field.getFieldLength();
    consistent &= lineHorizontal(field.getBottomLine());
    consistent &= rightToLeft(field.getBottomLine());

    consistent &= field.getLeftLine().start.x() == -field.getFieldLength() * 0.5;
    consistent &= field.getLeftLine().start.y() == -0.5 * field.getFieldWidth();
    consistent &= field.getLeftLine().end.y() == 0.5 * field.getFieldWidth();
    consistent &= lineVertical(field.getLeftLine());
    consistent &= bottomToTop(field.getLeftLine());

    consistent &= field.getRightLine().start.x() == field.getFieldLength() * 0.5;
    consistent &= field.getRightLine().start.y() == 0.5 * field.getFieldWidth();
    consistent &= field.getRightLine().end.y() == -0.5 * field.getFieldWidth();
    consistent &= lineVertical(field.getRightLine());
    consistent &= topToBottom(field.getRightLine());
    return consistent;
}
// Floating point errors from float->double conversions
bool near(double x, double y) { return abs(x - y) < 1e-12; }
bool checkConsistency3(const FieldState &field) {
    bool consistent = true;
    consistent &= near(field.getLeftPenaltyLine().start.x(), -field.getFieldLength() * 0.5 + field.getGoalWidth());
    consistent &= near(field.getLeftPenaltyLine().start.y(), -field.getGoalWidth());
    consistent &= near(field.getLeftPenaltyLine().end.y(), field.getGoalWidth());
    consistent &= lineVertical(field.getLeftPenaltyLine());
    consistent &= bottomToTop(field.getLeftPenaltyLine());

    consistent &= near(field.getRightPenaltyLine().start.x(), field.getFieldLength() * 0.5 - field.getGoalWidth());
    consistent &= near(field.getRightPenaltyLine().start.y(), field.getGoalWidth());
    consistent &= near(field.getRightPenaltyLine().end.y(), -field.getGoalWidth());
    consistent &= lineVertical(field.getRightPenaltyLine());
    consistent &= topToBottom(field.getRightPenaltyLine());
    return consistent;
}
bool checkOtherLines(const FieldState &field) {
    bool consistent = true;
    consistent &= near(field.getHalfLine().start.x(), 0);
    consistent &= near(field.getHalfLine().start.y(), field.getBottomY());
    consistent &= near(field.getHalfLine().end.y(), field.getTopY());
    consistent &= lineVertical(field.getHalfLine());
    consistent &= bottomToTop(field.getHalfLine());

    consistent &= near(field.getCenterLine().start.y(), 0);
    consistent &= near(field.getCenterLine().start.x(), field.getLeftX());
    consistent &= near(field.getCenterLine().end.x(), field.getRightX());
    consistent &= lineHorizontal(field.getCenterLine());
    consistent &= leftToRight(field.getCenterLine());

    consistent &= near(field.getBottomLeftPenaltyStretch().start.x(), field.getLeftX());
    consistent &= near(field.getBottomLeftPenaltyStretch().end.x(), field.getLeftX() + field.getGoalWidth());
    consistent &= lineHorizontal(field.getBottomLeftPenaltyStretch());
    consistent &= near(field.getBottomLeftPenaltyStretch().start.y(), -field.getGoalWidth());
    consistent &= leftToRight(field.getBottomLeftPenaltyStretch());

    consistent &= near(field.getTopLeftPenaltyStretch().start.x(), field.getLeftX());
    consistent &= near(field.getTopLeftPenaltyStretch().end.x(), field.getLeftX() + field.getGoalWidth());
    consistent &= lineHorizontal(field.getTopLeftPenaltyStretch());
    consistent &= near(field.getTopLeftPenaltyStretch().start.y(), field.getGoalWidth());
    consistent &= leftToRight(field.getTopLeftPenaltyStretch());

    consistent &= near(field.getBottomRightPenaltyStretch().start.x(), field.getRightX());
    consistent &= near(field.getBottomRightPenaltyStretch().end.x(), field.getRightX() - field.getGoalWidth());
    consistent &= lineHorizontal(field.getBottomRightPenaltyStretch());
    consistent &= near(field.getBottomRightPenaltyStretch().start.y(), -field.getGoalWidth());
    consistent &= rightToLeft(field.getBottomRightPenaltyStretch());

    consistent &= near(field.getTopRightPenaltyStretch().start.x(), field.getRightX());
    consistent &= near(field.getTopRightPenaltyStretch().end.x(), field.getRightX() - field.getGoalWidth());
    consistent &= lineHorizontal(field.getTopRightPenaltyStretch());
    consistent &= near(field.getTopRightPenaltyStretch().start.y(), field.getGoalWidth());
    consistent &= rightToLeft(field.getTopRightPenaltyStretch());

    return consistent;
}
bool checkGoals(const FieldState &field) {
    bool consistent = true;
    consistent &= near(field.getLeftGoal().start.x(), field.getLeftX());
    consistent &= near(field.getLeftGoal().start.y(), -0.5 * field.getGoalWidth());
    consistent &= near(field.getLeftGoal().end.y(), 0.5 * field.getGoalWidth());
    consistent &= lineVertical(field.getLeftGoal());
    consistent &= field.getOurGoal().start.x() == field.getLeftGoal().start.x();
    consistent &= field.getLeftGoal().start.x() == field.getGoal(true).start.x();

    consistent &= near(field.getRightGoal().start.x(), field.getRightX());
    consistent &= near(field.getRightGoal().start.y(), 0.5 * field.getGoalWidth());
    consistent &= near(field.getRightGoal().end.y(), -0.5 * field.getGoalWidth());
    consistent &= lineVertical(field.getRightGoal());
    consistent &= field.getTheirGoal().start.x() == field.getRightGoal().start.x();
    consistent &= field.getRightGoal().start.x() == field.getGoal(false).start.x();

    consistent &= near(field.getLeftGoalCenter().x(), field.getLeftGoal().center().x());
    consistent &= near(field.getLeftGoalCenter().y(), field.getLeftGoal().center().y());
    consistent &= field.getOurGoalCenter().x() == field.getLeftGoalCenter().x();
    consistent &= field.getOurGoalCenter().x() == field.getGoalCenter(true).x();

    consistent &= near(field.getRightGoalCenter().x(), field.getRightGoal().center().x());
    consistent &= near(field.getRightGoalCenter().y(), field.getRightGoal().center().y());
    consistent &= field.getTheirGoalCenter().x() == field.getRightGoalCenter().x();
    consistent &= field.getTheirGoalCenter().x() == field.getGoalCenter(false).x();

    return consistent;
}
bool checkShapes(const FieldState &field) {
    bool consistent = true;
    consistent &= near(field.getOurDefenceArea().minX(), field.getLeftX());
    consistent &= near(field.getOurDefenceArea().maxX(), field.getLeftX() + field.getGoalWidth());
    consistent &= near(field.getOurDefenceArea().minY(), -field.getGoalWidth());
    consistent &= near(field.getOurDefenceArea().maxY(), field.getGoalWidth());

    consistent &= near(field.getTheirDefenceArea().minX(), field.getRightX() - field.getGoalWidth());
    consistent &= near(field.getTheirDefenceArea().maxX(), field.getRightX());
    consistent &= near(field.getTheirDefenceArea().minY(), -field.getGoalWidth());
    consistent &= near(field.getTheirDefenceArea().maxY(), field.getGoalWidth());

    consistent &= near(field.getOurDefenceArea().minX(), field.getDefenceArea(true).minX());
    consistent &= near(field.getTheirDefenceArea().minX(), field.getDefenceArea(false).minX());

    consistent &= near(field.getField().minX(), field.getLeftX());
    consistent &= near(field.getField().maxX(), field.getRightX());
    consistent &= near(field.getField().minY(), field.getBottomY());
    consistent &= near(field.getField().maxY(), field.getTopY());

    consistent &= near(field.getMarginField().minX(), field.getLeftMarginX());
    consistent &= near(field.getMarginField().maxX(), field.getRightMarginX());
    consistent &= near(field.getMarginField().minY(), field.getBottomMarginY());
    consistent &= near(field.getMarginField().maxY(), field.getTopMarginY());

    consistent &= near(field.getMarginField().minX(), field.getField(true).minX());
    consistent &= near(field.getField().minX(), field.getField(false).minX());

    consistent &= near(field.getOurGoalRectangle().minX(), field.getLeftX() - field.getGoalDepth());
    consistent &= near(field.getOurGoalRectangle().maxX(), field.getLeftX());
    consistent &= near(field.getOurGoalRectangle().minY(), -field.getGoalWidth() * 0.5);
    consistent &= near(field.getOurGoalRectangle().maxY(), field.getGoalWidth() * 0.5);

    consistent &= near(field.getTheirGoalRectangle().minX(), field.getRightX());
    consistent &= near(field.getTheirGoalRectangle().maxX(), field.getRightX() + field.getGoalDepth());
    consistent &= near(field.getTheirGoalRectangle().minY(), -field.getGoalWidth() * 0.5);
    consistent &= near(field.getTheirGoalRectangle().maxY(), field.getGoalWidth() * 0.5);

    consistent &= field.getGoalRectangle(true).minX() == field.getOurGoalRectangle().minX();
    consistent &= field.getGoalRectangle(false).minX() == field.getTheirGoalRectangle().minX();

    consistent &= near(field.getCenterCircle().radius, 0.5 + field.getLineThickness());
    consistent &= near(field.getCenterCircle().center.x(), 0);
    consistent &= near(field.getCenterCircle().center.y(), 0);
    return consistent;
}
TEST(FieldState, divisionATest) {
    FieldState aField(DivisionAField());
    FieldState aFieldSwapped(DivisionAFieldSwapped());
    FieldState defaultAField(DefaultField::DivisionA);
    FieldState invalidEnum((DefaultField)9);

    std::vector<FieldState> fields{aField, aFieldSwapped, defaultAField, invalidEnum};
    // Remove some random lines and sometimes also the center arc
    int numLines = 12;
    for (int i = 0; i < numLines; ++i) {
        for (int j = i; j < numLines; ++j) {
            proto::SSL_GeometryFieldSize sslField = DivisionAField();
            sslField.mutable_field_lines()->DeleteSubrange(i, j - i);
            if (j == 3) {
                sslField.mutable_field_arcs()->DeleteSubrange(0, 1);
            }
            FieldState sample(sslField);
            fields.push_back(sample);
        }
    }
    for (const auto &field : fields) {
        EXPECT_DOUBLE_EQ(field.getFieldLength(), 12.0);
        EXPECT_DOUBLE_EQ(field.getFieldWidth(), 9.0);
        EXPECT_DOUBLE_EQ(field.getBoundaryWidth(), 0.3);
        EXPECT_DOUBLE_EQ(field.getGoalDepth(), 0.18);
        EXPECT_DOUBLE_EQ(field.getGoalWidth(), 1.2);
        EXPECT_DOUBLE_EQ(field.getGoalWallThickness(), 0.02);
        EXPECT_DOUBLE_EQ(field.getGoalHeight(), 0.16);
        EXPECT_DOUBLE_EQ(field.getLineThickness(), 0.01);
        EXPECT_TRUE(checkConsistency(field));
        EXPECT_TRUE(checkConsistency2(field));
        EXPECT_TRUE(checkConsistency3(field));
        EXPECT_TRUE(checkShapes(field));
        EXPECT_TRUE(checkOtherLines(field));
        EXPECT_TRUE(checkGoals(field));
    }
}

TEST(FieldState, otherDefaultFields) {
    FieldState field(DefaultField::DivisionB);
    EXPECT_DOUBLE_EQ(field.getFieldLength(), 9.0);
    EXPECT_DOUBLE_EQ(field.getFieldWidth(), 6.0);
    EXPECT_DOUBLE_EQ(field.getBoundaryWidth(), 0.3);
    EXPECT_DOUBLE_EQ(field.getGoalDepth(), 0.18);
    EXPECT_DOUBLE_EQ(field.getGoalWidth(), 1.0);
    EXPECT_DOUBLE_EQ(field.getGoalWallThickness(), 0.02);
    EXPECT_DOUBLE_EQ(field.getGoalHeight(), 0.16);
    EXPECT_DOUBLE_EQ(field.getLineThickness(), 0.01);
    EXPECT_TRUE(checkConsistency(field));
    EXPECT_TRUE(checkConsistency2(field));
    EXPECT_TRUE(checkConsistency3(field));
    EXPECT_TRUE(checkShapes(field));
    EXPECT_TRUE(checkOtherLines(field));
    EXPECT_TRUE(checkGoals(field));

    field = FieldState(DefaultField::DemoField);  // TODO: fix after measurements
    EXPECT_DOUBLE_EQ(field.getFieldLength(), 1.8);
    EXPECT_DOUBLE_EQ(field.getFieldWidth(), 1.2);
    EXPECT_DOUBLE_EQ(field.getBoundaryWidth(), 0.0);
    EXPECT_DOUBLE_EQ(field.getGoalDepth(), 0.01);
    EXPECT_DOUBLE_EQ(field.getGoalWidth(), 0.6);
    EXPECT_DOUBLE_EQ(field.getGoalWallThickness(), 0.02);
    EXPECT_DOUBLE_EQ(field.getGoalHeight(), 0.16);
    EXPECT_DOUBLE_EQ(field.getLineThickness(), 0.01);
    EXPECT_TRUE(checkConsistency(field));
    EXPECT_TRUE(checkConsistency2(field));
    EXPECT_TRUE(checkConsistency3(field));
    EXPECT_TRUE(checkShapes(field));
    EXPECT_TRUE(checkOtherLines(field));
    EXPECT_TRUE(checkGoals(field));
}