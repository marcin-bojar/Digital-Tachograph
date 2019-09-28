#ifndef DRIVERCARD_H
#define DRIVERCARD_H
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "time.h"


class DriverCard
{
	std::string name;
	std::string surname;
	Time drive_t;
	Time break_t;
	Time dev_break_t1;
	Time dev_break_t2;
	Time day_drive_t;
	Time w_drive_t;
	Time lw_drive_t;
	Time tw_drive_t;
	Time day_break_t;
	Time day_work_t;
	Time day_poa_t;
	Time poa_t;
	time_t work_start_t;
	time_t work_end_t;
	time_t break_start_t;
	time_t break_end_t;
	time_t last_in;

public:
	DriverCard(std::string n="", std::string s="");
	~DriverCard();
	friend class Tacho;
};

#endif // !DRIVERCARD_H
