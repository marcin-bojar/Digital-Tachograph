#include "tacho.h"
#include "drivercard.h"
#include "time.h"
#include "functions.h"

Tacho::Tacho()
{
	time(&tacho_virtual_t);
	checkp = tacho_virtual_t;
	std::cout << "Enter license plate of the truck:\n";
	getline(std::cin, plates);
	std::string filename = plates+".txt";
	if (!is_file(filename))
	{
		TACHOinout.open(filename, std::fstream::in | std::fstream::app);
		TACHOinout.rdbuf()->pubsetbuf(0, 0);
		print_anytime(tacho_virtual_t, TACHOinout);
		TACHOinout << "Tachograph installed! License plate: " << plates << std::endl << std::endl;
	}
	else 
	{
		TACHOinout.open(filename, std::fstream::in | std::fstream::app);
		TACHOinout.rdbuf()->pubsetbuf(0, 0);
	}		
		
}

Tacho::~Tacho()
{
	TACHOinout.close();
}

void Tacho::show() const
{
	std::cout << std::endl;
	std::cout << "Time and date: "; show_time();
	std::cout << "\nDrive time: " << drive_t << "\nBreak time: " << break_t;
	std::cout << "\nPresent activity: " << present;
	std::cout << "\nTwo-weeks drive time: " << tw_drive_t << "\n\n";
}

void Tacho::show_time() const
{
	char buffer[40];
	struct tm * timeinfo;
	timeinfo = localtime(&tacho_virtual_t);
	strftime(buffer, 40, "%H:%M %d/%m/%G", timeinfo);
	std::cout << buffer;
}

void Tacho::show_anytime(const time_t & t) const
{
	char buffer[40];
	struct tm * timeinfo;
	timeinfo = localtime(&t);
	strftime(buffer, 40, "%H:%M %d/%m/%G", timeinfo);
	std::cout << buffer;
}

void Tacho::print_anytime(const time_t & t, std::fstream & fs) const
{
	char buffer[40];
	struct tm * timeinfo;
	timeinfo = localtime(&t);
	strftime(buffer, 40, "%H:%M %d/%m/%G ", timeinfo);
	fs << buffer;
}

void Tacho::print_short_time(std::fstream & fs) const
{
	char buffer[10];
	struct tm * timeinfo;
	timeinfo = localtime(&tacho_virtual_t);
	strftime(buffer, 10, "%H:%M", timeinfo);
	fs << buffer;
}

void Tacho::print_date(const time_t & t, std::fstream & fs) const
{
	char buffer[20];
	struct tm *timeinfo;
	timeinfo = localtime(&t);
	strftime(buffer, 20, "%d/%m/%G\n", timeinfo);
	fs << buffer;
}

void Tacho::add_dt(const Time & a)
{
	Time temp = 0;
	int dt = 0;
	CARDinout << "\t\t"; print_short_time(CARDinout);
	TACHOinout << "\t\t"; print_short_time(TACHOinout);
	CARDinout << "\tDT: " << a << std::endl;
	TACHOinout <<  "\tDT: " << a << std::endl;
	tacho_virtual_t += (int)a * 60;						 /* a*60 - time of activity in seconds*/
	for (int i = drive_t; i <= a + drive_t; i++)		/* this loop simulates passing time min after min to see how tachograph displays warnings while driving */
	{
		if (i == 255)
			std::cout << std::endl << Time(i) << "\nDriver! Stop for a break in max 15 minutes!\n";
		if (i == 270)
			std::cout << std::endl << Time(i) << "\nDriver! Stop for a break now!\n";		/* maximal driving time is 4,5 h. after that 45 minutes break is required */
		if (i > 270 && i % 5 == 0)
			std::cout << std::endl << Time(i) << "\nDriver! Stop for a break!\n";			/* repeats warning every 5 minutes */
	}
	for (time_t i = checkp; i <= tacho_virtual_t; i++)
	{
		checkpoint = localtime(&i);
		if (checkpoint->tm_hour == 0 && checkpoint->tm_min == 0 && checkpoint->tm_sec == 0 && checkpoint->tm_wday == 1)		/* if new week starts while driving, time of this driving period should be divided between weeks */
		{
			temp = dt / 60;
			w_drive_t += temp;					/* time of driving until midnight goes to week that's ending */
			check_wday(a, day_drive_t);			/* check_wday() does all necessery operations when week is over (updates tw_drive_t and lw_drive_t, resets w_drive_t and day_drive_t*/
			temp = Time((int)a - dt / 60);		/* rest of drive time goes to new week */
			w_drive_t += temp;
			break;
		}
			
		dt++;		/* counts drive time before midnight in seconds */
	}
	if (temp == 0)
	{
		w_drive_t += a;
		check_wday(a, day_drive_t);
	}	
	drive_t += a;
	tw_drive_t = w_drive_t + lw_drive_t;
	present = a;
	if (break_t >= 45)
		break_t = 0;
	check_shift_t(a);
}

