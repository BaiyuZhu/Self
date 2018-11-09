#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include "datetime.h"
#include "twiteng.h"
#include "util.h"
#include "tweet.h"
#include "cmdhandler.h"

using namespace std;

DateTime::DateTime(){}

DateTime::DateTime(int hh, int mm, int ss, int year, int month, int day):
hour(hh), min(mm), sec(ss), year(year), month(month), day(day)
{}

//Copy the information from one Datetime class.
DateTime::DateTime(const DateTime& other){
	year = other.year;
	month = other.month;
	day = other.day;
	hour = other.hour;
	min = other.min;
	sec = other.sec;
	
}

//The method to determine which time is earlier.
bool DateTime::operator<(const DateTime& other) const
{
	if(year < other.year){
		return true;
	}else if(year > other.year){
		return false;
	}else{
		if(month < other.month){
			return true;
		}else if(month > other.month){
			return false;
		}else{
			if(day < other.day){
				return true;
			}else if(day > other.day){
				return false;
			}else{
				if(hour < other.hour){
					return true;
				}else if(hour > other.hour){
					return false;
				}else{
					if(min < other.min){
						return true;
					}else if(min > other.min){
						return false;
					}else{
						if(sec < other.sec){
							return true;
						}else{
							return false;
						}
					}
				}
			}
		}
	}
}

DateTime DateTime::operator=(DateTime& other){
	year = other.year;
	month = other.month;
	day = other.day;
	hour = other.hour;
	min = other.min;
	sec = other.sec;
	return *this;
}