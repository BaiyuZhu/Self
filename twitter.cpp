#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <list>
#include <vector>
#include "twiteng.h"
#include "util.h"
#include "tweet.h"
#include "cmdhandler.h"

using namespace std;

//Define operator << for tweets, users and time.
ostream& operator<<(std::ostream& os, const Tweet& t){
  os << t.times << *(t.username) << t.texts << endl;
  return os;
}

ostream& operator<<(std::ostream& os, const User& u){
  os << u.Name;
  return os;
}

ostream& operator<<(std::ostream& os, const DateTime& other){

  os << other.year << "-";
  if(other.month < 10){
    os << "0"<<other.month<<"-";
  }else{
    os << other.month << "-"; 
  } 

  if(other.day < 10){
    os << "0"<<other.day<<" ";
  }else{
    os << other.day << " "; 
  }

  if(other.hour < 10){
    os << "0"<<other.hour<<":";
  }else{
    os << other.hour << ":"; 
  }

  if(other.min < 10){
    os << "0"<<other.min<<":";
  }else{
    os << other.min << ":"; 
  }

  if(other.sec < 10){
    os << "0"<<other.sec<<" ";
  }else{
    os << other.sec << " "; 
  }

  return os;
}

Handler* createHandlers();

Handler* createHandlers()
{
  return new AndHandler(
    new OrHandler(
      new TweetHandler(
        new FollowHandler(
          new SaveHandler(
            new graphHandler(
              new QuitHandler
              )
            )
          ) 
      )
    )
  );
}

int main(int argc, char* argv[])
{
  if(argc < 2){
    cerr << "Please provide the twitter data file" << endl;
    return 1;
  }
  TwitEng twit;
  if ( twit.parse(argv[1]) ){
    cerr << "Unable to parse " << argv[1] << endl;
    return 1;
  }

  // Initialize command handlers
  Handler* handlers = createHandlers();

  cout << "=====================================" << endl;
  cout << "Menu: " << endl;
  cout << "  AND term term ...                  " << endl;
  cout << "  OR term term ...                   " << endl;
  cout << "  TWEET username tweet_text" << endl;
  cout << "  FOLLOW name_of_follower name_to_follow" << endl;
  cout << "  SAVE name_of_the_file_to_save" << endl;
  cout << "  SCC results-filename" << endl;
  cout << "  QUIT (and write feed files)        " << endl;
  cout << "=====================================" << endl;

  Handler::HANDLER_STATUS_T status = Handler::HANDLER_OK;
  while(status != Handler::HANDLER_QUIT && 
        status != Handler::HANDLER_KILL)
  {
    cout << "\nEnter command: " << endl;
    string line;
    getline(cin,line);
    stringstream ss(line);
    string cmd;
    if((ss >> cmd)){
      // convert to upper case
      convUpper(cmd); 
      // Invoke the chain of responsibility
      status = handlers->handle(&twit, cmd, ss);
    }
  }
  delete handlers;
  return 0;
}
