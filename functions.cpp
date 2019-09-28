#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "time.h"
#include "functions.h"



void showmenu()
{
	std::cout << "What type of activity will be next?\n";
	std::cout << "Choose from 1 to 9:\n";
	std::cout << "1) Driving\n";
	std::cout << "2) Working\n";
	std::cout << "3) Resting\n";
	std::cout << "4) Period of availability\n";
	std::cout << "5) Start your shift\n";
	std::cout << "6) End your shift\n";
	std::cout << "7) Insert driver's card\n";
	std::cout << "8) Eject driver's card\n";
	std::cout << "9) Quit\n";

}

Time entertime()
{
	int h;
	int min;
	std::cout << "How long did it take?\n";
	std::cout << "Hours: ";
	while (!(std::cin >> h) || h < 0)
	{
		std::cout << "Enter number equal or higher than 0!\n";
		std::cin.clear();
		while (std::cin.get() != '\n')
			continue;
	}
	while (std::cin.get() != '\n')
		continue;
	std::cout << "Minutes: \n";
	while (!(std::cin >> min) || min < 0 || min > 59)
	{
		std::cout << "Enter number from 0 to 59!\n";
		std::cin.clear();
		while (std::cin.get() != '\n')
			continue;
	}
	while (std::cin.get() != '\n')
		continue;

	return Time(h, min);
}

bool is_file(const std::string & fn)
{
	std::ifstream test(fn);
	return test.is_open();
}

