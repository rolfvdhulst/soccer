//
// Created by rolf on 23-2-20.
//

#include "Time.h"

Time::Time(std::chrono::nanoseconds time) : timePoint{time} {}
Time Time::operator+(const Time &other) const { return Time(timePoint + other.timePoint); }
Time Time::operator-(const Time &other) const { return Time(timePoint - other.timePoint); }
Time Time::operator-=(const Time &other) { return Time(timePoint -= other.timePoint); }
Time Time::operator+=(const Time &other) { return Time(timePoint += other.timePoint); }
bool Time::operator>(const Time &other) const { return timePoint > other.timePoint; }
bool Time::operator>=(const Time &other) const { return timePoint >= other.timePoint; }
bool Time::operator<=(const Time &other) const { return timePoint <= other.timePoint; }
bool Time::operator<(const Time &other) const { return timePoint < other.timePoint; }
bool Time::operator==(const Time &other) const { return timePoint == other.timePoint; }
bool Time::operator!=(const Time &other) const { return timePoint != other.timePoint; }
double Time::asSeconds() const { return timePoint.count() / 1e9; }
double Time::asMilliSeconds() const { return timePoint.count() / 1e6; }
long Time::asIntegerSeconds() const { return timePoint.count() / 1000000000; }
long Time::asNanoSeconds() const { return timePoint.count(); }
long Time::asMicroSeconds() const { return timePoint.count() / 1000; }
long Time::asIntegerMilliSeconds() const { return timePoint.count() / 1000000; }
Time Time::now() { return Time(std::chrono::system_clock::now().time_since_epoch()); }
Time Time::timeSince() const { return (now() - *this); }
Time Time::timeTo() const { return (*this - now()); }
Time::Time(double seconds) :
timePoint{std::chrono::nanoseconds((long)(seconds*1e9))} {
}
