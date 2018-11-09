#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <list>
#include "datetime.h"
#include "user.h"
#include "tweet.h"

using namespace std;

Tweet::Tweet(){}

//Constructor
Tweet::Tweet(User* user, DateTime& time, std::string& text){
	this->username = user;
	this->times = time;
	this->texts = text;
}

DateTime const & Tweet::time() const
{
	return times;
}

string const & Tweet::text() const
{
	return texts;
}

//Determine which tweet is posted earlier.
bool Tweet::operator<(const Tweet& other) const
{
	return this->times < other.times;
}

User* Tweet::user() const
{
	return username;
}