//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_TIME_H
#define SOCCER_TIME_H

#include <chrono>
#include <ctime>

class Time {
   public:
    Time() : timePoint{std::chrono::nanoseconds(0)} {};
    explicit Time(std::chrono::nanoseconds time);
    explicit Time(double seconds);
    explicit Time(long nanoseconds) : timePoint{std::chrono::nanoseconds(nanoseconds)} {};
    explicit Time(unsigned long nanoseconds) : timePoint{std::chrono::nanoseconds(nanoseconds)} {};
    static Time now();

    [[nodiscard]] Time timeSince() const;
    [[nodiscard]] Time timeTo() const;
    [[nodiscard]] long asNanoSeconds() const;
    [[nodiscard]] long asIntegerSeconds() const;
    [[nodiscard]] long asIntegerMilliSeconds() const;
    [[nodiscard]] long asMicroSeconds() const;
    [[nodiscard]] double asSeconds() const;
    [[nodiscard]] double asMilliSeconds() const;
    Time operator+(const Time &other) const;
    Time operator-(const Time &other) const;
    Time operator-=(const Time &other);
    Time operator+=(const Time &other);
    bool operator>(const Time &other) const;
    bool operator>=(const Time &other) const;
    bool operator<(const Time &other) const;
    bool operator<=(const Time &other) const;
    bool operator==(const Time &other) const;
    bool operator!=(const Time &other) const;

   private:
    std::chrono::nanoseconds timePoint;
};

#endif  // SOCCER_TIME_H
