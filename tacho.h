#ifndef TACHO_H
#define TACHO_H
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "drivercard.h"
#include "functions.h"
#include "time.h"


class Tacho
{
		std::string plates;
		Time drive_t;
		Time break_t;
		Time dev_break_t1;
		Time dev_break_t2;
		Time w_drive_t;
		Time lw_drive_t;
		Time tw_drive_t;
		Time day_drive_t;
		Time day_break_t;
		Time day_work_t;
		Time day_poa_t;
		Time present;
		Time poa_t;
		time_t work_start_t = 0;
		time_t work_end_t = 0;
		time_t break_start_t = 0;
		time_t break_end_t = 0;
		std::vector<time_t> se_times;
		std::vector<std::string> se_countries;
		time_t tacho_virtual_t = 0;
		time_t checkp;
		struct tm * checkpoint;
		DriverCard driver;
		std::fstream CARDinout;
		std::fstream TACHOinout;
		std::string country;
		std::string countries[246] {"AF", "AX", "AL", "DZ", "AS", "AD", "AO", "AI", "AQ", "AG", "AR", "AM", "AW", "AU", "AT", "AZ", "BS", "BH", "BD", "BB", "BY", "BE", "BZ", "BJ", "BM", "BT", "BO", "BA", "BW", "BV", "BR", "IO", "BN", "BG", "BF", "BI", "KH", "CM", "CA", "CV", "KY", "CF", "TD", "CL", "CN", "CX", "CC", "CO", "KM", "CG", "CD", "CK", "CR", "CI", "HR", "CU", "CY", "CZ", "DK", "DJ", "DM", "DO", "EC", "EG", "SV", "GQ", "ER", "EE", "ET", "FK", "FO", "FJ", "FI", "FR", "GF", "PF", "TF", "GA", "GM", "GE", "DE", "GH", "GI", "GR", "GL", "GD", "GP", "GU", "GT", "GG", "GN", "GW", "GY", "HT", "HM", "VA", "HN", "HK", "HU", "IS", "IN", "ID", "IR", "IQ", "IE", "IM", "IL", "IT", "JM", "JP", "JE", "JO", "KZ", "KE", "KI", "KP", "KR", "KW", "KG", "LA", "LV", "LB", "LS", "LR", "LY", "LI", "LT", "LU", "MO", "MK", "MG", "MW", "MY", "MV", "ML", "MT", "MH", "MQ", "MR", "MU", "YT", "MX", "FM", "MD", "MC", "MN", "ME", "MS", "MA", "MZ", "MM", "NA", "NR", "NP", "NL", "AN", "NC", "NZ", "NI", "NE", "NG", "NU", "NF", "MP", "NO", "OM", "PK", "PW", "PS", "PA", "PG", "PY", "PE", "PH", "PN", "PL", "PT", "PR", "QA", "RE", "RO", "RU", "RW", "BL", "SH", "KN", "LC", "MF", "PM", "VC", "WS", "SM", "ST", "SA", "SN", "RS", "SC", "SL", "SG", "SK", "SI", "SB", "SO", "ZA", "GS", "ES", "LK", "SD", "SR", "SJ", "SZ", "SE", "CH", "SY", "TW", "TJ", "TZ", "TH", "TL", "TG", "TK", "TO", "TT", "TN", "TR", "TM", "TC", "TV", "UG", "UA", "AE", "GB", "US", "UM", "UY", "UZ", "VU", "VE", "VN", "VG", "VI", "WF", "EH", "YE", "ZM", "ZW"};
		void show_time() const;										/* displays virtual time and date of the tachograph */
		void show_anytime(const time_t & t) const;					/* displays selected time_t variable */
		void print_anytime(const time_t & t, std::fstream & fs) const;	/* prints selected time_t variable in output file */
		void print_short_time(std::fstream & fs) const;				/* prints virtual time of tachograph in output file*/
		void print_date(const time_t & t, std::fstream & fs) const;
		void check_shift_t(const Time & a) const;					/* makes sure shift time is not overran */
		void enter_country();
		void check_wday(const Time & a, Time & act_t);

	public:
		Tacho();
		~Tacho();
		void show() const;
		void add_dt(const Time & a);										/* adds drive time */
		void add_bt(const Time & a);										/* adds break time */
		void add_wt(const Time & a);										/* adds work time */
		void add_poa(const Time & a);										/* adds period of availability */
		void start_work();
		void end_work();
		void insert_card();
		void eject_card();
		void use_tacho();
};

#endif // TACHO_H
