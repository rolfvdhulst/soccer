//
// Created by rolf on 07-03-20.
//

#include "FieldState.h"

const Rectangle &FieldState::getOurDefenceArea() const { return leftDefenceArea; }
const Rectangle &FieldState::getTheirDefenceArea() const { return rightDefenceArea; }
const Rectangle &FieldState::getDefenceArea(bool us) const { return us ? leftDefenceArea : rightDefenceArea; }
const Rectangle &FieldState::getField() const { return field; }
const Rectangle &FieldState::getMarginField() const { return fieldWithMargin; }
const Rectangle &FieldState::getField(bool withMargin) const { return withMargin ? fieldWithMargin : field; }
const Rectangle &FieldState::getOurGoalRectangle() const { return leftGoalRectangle; }
const Rectangle &FieldState::getTheirGoalRectangle() const { return rightGoalRectangle; }
const Rectangle &FieldState::getGoalRectangle(bool us) const { return us ? leftGoalRectangle : rightGoalRectangle; }
const Circle &FieldState::getCenterCircle() const { return centerCircle; }
double FieldState::getFieldLength() const { return fieldLength; }
double FieldState::getFieldWidth() const { return fieldWidth; }
double FieldState::getGoalWidth() const { return goalWidth; }
double FieldState::getGoalDepth() const { return goalDepth; }
double FieldState::getGoalHeight() const { return goalHeight; }
double FieldState::getGoalWallThickness() const { return wallThickness; }
double FieldState::getLineThickness() const { return lineThickness; }
double FieldState::getBoundaryWidth() const { return boundaryWidth; }
double FieldState::getLeftX() const { return leftX; }
double FieldState::getRightX() const { return rightX; }
double FieldState::getTopY() const { return topY; }
double FieldState::getBottomY() const { return bottomY; }
double FieldState::getLeftMarginX() const { return leftMarginX; }
double FieldState::getRightMarginX() const { return rightMarginX; }
double FieldState::getTopMarginY() const { return topMarginY; }
double FieldState::getBottomMarginY() const { return bottomMarginY; }
const LineSegment &FieldState::getTopLine() const { return topLine; }
const LineSegment &FieldState::getLeftLine() const { return leftLine; }
const LineSegment &FieldState::getBottomLine() const { return bottomLine; }
const LineSegment &FieldState::getRightLine() const { return rightLine; }
const LineSegment &FieldState::getHalfLine() const { return halfLine; }
const LineSegment &FieldState::getCenterLine() const { return centerLine; }
const LineSegment &FieldState::getLeftPenaltyLine() const { return leftPenaltyLine; }
const LineSegment &FieldState::getRightPenaltyLine() const { return rightPenaltyLine; }
const LineSegment &FieldState::getTopLeftPenaltyStretch() const { return topLeftPenaltyStretch; }
const LineSegment &FieldState::getBottomLeftPenaltyStretch() const { return bottomLeftPenaltyStretch; }
const LineSegment &FieldState::getBottomRightPenaltyStretch() const { return bottomRightPenaltyStretch; }
const LineSegment &FieldState::getTopRightPenaltyStretch() const { return topRightPenaltyStretch; }
const LineSegment &FieldState::getLeftGoal() const { return leftGoal; }
const LineSegment &FieldState::getOurGoal() const { return leftGoal; }
const LineSegment &FieldState::getRightGoal() const { return rightGoal; }
const LineSegment &FieldState::getTheirGoal() const { return rightGoal; }
const LineSegment &FieldState::getGoal(bool us) const { return us ? leftGoal : rightGoal; }
const Vector2 &FieldState::getLeftGoalCenter() const { return leftGoalCenter; }
const Vector2 &FieldState::getOurGoalCenter() const { return leftGoalCenter; }
const Vector2 &FieldState::getRightGoalCenter() const { return rightGoalCenter; }
const Vector2 &FieldState::getTheirGoalCenter() const { return rightGoalCenter; }
const Vector2 &FieldState::getGoalCenter(bool us) const { return us ? leftGoalCenter : rightGoalCenter; }
namespace {
const std::string TopLineName = "TopTouchLine";
const std::string BottomLineName = "BottomTouchLine";
const std::string LeftLineName = "LeftGoalLine";
const std::string RightLineName = "RightGoalLine";
const std::string HalfLineName = "HalfwayLine";
const std::string CenterLineName = "CenterLine";
const std::string LeftPenaltyLineName = "LeftPenaltyStretch";
const std::string RightPenaltyLineName = "RightPenaltyStretch";
const std::string LeftLeftPenaltyStretchName = "LeftFieldLeftPenaltyStretch";
const std::string LeftRightPenaltyStretchName = "LeftFieldRightPenaltyStretch";
const std::string RightLeftPenaltyStretchName = "RightFieldLeftPenaltyStretch";
const std::string RightRightPenaltyStretchName = "RightFieldRightPenaltyStretch";
const std::string CenterCircleName = "CenterCircle";
}  // namespace
FieldState::FieldState(DefaultField fieldType)
    : fieldLength{getFieldLength(fieldType)},
      fieldWidth{getFieldWidth(fieldType)},
      goalDepth{getGoalDepth(fieldType)},
      goalWidth{getGoalWidth(fieldType)},
      boundaryWidth{getBoundaryWidth(fieldType)},
      leftX{-0.5 * fieldLength},
      rightX{0.5 * fieldLength},
      bottomY{-0.5 * fieldWidth},
      topY{0.5 * fieldWidth},
      leftMarginX{-0.5 * fieldLength - boundaryWidth},
      rightMarginX{0.5 * fieldLength + boundaryWidth},
      bottomMarginY{-0.5 * fieldWidth - boundaryWidth},
      topMarginY{0.5 * fieldWidth + boundaryWidth},
      goalHeight{0.16},  // As defined in the rules, https://robocup-ssl.github.io/ssl-rules/sslrules.html#_field_setup see here.
      wallThickness{0.02},
      lineThickness{getLineThickness(fieldType)} {
    makeOutsideLines();
    fixPenaltyFieldLines(false, false, fieldType);
    makeOtherLines(false, false, false, false, false, false);
    centerCircle = Circle(Vector2(0, 0), 0.5 + lineThickness);  // Default for both A and B league divisions

    defineDefenceRectangles();
    defineFieldRectangles();
    defineGoals();
}
FieldState::FieldState(const proto::SSL_GeometryFieldSize &sslGeometry, DefaultField backupField)
    : fieldLength{mmToM(sslGeometry.field_length())},
      fieldWidth{mmToM(sslGeometry.field_width())},
      goalDepth{mmToM(sslGeometry.goal_depth())},
      goalWidth{mmToM(sslGeometry.goal_width())},
      boundaryWidth{mmToM(sslGeometry.boundary_width())},
      leftX{-0.5 * fieldLength},
      rightX{0.5 * fieldLength},
      bottomY{-0.5 * fieldWidth},
      topY{0.5 * fieldWidth},
      leftMarginX{-0.5 * fieldLength - boundaryWidth},
      rightMarginX{0.5 * fieldLength + boundaryWidth},
      bottomMarginY{-0.5 * fieldWidth - boundaryWidth},
      topMarginY{0.5 * fieldWidth + boundaryWidth},
      goalHeight{0.16},  // As defined in the rules, https://robocup-ssl.github.io/ssl-rules/sslrules.html#_field_setup see here.
      wallThickness{0.02} {
    // The order of the operations in these is crucial! Changing it will very likely result in bugs.
    // Some of the functions rely on other things setting things correctly.
    lineThickness = getLineThickness(sslGeometry);
    setCoreLines(sslGeometry, backupField);
    setOtherLines(sslGeometry);
    setCenterCircle(sslGeometry);

    defineDefenceRectangles();
    defineFieldRectangles();
    defineGoals();
}

