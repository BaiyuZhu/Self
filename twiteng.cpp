#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <list>
#include <fstream>
#include <cstring>
#include <stack>
#include "datetime.h"
#include "user.h"
#include "twiteng.h"
#include "util.h"
#include "tweet.h"
#include "msort.h"

using namespace std;

TwitEng::TwitEng(){}
//Clear the memory user class occupied.
TwitEng::~TwitEng(){
	for(unsigned i = 0; i < users.size(); i++){
		delete users[i];
	}
}

//Read the time from string text.
DateTime TwitEng::readtime(string date, string time)
{
	date[4] = ' ';
	date[7] = ' ';
	time[2] = ' ';
	time[5] = ' ';

	stringstream ss(date);
	int num;
	DateTime dt;
	ss >> num;
	dt.year = num;
	ss >> num;
	dt.month = num;
	ss >> num;
	dt.day = num;

	stringstream st(time);
	st >> num;
	dt.hour = num;
	st >> num;
	dt.min = num;
	st >> num;
	dt.sec = num;
	return dt;
}

//Extract hashtag terms from a tweet and put the pair into the index.
void TwitEng::hashtag(Tweet* t){
	map<string, set<Tweet*> >::iterator it;
	//vector<string> hashtags;
	string temp = t-> text();
	stringstream ss(temp);
	string word;
	while(ss >> word){
		if(word[0] == '#'){

			string word1 = word.substr(1);
			convUpper(word1);
			it = hashid.find(word1);
			if(it == hashid.end()){
				set<Tweet*> twitlist;
				twitlist.insert(t);
				hashid.insert(make_pair(word1, twitlist));
			}else{
				hashid[word1].insert(t);
			}
		}
	}
}



//Read information from the input file.
bool TwitEng::parse(char* filename)
{
	ifstream infile;
	infile.open(filename);

	if(infile.fail()){
		return true;
	}

	string line;
	getline(infile,line);
	stringstream ss(line);
	int usernum;
	ss >> usernum;
	vector<vector<string> > lines;
	
	for(int i = 0; i < usernum; i++){
		getline(infile,line);
		if(line != ""){
			vector<string> names;
			stringstream sn(line);
			string name;
			while(sn >> name){
				
				names.push_back(name);
			}
			
			lines.push_back(names);
		}
	}

	map<string, User*> usermap;
	for(unsigned i = 0; i < lines.size(); i++){
		User* u = new User(lines[i][0]);	
		users.push_back(u);
		usermap.insert(make_pair(lines[i][0],u));
	}
	//Add followers and who the user is following into class.
	for(unsigned i = 0; i < lines.size(); i++){
		User* u = usermap[lines[i][0]];
		if(lines[i].size() > 1){
			for(unsigned j = 1; j < lines[i].size(); j++){
				if(usermap.find(lines[i][j]) == usermap.end()){
					cout << "Unregistered user appeared." << endl;
					return true;
				}
				User* u2 = usermap[lines[i][j]];
				(*u).addFollowing(u2);
				(*u2).addFollower(u);
			}
		}
	}

	//Read and process rest of the tweet.
	while(getline(infile, line)){
		if(line == ""){
			continue;
		}
		stringstream sr(line);
		string twitdate;
		string twittime;
		sr >> twitdate >> twittime;
		DateTime time = readtime(twitdate, twittime);
		string twitrest;
		string twitname;
		string twittext = "";

		sr >> twitname;

		while(sr >> twitrest){
			twittext = twittext + " " + twitrest;
		}

		Tweet* tptr = new Tweet(usermap[twitname], time, twittext);
		for(unsigned i = 0; i < users.size(); i++){
			if(users[i]->firstMention(tptr) == 1){
				users[i]->addMention(tptr);
			}
		}
		//Add the tweet to the user class.
		usermap[twitname]->addTweet(tptr);

		hashtag(tptr);	
	}

	infile.close();
	return false;
}

void TwitEng::addFollow(string& follower, string& following){
	if(follower == following){
		cout << "You can follow your self." << endl;
		return;
	}
	int foer = -1;
	int foing = -1;
	for(unsigned i = 0; i < users.size(); i++){
		if(follower == users[i] -> name()){
			foer = i;
		}else if(following == users[i] -> name()){
			foing = i;
		}
	}
	if(foer == -1 || foing == -1){
		cout << "The follower or the person who shoud be followed is not a registered user." << endl;
		return;
	}else{
		users[foer]-> addFollowing(users[foing]);
		users[foing] -> addFollower(users[foer]);
	}
}

void TwitEng::saveFile(std::ostream& ofile)
{
	vector<User*> out = users;
	//Sort the users
	UserComp comp;
	mergeSort<User*>(out,comp);
	//output the users and the people they are following.
	ofile << out.size() << endl;
	for(unsigned i = 0; i < out.size(); i++){
		ofile << out[i] -> name();
		set<User*> Followset = out[i]->following();
		set<User*>::iterator it;
		
		for(it = Followset.begin(); it != Followset.end(); ++it){

			ofile << " " << (*it)->name();
		}
		ofile << endl;
	}

	vector<Tweet*> twitvec;
	for(unsigned i = 0; i < users.size(); i++){
		
		list<Tweet*>::iterator itt;
		list<Tweet*> tlist = users[i]->tweets();
		if(!tlist.empty()){
			
			for(itt = tlist.begin(); itt != tlist.end(); ++itt){
				twitvec.push_back(*itt);
			}
		}	
	}
	//Sort the tweets.
	ReverseComp comp2;
	mergeSort<Tweet*>(twitvec, comp2);
	for(unsigned i = 0; i < twitvec.size(); i++){
		ofile << *(twitvec[i]);
	}
}

