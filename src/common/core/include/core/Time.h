//
// Created by rolf on 23-2-20.
//

#ifndef SOCCER_TIME_H
#define SOCCER_TIME_H

#include <ctime>
#include <chrono>

class Time {
    public:
        Time(std::chrono::nanoseconds time);
        static Time now();

        Time timeSince() const;
        long asNanoSeconds() const;
        long asIntegerSeconds() const;
        long asIntegerMilliSeconds() const;
        long asMicroSeconds() const;
        double asSeconds() const;
        double asMilliSeconds() const;
        Time operator +(const Time &other) const;
        Time operator -(const Time &other) const;
        Time operator -=(const Time &other);
        Time operator +=(const Time &other);
        bool operator >(const Time &other) const;
        bool operator >=(const Time &other) const;
        bool operator <(const Time &other) const;
        bool operator <=(const Time &other) const;
        bool operator ==(const Time &other) const;
        bool operator !=(const Time &other) const;
    private:
        std::chrono::nanoseconds timePoint;
        
};

#endif //SOCCER_TIME_H