void Tacho::add_bt(const Time & a)
{
	CARDinout << "\t\t"; print_short_time(CARDinout);
	TACHOinout << "\t\t"; print_short_time(TACHOinout);
	CARDinout << "\tBT: " << a << std::endl;
	TACHOinout << "\tBT: " << a << std::endl;

	if (a >= 45)
	{
		break_t = a;
		drive_t = 0;
		present = a;
		tacho_virtual_t += (int)a * 60;
		check_wday(a, day_break_t);

	}
	else if (a >= 45 && dev_break_t1 != 0)
	{
		break_t = dev_break_t1 + a;
		drive_t = 0;
		present = a;
		tacho_virtual_t += (int)a * 60;
		check_wday(a, day_break_t);

	}
	else if (dev_break_t1 == 0 && a < 45 && a >= 15)
	{
		dev_break_t1 = a;
		break_t = dev_break_t1;
		present = dev_break_t1;
		tacho_virtual_t += (int)a * 60;
		check_wday(a, day_break_t);

	}
	else if (dev_break_t1 != 0 && a >= 30)
	{
		dev_break_t2 = a;
		break_t = dev_break_t1 + dev_break_t2;
		drive_t = 0;
		present = dev_break_t2;
		tacho_virtual_t += (int)a * 60;
		check_wday(a, day_break_t);

	}
	else if (dev_break_t1 != 0 && a < 30)
	{
		present = a;
		tacho_virtual_t += (int)a * 60;
		check_wday(a, day_break_t);

	}


	else if (a < 15)
	{
		present = a;
		tacho_virtual_t += (int)a * 60;
		check_wday(a, day_break_t);

	}
	else 
	{
		present = a;
		break_t = break_t;
		tacho_virtual_t += (int)a * 60;
		check_wday(a, day_break_t);

	}

	if (a < 540)					/* 540 minutes == 9 hours - minimal length of rest between two shifts*/
		check_shift_t(a);	
	else							/* prevents from unnecessary warnings even if driver forgets to register beginning of shift */
		work_start_t = tacho_virtual_t;

}

void Tacho::add_wt(const Time & a)
{
	CARDinout << "\t\t"; print_short_time(CARDinout);
	TACHOinout << "\t\t"; print_short_time(TACHOinout);
	CARDinout << "\tWT: " << a << std::endl;
	TACHOinout << "\tWT: " << a << std::endl;
	present = a;
	tacho_virtual_t += (int)a * 60;
	check_shift_t(a);
	if (break_t >= 45)
		break_t = 0;
	check_wday(a, day_work_t);
	

	
}

void Tacho::add_poa(const Time & a)
{
	CARDinout << "\t\t"; print_short_time(CARDinout);
	TACHOinout << "\t\t"; print_short_time(TACHOinout);
	CARDinout << "\tPOA: " << a << std::endl;
	TACHOinout << "\tPOA: " << a << std::endl;
	present = a;
	tacho_virtual_t += (int)a * 60;
	check_shift_t(a);
	if (break_t >= 45)
		break_t = 0;
	check_wday(a, day_poa_t);
	
}

