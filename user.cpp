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
#include "msort.h"
#include "twiteng.h"

using namespace std;

User::~User(){
	list<Tweet*>::iterator it;
	for(it = tweet.begin(); it != tweet.end(); ++it){
		delete *it;
	}
}

User::User(string name){
	this->Name = name;
}

string User::name() const{
	return Name;
}

set<User*> User::followers() const
{
	return foer;
}

set<User*> User::following() const
{
	return foing;
}

list<Tweet*> User::tweets() const
{
	return tweet;
}

void User::addFollower(User* u){
	foer.insert(u);
}

void User::setName(string n){
	Name = n;
}

void User::addFollowing(User* u){
	foing.insert(u);
}

void User::addTweet(Tweet* t){
	tweet.push_back(t);
}

//Extract username.
string User::trim(string s){
	string result = "";
	for(unsigned i = 0; i < s.length(); i++){
		if(isalnum(s[i]) || s[i] == ' '){
			result += s[i];
		}
	}
	return result;
}

//Check if two users follow each other.
bool User::exchange(User* other){
	if((other->following()).find(this) != (other->following()).end() && foer.find(other) != foer.end()){
		return true;
	}else{
		return false;
	}
}

//Determine if the user is mentioned but not first mentioned.
int User::firstMention(Tweet* t){
	string content = t->text();
	stringstream ss(content);
	string mention = "";
	ss >> mention;
	if(mention[0] == '@'){
		string name = mention.substr(1);
		name = trim(name);
		if(name == Name){
			return 1;
		}else{
			return 0;
		}
		
		return 0;
	}else{
		while(ss >> mention){
			if(mention[0] == '@'){
				string name = mention.substr(1);
				name = trim(name);
				if(name == Name){
					return 1;
				}
			}
		}
	}
	return 0;
}

//Determine if the tweet should be in the feed files.
bool User::ifremove(Tweet* t){
	User* poster = t->user();
	std::string content = t->text();
	stringstream ss(content);
	std::string first;
	ss >> first;
	if(first[0] == '@'){

		string name = first.substr(1);
		name = trim(name);
		if(name != Name){
			return true;	
		}else{
			if(exchange(poster)){
				return false;
			}else{
				return true;
			}
		}
	}else{
		
		return false;
	}
}

vector<Tweet*> User::getFeed(){
	//Organize the information, make it ready to be written to the feed files.
	vector<Tweet*> tweetlist;
	list<Tweet*>::iterator it;
	for(it = tweet.begin(); it != tweet.end(); ++it){
		tweetlist.push_back(*it);

	}

	set<User*>::iterator ituser;
	for(ituser = foing.begin(); ituser != foing.end(); ++ituser){
		list<Tweet*>::iterator it2;

		for(it2 = ((*ituser)->tweet).begin(); it2 != ((*ituser)->tweet).end(); ++it2){

			if(ifremove(*it2) == false){
				
				tweetlist.push_back(*it2);
			}
				
		}
	}
	TweetComp comp;
	mergeSort<Tweet*>(tweetlist,comp);
	return tweetlist;
}

vector<Tweet*> User::mentionFeed(){
	//Get the mention feed content ready.
	vector<Tweet*> mlist;
	set<Tweet*>::iterator it;
	for(it = mention.begin(); it != mention.end(); ++it){
		mlist.push_back(*it);
	}
	TweetComp comp;
	mergeSort<Tweet*>(mlist, comp);
	return mlist;
}

//Add a tweet to mention.
void User::addMention(Tweet* t){
	mention.insert(t);
}

bool User::operator<(const User& other) const
{
	bool result = this->Name < other.Name;
	return result;
}