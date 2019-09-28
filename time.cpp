#include "time.h"

Time::Time(int min)
{
	hours = min / min_per_h;
	min_left = min % min_per_h;
	minutes = min;
}

Time::Time(int h, int min)
{
	hours = h;
	min_left = min;
	minutes = h * min_per_h + min;
}

Time::~Time()
{
}

void Time::show() const
{
	std::cout << hours << " hours, " << min_left << " minutes.\n";
}

Time Time::operator+(const Time & b) const
{
	return Time(minutes + b.minutes);
}

Time Time::operator-(const Time & b) const
{
	return Time(minutes - b.minutes);
}

Time & Time::operator+=(const Time & b)
{
	return *this = Time(minutes + b.minutes);
}

Time & Time::operator-=(const Time & b)
{
	return *this = Time(minutes - b.minutes);
}

Time::operator int() const
{
	return minutes;
}

std::ostream & operator<<(std::ostream & os, const Time & t)
{
	os << t.hours << " hours, " << t.min_left << " minutes.";

	return os;
}

std::istream & operator >>(std::istream & is, Time & t)
{
	is >> t.hours >> t.min_left;

	return is;
}