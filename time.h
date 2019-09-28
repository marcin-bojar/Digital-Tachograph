#ifndef TIME_H
#define TIME_H
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>

class Time
{
	int hours;
	int min_left;
	int minutes;
	enum { min_per_h = 60 };

public:
	Time(int min=0);
	Time(int h, int min);
	~Time();
	void show() const;
	Time operator+(const Time & b) const;
	Time operator-(const Time & b) const;
	Time & operator+=(const Time & b);
	Time & operator-=(const Time & b);
	operator int() const;
	friend std::ostream & operator<<(std::ostream & os, const Time & t);
	friend std::istream & operator>>(std::istream & is, Time & t);
};

#endif // TIME_H