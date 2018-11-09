#include <sstream>
#include <ctime>
#include <iostream>
#include <iomanip>
#include "cmdhandler.h"
#include "util.h"
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <set>
#include <list>
#include <fstream>
#include <cstring>
#include "datetime.h"
#include "user.h"
#include "twiteng.h"
#include "tweet.h"

using namespace std;
//QUIT
QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
  : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
	return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(TwitEng* eng, std::istream& instr) const
{
	eng->dumpFeeds();
	return HANDLER_QUIT;
}

//AND
AndHandler::AndHandler(){}

AndHandler::AndHandler(Handler* next)
 : Handler(next)
 {

 }

 bool AndHandler::canHandle(const std::string& cmd) const
 {
 	return cmd == "AND";
 }

 Handler::HANDLER_STATUS_T AndHandler::process(TwitEng* eng, std::istream& instr) const
 {

  //Create a vector to hold the hashtag terms.
 	vector<string> terms;
 	string words;
 	
 	while(instr >> words){
 		
 		terms.push_back(words);
 	}
 	if(words == ""){
 		cout << "No hashtag terms." << endl;
 		return HANDLER_ERROR;
 	}
  //Use the vector created to search and display.
 	vector<Tweet*> show = eng->search(terms, 0);
 	cout << show.size() << " matches." << endl;
 	for(unsigned i = 0; i < show.size(); i++){

 		cout << *(show[i]);
 	}

 	return HANDLER_OK;
 }

//OR
OrHandler::OrHandler(){}

OrHandler::OrHandler(Handler* next)
 : Handler(next)
 {

 }

 bool OrHandler::canHandle(const string& cmd) const
 {
 	return cmd == "OR";
 }

 Handler::HANDLER_STATUS_T OrHandler::process(TwitEng* eng, std::istream& instr) const
 {

  //Create a vector to hold hashtag terms, and use it to search.
 	vector<string> terms;
 	string words;
 	while(instr >> words){
 		terms.push_back(words);
 	}
 	if(words == ""){
 		cout << "No hashtag terms." << endl;
 		return HANDLER_ERROR;
 	}
 	vector<Tweet*> show = eng->search(terms, 1);
 	cout << show.size() << " matches." << endl;
 	for(unsigned i = 0; i < show.size(); i++){
 		
 		cout << *(show[i]);
 	}

 	return HANDLER_OK;
 }

//FOLLOW
FollowHandler::FollowHandler(){}

FollowHandler::FollowHandler(Handler* next)
 : Handler(next)
 {

 }

bool FollowHandler::canHandle(const string& cmd) const
{
  return cmd == "FOLLOW";
}

Handler::HANDLER_STATUS_T FollowHandler::process(TwitEng* eng, std::istream& instr) const
{
  string follower, following;
  instr >> follower;
  if(follower == ""){
    cout << "Usernames not given." << endl;
    return HANDLER_ERROR;
  }
  instr >> following;
  if(following == ""){
    cout << "Usernames not given." << endl;
    return HANDLER_ERROR;
  }
  eng->addFollow(follower, following);
  return HANDLER_OK;
}

//SAVE
SaveHandler::SaveHandler(){}

SaveHandler::SaveHandler(Handler* next)
: Handler(next)
{

}

bool SaveHandler::canHandle(const string& cmd) const
{
  return cmd == "SAVE";
}

Handler::HANDLER_STATUS_T SaveHandler::process(TwitEng* eng, std::istream& instr) const
{
  //output the name first
  string fname = "";
  instr >> fname;
  if(fname == ""){
    cout << "No filename is provided." << endl;
    return HANDLER_ERROR;
  }
  
  ofstream ofile;
  ofile.open(fname.c_str());
  eng->saveFile(ofile);
  ofile.close();
  return HANDLER_OK;
}

graphHandler::graphHandler(){}

graphHandler::graphHandler(Handler* next)
: Handler(next)
{

}

bool graphHandler::canHandle(const string& cmd) const
{
  return cmd == "SCC";
}

Handler::HANDLER_STATUS_T graphHandler::process(TwitEng* eng, std::istream& instr) const
{
  string fname = "";
  instr >> fname;
  if(fname == ""){
    cout << "No filename is provided." << endl;
    return HANDLER_ERROR;
  }
  ofstream ofile;
  ofile.open(fname.c_str());
  eng->components(ofile);
  ofile.close();
  return HANDLER_OK;
}

//TWEET
TweetHandler::TweetHandler(){}

TweetHandler::TweetHandler(Handler* next)
 : Handler(next)
 {

 }

bool TweetHandler::canHandle(const std::string& cmd) const
{
	return cmd == "TWEET";
}

Handler::HANDLER_STATUS_T TweetHandler::process(TwitEng* eng, std::istream& instr) const
{
  //Use the time struct to display times first.
	  time_t rawtime;
  	struct tm * timeinfo;

  	time (&rawtime);
  	timeinfo = localtime(&rawtime);

  	cout << timeinfo->tm_year + 1900 <<"-"
  	<< setw(2) << setfill('0') << timeinfo->tm_mon + 1 
  	<< "-" << setw(2) << timeinfo->tm_mday << " "
  	<< setw(2) << setfill('0') << timeinfo->tm_hour
  	<< ":" <<  setw(2) << setfill('0') << timeinfo->tm_min << ":" 
  	<<  setw(2) << setfill('0') << timeinfo->tm_sec;

  	DateTime newtime = DateTime(timeinfo->tm_hour, 
  		timeinfo->tm_min, timeinfo->tm_sec, timeinfo->tm_year + 1900, timeinfo->tm_mon+1, timeinfo->tm_mday);


    //Get the username from the tweet first.
  	string name;
  	instr >> name;
    //If nothing follows the command, give an error message.
  	if(name == ""){
  		cout << " (Nobody created nothing.)" << endl;
  		return HANDLER_ERROR;
  	}
    //Display the tweet.
  	string textword;
  	string text = "";
  	cout << " " << name;
  	while(instr >> textword){
  		cout << " " << textword;
  		text += " ";
  		text += textword;
  	}
  	if(textword == ""){
  		cout << " (Nothing is created.)" << endl;
  		return HANDLER_ERROR;
  	}
  	cout << endl;
    //Add the tweet to the internal class.
  	eng->addTweet(name, newtime, text);

  	return HANDLER_OK;

}