void TwitEng::addTweet(std::string& username, DateTime& time, std::string& text)
{
	int cnt = 0;
	for(unsigned i = 0; i < users.size(); i++){
		if(username == users[i] -> name()){
			Tweet* newtweet = new Tweet(users[i], time, text);
			users[i] -> addTweet(newtweet);
			hashtag(newtweet);
			cnt ++;
		}
	}
	//Detect if the username is valid.
	if(cnt == 0){
		cout << "Not a registered user, the tweet is not posted." << endl;
	}
}

//Generate the feed files.
void TwitEng::dumpFeeds(){
	int usernum = users.size();
	for(int i = 0; i < usernum; i++){

		ofstream ofile;
		ofstream ofile2;
		string fname2 = users[i] -> name();
		string fname = users[i] -> name();
		fname += ".feed";
		fname2 += ".mentions";
		ofile.open(fname.c_str());
		ofile2.open(fname2.c_str());
		ofile << users[i] -> name() << endl;
		ofile2 << users[i] -> name() << endl;
		for(unsigned j = 0; j < users[i]->getFeed().size(); j++){
			ofile << *(users[i]->getFeed()[j]);
		}

		for(unsigned k = 0; k < users[i]->mentionFeed().size(); k++){
			ofile2 << *(users[i]->mentionFeed()[k]);
		}
		ofile.close();
		ofile2.close();
	}
}

//Give the intersection of two tweet sets.
set<Tweet*> TwitEng::intersection(set<Tweet*>& self, set<Tweet*>& other){
  set<Tweet*> result;
  set<Tweet*>::iterator it;
  for(it = other.begin(); it != other.end(); ++it){
    if(self.find(*it) != self.end()){
      result.insert(*it);
    }
  }
  return result;
}

//Give the union of two tweet sets.
set<Tweet*> TwitEng::unions(set<Tweet*>& self, set<Tweet*>& other){
  set<Tweet*> result;
  set<Tweet*>::iterator it;
  for(it = other.begin(); it != other.end(); ++it){
    result.insert(*it);
  }
  for(it = self.begin(); it != self.end(); ++it){
    result.insert(*it);
  }
  return result;

}

vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy){
	for(unsigned i = 0; i < terms.size(); i++){
		convUpper(terms[i]);
	}
	if(strategy == 0){
		//Iterating through the hashtag terms given, and find their intersection.
		vector<Tweet*> twitlist;
		set<Tweet*> result;
		set<Tweet*> searchset = hashid[terms[0]];
		set<Tweet*>::iterator it;
		for(it = searchset.begin(); it != searchset.end(); ++it){
			result.insert(*it);
		}
		for(unsigned i = 1; i < terms.size(); i++){
			set<Tweet*>& temp = hashid[terms[i]];

			result = intersection(result,temp);
		}
		
		for(it = result.begin(); it != result.end(); ++it){
			twitlist.push_back(*it);
		}
		TweetComp comp;
		//sort(twitlist.begin(),twitlist.end(),TweetComp());
		mergeSort<Tweet*>(twitlist,comp);
		return twitlist;
	}else if(strategy == 1){
		//Iterating through the hashtag terms given, and find their union.
		vector<Tweet*> twitlist;
		set<Tweet*> result;
		set<Tweet*> searchset = hashid[terms[0]];
		set<Tweet*>::iterator it;
		for(it = searchset.begin(); it != searchset.end(); ++it){
			result.insert(*it);
		}
		for(unsigned i = 1; i < terms.size(); i++){
			set<Tweet*>& temp = hashid[terms[i]];
			result = unions(result,temp);
		}

		
		
		for(it = result.begin(); it != result.end(); ++it){

			twitlist.push_back(*it);
		}
		//sort(twitlist.begin(),twitlist.end(),TweetComp());
		TweetComp comp;
		mergeSort<Tweet*>(twitlist,comp);
		return twitlist;
	}else{
		//If nothing to search, give an empty vector.
		vector<Tweet*> twitlist;
		return twitlist;
	}
}


//Use tarjan's algorithm to find components.
void TwitEng::dfs(stack<User*>& stk, int& idx, User* user, map<User*, userdata>& usermap, ofstream& ofile, 
	int& count){
	
	usermap[user].value = idx;
	usermap[user].lowlink = idx;
	idx += 1;
	stk.push(user);
	usermap[user].onstack = true;
	usermap[user].define = true;
	set<User*> following = user->following();
	set<User*>::iterator it;
	for(it = following.begin(); it != following.end(); ++it){
		if(usermap[*it].define != true){
			dfs(stk, idx, *it, usermap, ofile, count);
			usermap[user].lowlink = min(usermap[user].lowlink, usermap[*it].lowlink);
		}else if(usermap[*it].onstack == true){
			usermap[user].lowlink = min(usermap[user].lowlink, usermap[*it].lowlink);
		}
	}

	if(usermap[user].value == usermap[user].lowlink){
		count += 1;
		ofile << "Component " << count << endl;
		do{
			User* first = stk.top();
			ofile << *first << endl;
			stk.pop();
			usermap[first].onstack = false;
			if(first == user){
				break;
			}
		}while(true);
		ofile << endl;
	}
}

//Generate all the components of an input file.
void TwitEng::components(ofstream& ofile)
{
	stack<User*>stk;
	int idx = 0;
	int count = 0;
	map<User*, userdata> usermap;
	for(unsigned i = 0; i < users.size(); i++){
		userdata data;
		data.define = false;
		data.onstack = false;
		usermap.insert(make_pair(users[i], data));
	}
	for(unsigned i = 0; i < users.size(); i++){
		if(usermap[users[i]].define != true){
			dfs(stk, idx, users[i], usermap, ofile, count);
		}
	}
	
}