void Tacho::start_work()
{

	 if (difftime(tacho_virtual_t, work_end_t) < 32400 && work_end_t != 0)			/* 32400 seconds == 9 hours - minimal lenght of break between two shifts*/
		{
			std::cout << "Your break time is too short to start new shift! Are you sure? Y/N\n";
			char ch;
			while (!(std::cin >> ch) || (ch != 'Y' && ch != 'y' && ch != 'N' && ch != 'n'))
			{
				std::cout << "Enter Y or N\n"; 
				std::cin.clear();
				while (std::cin.get() != '\n')
					continue;
			}
			if (ch == 'Y' || ch == 'y')
			{
				break_end_t = tacho_virtual_t;
				enter_country();
				se_times.push_back(tacho_virtual_t);
				se_countries.push_back("|-> " + country);
				if (work_start_t != 0 )
					{
					std::cout << "\nYou continue your last shift which began at ";
					show_anytime(work_start_t);
					}				
				std::cout << std::endl;
			}
			else if (ch == 'N' || ch == 'n')
				return;
		}
		else
		{
			work_start_t = break_end_t = tacho_virtual_t;
			enter_country();
			se_times.push_back(tacho_virtual_t);
			se_countries.push_back("|-> " + country);
			std::cout << "\nYour shift began at "; show_time(); std::cout << " in " << country << std::endl;
		}
	
}

void Tacho::end_work()
{
	work_end_t = break_start_t = tacho_virtual_t;
	se_times.push_back(tacho_virtual_t);
	enter_country();
	se_countries.push_back("|<- " + country);
	std::cout << "\nYour shift ended at "; show_time(); std::cout << " in " << country << std::endl;
}

void Tacho::check_shift_t(const Time & a) const
{

	for (int i = tacho_virtual_t - a*60; i <= tacho_virtual_t; i++)			
	{
		if (i == work_start_t + 50400)										/* 50400 secoonds == 14 hours - warning for a driver */
		{
			time_t warning = i;
			show_anytime(warning);
			std::cout << " Last hour of this shift! Search for a parking!\n";
		}

		if (i == work_start_t + 54000)								/* 54000 secoonds == 15 hours - maximal shift length in EU */
		{
			time_t max = i;
			show_anytime(max);
			std::cout << " Your shift time run out. Stop for a rest now!\n";
		}
		
	}
	
}

void Tacho::check_wday(const Time & a, Time & act_t)
{
	int day_t = 0;			/* day_t, total_t and temp are used to divide time of activity between two different days if performed during midnight */
	int total_t = 0;		/* day_t and total_t also helps to calculate times for day summary, i.e. when driver rests for few days and total time of this break must be divided between those days */
	Time temp = 0;
	for (time_t i = checkp; i <= tacho_virtual_t; i++)
	{	
		checkpoint = localtime(&i);
		if (checkpoint->tm_hour == 0 && checkpoint->tm_min == 0 && checkpoint->tm_sec == 0 && checkpoint->tm_wday == 1)
		{
			if (lw_drive_t == 0)
			{
				tw_drive_t = w_drive_t;
				lw_drive_t = w_drive_t;
				w_drive_t = 0;
			}
			else
			{
				tw_drive_t -= lw_drive_t;
				lw_drive_t = w_drive_t;
				w_drive_t = 0;
			}

		}
		if (checkpoint->tm_hour == 23 && checkpoint->tm_min == 59 && checkpoint->tm_sec == 59)
		{
			CARDinout << "\n\t*************************************************\n";
			TACHOinout << "\n\t*************************************************\n";
			print_date(mktime(checkpoint), CARDinout); print_date(mktime(checkpoint), TACHOinout);
			CARDinout << "SUMMARY:\n\n"; TACHOinout << "SUMMARY:\n\n";

		}
		if (checkpoint->tm_hour == 0 && checkpoint->tm_min == 0 && checkpoint->tm_sec == 0)
		{
			temp = day_t / 60;
			act_t += temp;
			for (int j = 0; j < se_countries.size(); j++)
			{
				CARDinout << "\t"; print_anytime(se_times[j], CARDinout);
				CARDinout << "\t" << se_countries[j] << std::endl;
				TACHOinout << "\t"; print_anytime(se_times[j], TACHOinout);
				TACHOinout << "\t" << se_countries[j] << std::endl;
			}
			CARDinout << "\n\t\t Day drive time: " << day_drive_t; TACHOinout << "\n\t\t Day drive time: " << day_drive_t;
			CARDinout << "\n\t\t Day break time: " << day_break_t; TACHOinout << "\n\t\t Day break time: " << day_break_t;
			CARDinout << "\n\t\t Day work time: " << day_work_t; TACHOinout << "\n\t\t Day work time: " << day_work_t;
			CARDinout << "\n\t\t Day POA time: " << day_poa_t; TACHOinout << "\n\t\t Day POA time: " << day_poa_t;
			CARDinout << "\n\t*************************************************\n\n";
			TACHOinout << "\n\t*************************************************\n\n";
			checkpoint = localtime(&i);				// without this line repeated checkpoint is set as last object from se_times[]. why?
			print_date(mktime(checkpoint), CARDinout); print_date(mktime(checkpoint), TACHOinout);
			day_drive_t = 0;
			day_break_t = 0;
			day_work_t = 0;
			day_poa_t = 0;
			se_times.clear();
			se_countries.clear();
			temp = Time((int)a - total_t / 60);
			day_t = 0;	
		}

		total_t++;
		day_t++;

	}
	if (temp == 0)
		act_t += a;
	else
		act_t += temp;
	checkp = tacho_virtual_t;
	
}