void FieldState::setCoreLines(const proto::SSL_GeometryFieldSize &sslGeometry, const DefaultField &backupField) {
    bool topLineSet = false;
    bool leftLineSet = false;
    bool bottomLineSet = false;
    bool rightLineSet = false;
    bool leftPenaltyLineSet = false;
    bool rightPenaltyLineSet = false;

    for (const auto &fieldLine : sslGeometry.field_lines()) {
        if (fieldLine.name() == TopLineName) {
            topLine = LineSegment(Vector2(fieldLine.p1()), Vector2(fieldLine.p2()));
            mmToM(topLine);
            if (fieldLine.p1().x() > fieldLine.p2().x()) {
                topLine.reverse();
            }
            topLineSet = true;
        } else if (fieldLine.name() == BottomLineName) {
            bottomLine = LineSegment(Vector2(fieldLine.p1()), Vector2(fieldLine.p2()));
            mmToM(bottomLine);
            if (fieldLine.p1().x() < fieldLine.p2().x()) {
                bottomLine.reverse();
            }
            bottomLineSet = true;
        } else if (fieldLine.name() == LeftLineName) {
            leftLine = LineSegment(Vector2(fieldLine.p1()), Vector2(fieldLine.p2()));
            mmToM(leftLine);
            if (fieldLine.p1().y() > fieldLine.p2().y()) {
                leftLine.reverse();
            }
            leftLineSet = true;
        } else if (fieldLine.name() == RightLineName) {
            rightLine = LineSegment(Vector2(fieldLine.p1()), Vector2(fieldLine.p2()));
            mmToM(rightLine);
            if (fieldLine.p1().y() < fieldLine.p2().y()) {
                rightLine.reverse();
            }
            rightLineSet = true;
        } else if (fieldLine.name() == LeftPenaltyLineName) {
            leftPenaltyLine = LineSegment(Vector2(fieldLine.p1()), Vector2(fieldLine.p2()));
            mmToM(leftPenaltyLine);
            if (fieldLine.p1().y() > fieldLine.p2().y()) {
                leftPenaltyLine.reverse();
            }
            leftPenaltyLineSet = true;
        } else if (fieldLine.name() == RightPenaltyLineName) {
            rightPenaltyLine = LineSegment(Vector2(fieldLine.p1()), Vector2(fieldLine.p2()));
            mmToM(rightPenaltyLine);
            if (fieldLine.p1().y() < fieldLine.p2().y()) {
                rightPenaltyLine.reverse();
            }
            rightPenaltyLineSet = true;
        }
    }
    fixOutsideFieldLines(topLineSet, leftLineSet, bottomLineSet, rightLineSet);
    fixPenaltyFieldLines(leftPenaltyLineSet, rightPenaltyLineSet, backupField);
}
double FieldState::mmToM(double value) { return value / 1000.0; }
Vector2 FieldState::mmToM(const Vector2 &value) { return value * 0.001; }
void FieldState::mmToM(LineSegment &line) {
    line.start() = mmToM(line.start());
    line.end() = mmToM(line.end());
}
bool FieldState::threeTrue(bool a, bool b, bool c, bool d) {
    int count = 0;
    if (a) {
        count++;
    }
    if (b) {
        count++;
    }
    if (c) {
        count++;
    }
    if (d) {
        count++;
    }
    return count == 3;
}
void FieldState::fixOutsideFieldLines(bool topLineSet, bool leftLineSet, bool bottomLineSet, bool rightLineSet) {
    bool allValid = topLineSet && leftLineSet && bottomLineSet && rightLineSet;
    if (allValid) {
        return;
    }
    // At least one line is missing.
    // If only one line is missing we add it ourselves.
    // Else we fall back on the fieldwidth and length values
    if (threeTrue(topLineSet, leftLineSet, bottomLineSet, rightLineSet)) {
        fixOneOutsideLineMissing(topLineSet, leftLineSet, bottomLineSet, rightLineSet);
    } else {
        makeOutsideLines();
    }
}
void FieldState::fixOneOutsideLineMissing(bool topLineSet, bool leftLineSet, bool bottomLineSet, bool rightLineSet) {
    if (!topLineSet) {
        topLine = LineSegment(leftLine.end(), rightLine.start());
    } else if (!leftLineSet) {
        leftLine = LineSegment(bottomLine.end(), topLine.start());
    } else if (!bottomLineSet) {
        bottomLine = LineSegment(rightLine.end(), leftLine.start());
    } else if (!rightLineSet) {
        rightLine = LineSegment(topLine.end(), bottomLine.start());
    }
}
void FieldState::makeOutsideLines() {
    Vector2 bottomLeft(leftX, bottomY);
    Vector2 topLeft(leftX, topY);
    Vector2 topRight(rightX, topY);
    Vector2 bottomRight(rightX, bottomY);
    topLine = LineSegment(topLeft, topRight);
    leftLine = LineSegment(bottomLeft, topLeft);
    bottomLine = LineSegment(bottomRight, bottomLeft);
    rightLine = LineSegment(topRight, bottomRight);
}
void FieldState::fixPenaltyFieldLines(bool leftPenaltyLineSet, bool rightPenaltyLineSet, DefaultField backupField) {
    bool bothTrue = leftPenaltyLineSet && rightPenaltyLineSet;
    if (bothTrue) {
        return;
    }
    double penaltySize = getGoalWidth(backupField);
    // If one of the penalty lines was set we override it and assume symmetry
    if (leftPenaltyLineSet) {
        penaltySize = abs(leftPenaltyLine.start().x() - leftX);
    } else if (rightPenaltyLineSet) {
        penaltySize = abs(rightX - rightPenaltyLine.start().x());
    }
    if (!leftPenaltyLineSet) {
        Vector2 top(leftX + penaltySize, penaltySize);
        Vector2 bottom(leftX + penaltySize, -penaltySize);
        leftPenaltyLine = LineSegment(bottom, top);
    }
    if (!rightPenaltyLineSet) {
        Vector2 top(rightX - penaltySize, penaltySize);
        Vector2 bottom(rightX - penaltySize, -penaltySize);
        rightPenaltyLine = LineSegment(top, bottom);
    }
}
double FieldState::getLineThickness(const proto::SSL_GeometryFieldSize &sslGeometry) {
    // Get the thickness which occurs most often
    std::map<float, int> thicknesses;
    for (const auto &fieldLine : sslGeometry.field_lines()) {
        float thickness = fieldLine.thickness();
        auto element = thicknesses.find(thickness);
        if (element != thicknesses.end()) {
            ++thicknesses[fieldLine.thickness()];
        } else {
            thicknesses.insert(std::pair<float, int>(thickness, 0));
        }
    }
    auto iterator = std::max_element(thicknesses.begin(), thicknesses.end(),
                                     [](const std::map<float, int>::value_type &p1, const std::map<float, int>::value_type &p2) { return p1.second < p2.second; });
    return mmToM(iterator->first);
}
void FieldState::setOtherLines(const proto::SSL_GeometryFieldSize &sslGeometry) {
    bool halfLineSet = false;
    bool centerLineSet = false;

    bool bottomLeftPenaltyStretchSet = false;
    bool topLeftPenaltyStretchSet = false;
    bool bottomRightPenaltyStretchSet = false;
    bool topRightPenaltyStretchSet = false;

    for (const auto &fieldLine : sslGeometry.field_lines()) {
        if (fieldLine.name() == HalfLineName) {
            halfLine = LineSegment(Vector2(fieldLine.p1()), Vector2(fieldLine.p2()));
            mmToM(halfLine);
            if (fieldLine.p1().y() > fieldLine.p2().y()) {
                halfLine.reverse();
            }
            halfLineSet = true;
        } else if (fieldLine.name() == CenterLineName) {
            centerLine = LineSegment(Vector2(fieldLine.p1()), Vector2(fieldLine.p2()));
            mmToM(centerLine);
            if (fieldLine.p1().x() > fieldLine.p2().x()) {
                centerLine.reverse();
            }
            centerLineSet = true;
        } else if (fieldLine.name() == LeftLeftPenaltyStretchName || fieldLine.name() == LeftRightPenaltyStretchName) {
            LineSegment line(Vector2(fieldLine.p1()), Vector2(fieldLine.p2()));
            mmToM(line);
            if (fieldLine.p1().x() > fieldLine.p2().x()) {
                line.reverse();
            }
            if (fieldLine.p1().y() > 0) {
                topLeftPenaltyStretch = line;
                topLeftPenaltyStretchSet = true;
            } else {
                bottomLeftPenaltyStretch = line;
                bottomLeftPenaltyStretchSet = true;
            }
        } else if (fieldLine.name() == RightLeftPenaltyStretchName || fieldLine.name() == RightRightPenaltyStretchName) {
            LineSegment line(Vector2(fieldLine.p1()), Vector2(fieldLine.p2()));
            mmToM(line);
            if (fieldLine.p1().x() < fieldLine.p2().x()) {
                line.reverse();
            }
            if (fieldLine.p1().y() > 0) {
                topRightPenaltyStretch = line;
                topRightPenaltyStretchSet = true;
            } else {
                bottomRightPenaltyStretch = line;
                bottomRightPenaltyStretchSet = true;
            }
        }
    }
    makeOtherLines(halfLineSet, centerLineSet, bottomLeftPenaltyStretchSet, topLeftPenaltyStretchSet, bottomRightPenaltyStretchSet, topRightPenaltyStretchSet);
}
void FieldState::makeOtherLines(bool halfLineSet, bool centerLineSet, bool bottomLeftPenaltyStretchSet, bool topLeftPenaltyStretchSet, bool bottomRightPenaltyStretchSet,
                                bool topRightPenaltyStretchSet) {
    Vector2 topCenter = topLine.center();
    Vector2 bottomCenter = bottomLine.center();
    Vector2 leftCenter = leftLine.center();
    Vector2 rightCenter = rightLine.center();
    if (!halfLineSet) {
        halfLine = LineSegment(bottomCenter, topCenter);
    }
    if (!centerLineSet) {
        centerLine = LineSegment(leftCenter, rightCenter);
    }

    if (!bottomLeftPenaltyStretchSet) {
        Vector2 leftBottomLeft(leftX, leftPenaltyLine.start().y());
        Vector2 leftBottomRight = leftPenaltyLine.start();
        bottomLeftPenaltyStretch = LineSegment(leftBottomLeft, leftBottomRight);
    }
    if (!topLeftPenaltyStretchSet) {
        Vector2 leftTopLeft(leftX, leftPenaltyLine.end().y());
        Vector2 leftTopRight = leftPenaltyLine.end();
        topLeftPenaltyStretch = LineSegment(leftTopLeft, leftTopRight);
    }
    if (!bottomRightPenaltyStretchSet) {
        Vector2 rightBottomRight(rightX, rightPenaltyLine.end().y());
        Vector2 rightBottomLeft = rightPenaltyLine.end();
        bottomRightPenaltyStretch = LineSegment(rightBottomRight, rightBottomLeft);
    }
    if (!topRightPenaltyStretchSet) {
        Vector2 rightTopRight(rightX, rightPenaltyLine.start().y());
        Vector2 rightTopLeft = rightPenaltyLine.start();
        topRightPenaltyStretch = LineSegment(rightTopRight, rightTopLeft);
    }
}
void FieldState::setCenterCircle(const proto::SSL_GeometryFieldSize &sslGeometry) {
    bool centerCircleSet = false;
    for (const auto &fieldArc : sslGeometry.field_arcs()) {
        if (fieldArc.name() == CenterCircleName) {
            Vector2 center(fieldArc.center());
            centerCircle = Circle(center,
                                  mmToM(fieldArc.radius() + fieldArc.thickness()));  // lines are part of the circle.
            centerCircleSet = true;
        }
    }
    if (!centerCircleSet) {
        centerCircle = Circle(Vector2(0, 0), 0.5 + lineThickness);  // Default for both A and B league divisions
    }
}
void FieldState::defineDefenceRectangles() {
    leftDefenceArea = Rectangle(bottomLeftPenaltyStretch.start(), leftPenaltyLine.end());
    rightDefenceArea = Rectangle(rightPenaltyLine.end(), topRightPenaltyStretch.start());
}
void FieldState::defineFieldRectangles() {
    field = Rectangle(Vector2(leftX, bottomY), fieldLength, fieldWidth);
    fieldWithMargin = Rectangle(Vector2(leftMarginX, bottomMarginY), fieldLength + 2 * boundaryWidth, fieldWidth + 2 * boundaryWidth);
}
void FieldState::defineGoals() {
    double halfGoalWidth = 0.5 * goalWidth;
    double x = leftX;  // TODO: add linethickness (are goals ON or behind the lines?)
    leftGoal = LineSegment(Vector2(x, -halfGoalWidth), Vector2(x, halfGoalWidth));
    leftGoalCenter = leftGoal.center();
    leftGoalRectangle = Rectangle(Vector2(x - goalDepth, -halfGoalWidth), Vector2(x, halfGoalWidth));

    x = rightX;
    rightGoal = LineSegment(Vector2(x, halfGoalWidth), Vector2(x, -halfGoalWidth));
    rightGoalCenter = rightGoal.center();
    rightGoalRectangle = Rectangle(Vector2(x, -halfGoalWidth), Vector2(x + goalDepth, halfGoalWidth));
}
double FieldState::getFieldLength(const DefaultField &field) {
    switch (field) {
        case DefaultField::DivisionA:
            return 12.0;
        case DefaultField::DivisionB:
            return 9.0;
        case DefaultField::DemoField:
            return 1.8;  // TODO: measure
        default:
            return 12.0;
    }
}
double FieldState::getFieldWidth(const DefaultField &field) {
    switch (field) {
        case DefaultField::DivisionA:
            return 9.0;
        case DefaultField::DivisionB:
            return 6.0;
        case DefaultField::DemoField:
            return 1.2;  // TODO: measure
        default:
            return 9.0;
    }
}
double FieldState::getGoalDepth(const DefaultField &field) {
    switch (field) {
        case DefaultField::DivisionA:
        case DefaultField::DivisionB:
            return 0.18;
        case DefaultField::DemoField:
            return 0.01;  // TODO: measure
        default:
            return 0.18;
    }
}
double FieldState::getGoalWidth(const DefaultField &field) {
    switch (field) {
        case DefaultField::DivisionA:
            return 1.2;
        case DefaultField::DivisionB:
            return 1.0;
        case DefaultField::DemoField:
            return 0.6;  // TODO: measure
        default:
            return 1.2;
    }
}
double FieldState::getBoundaryWidth(const DefaultField &field) {
    switch (field) {
        case DefaultField::DivisionA:
        case DefaultField::DivisionB:
            return 0.3;
        case DefaultField::DemoField:
            return 0.0;
        default:
            return 0.3;
    }
}
double FieldState::getLineThickness(const DefaultField &field) {
    switch (field) {
        case DefaultField::DivisionA:
        case DefaultField::DivisionB:
            return 0.01;
        case DefaultField::DemoField:
            return 0.01;
        default:
            return 0.01;
    }
}
