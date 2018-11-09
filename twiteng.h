#ifndef TWITENG_H
#define TWITENG_H
#include <map>
#include <string>
#include <set>
#include <stack>
#include <fstream>
#include <vector>
#include "user.h"
#include "datetime.h"


class TwitEng
{
 public:
  TwitEng();
  ~TwitEng();

  struct userdata{
    bool onstack;
    bool define;
    int value;
    int lowlink;
  };

  DateTime readtime(std::string date, std::string time);
  /**
   * Parses the Twitter database and populates internal structures
   * @param filename of the database file
   * @return true if there is an error, false if successful
   */
  bool parse(char* filename);

  void print(){
    std::vector<User*>::iterator it;
    for(it = users.begin(); it != users.end(); ++it){
      std::cout << **it << std::endl;


    } 
  }

  /**
   * Allocates a tweet from its parts and adds it to internal structures
   * @param username of the user who made the tweet
   * @param timestamp of the tweet
   * @param text is the actual text of the tweet as a single string
   */
  void addTweet(std::string& username, DateTime& time, std::string& text);

  /**
   * Searches for tweets with the given words and strategy
   * @param words is the hashtag terms in any case without the '#'
   * @param strategy 0=AND, 1=OR
   * @return the tweets that match the search
   */
  std::vector<Tweet*> search(std::vector<std::string>& terms, int strategy);

  /**
   * Dump feeds of each user to their own file
   */
  void dumpFeeds();

  friend bool check(std::string s);


  void hashtag(Tweet* t);

  std::set<Tweet*> intersection(std::set<Tweet*>& self, std::set<Tweet*>& other);

  std::set<Tweet*> unions(std::set<Tweet*>& self, std::set<Tweet*>& other);

  void addFollow(std::string& follower, std::string& following);

  void saveFile(std::ostream& ofile);

  void components(std::ofstream& ofile);

  void dfs(std::stack<User*>& stk, int& idx, User* user, 
    std::map<User*, userdata>& usermap, std::ofstream& ofile, int& count);

  
  

  //std::vector<Tweet*> giveTweets();
  /* You may add other member functions */
 private:
  /* Add any other data members or helper functions here  */
  std::vector<User*> users;
  
  std::map<std::string, std::set<Tweet*>   > hashid;

};


#endif