void Tacho::enter_country()
{
	int match = 0;
	std::cout << "Enter country (enter ? for list): ";
	do
	{
		std::cin >> country;
		for (int i = 0; i < country.size(); i++)
			country[i] = toupper(country[i]);
		if (country == "?") 
		{
			for (int i = 0; i < 246; i++)
				std::cout << i + 1 << ": " << countries[i] << std::endl;
		}
		for (int i = 0; i < 246; i++)
		{
			if (country == countries[i])
			{
				match++;
				break;
			}
			else if (i == 245 && match == 0 && country != "?")
				std::cout << "Enter correct country! (? for list)\n";
		}
	} while (match == 0);
	
}

void Tacho::insert_card()
{
	std::string filename;
	std::cout << "Enter your first name: ";
	getline(std::cin, driver.name);
	std::cout << "\nEnter your surname: ";
	getline(std::cin, driver.surname);
	filename = driver.name + driver.surname + ".txt";
	CARDinout.open(filename, std::fstream::in | std::fstream::app);
	CARDinout.rdbuf()->pubsetbuf(0, 0);
	if (!CARDinout.is_open())
	{
		std::cout << "Error occured while reading card! Try to insert your card again!\n";
		return;
	}	
	std::cout << "Welcome " << driver.name << " " << driver.surname << "..." << std::endl;
	TACHOinout << std::endl;
	CARDinout << std::endl;
	print_anytime(tacho_virtual_t, TACHOinout);
	print_anytime(tacho_virtual_t, CARDinout);
	TACHOinout << "Driver: " << driver.name << " " << driver.surname << " --> Card inserted!\n\n";
	CARDinout  << plates << " --> Card inserted!\n\n";
}

void Tacho::eject_card()
{
	driver.last_in = tacho_virtual_t;
	std::cout << "Goodbye " << driver.name << " " << driver.surname << "." << std::endl;
	TACHOinout << std::endl;
	CARDinout << std::endl;
	print_anytime(tacho_virtual_t, TACHOinout);
	print_anytime(tacho_virtual_t, CARDinout);
	TACHOinout << "Driver " << driver.name << driver.surname << " --> Card ejected!\n\n";
	CARDinout << plates << " --> Card ejected!\n\n";
	CARDinout.close();
}

void Tacho::use_tacho()
{
	int choice;
	while (!(std::cin >> choice) || choice < 0 || choice > 9)
	{
		std::cout << "Enter correct number!";
		std::cin.clear();
		while (std::cin.get() != '\n')
			continue;
	}
	while (choice)
	{
		while (std::cin.get() != '\n')
			continue;

		switch (choice)
		{
		case 1:
			add_dt(entertime());
			show();
			break;
		case 2:
			add_wt(entertime());
			show();
			break;
		case 3:
			add_bt(entertime());
			show();
			break;
		case 4:
			add_poa(entertime());
			show();
			break;
		case 5:
			start_work();
			show();
			break;
		case 6:
			end_work();
			show();
			break;
		case 7:
			insert_card();
			show();
			break;
		case 8:
			eject_card();
			show();
			break;
		case 9:
			return;
		}
		showmenu();
		while (!(std::cin >> choice) || choice < 0 || choice > 9)
		{
			std::cout << "Enter correct number!";
			std::cin.clear();
			while (std::cin.get() != '\n')
				continue;
		}
	}
}

			

