#include <iostream>
#include <string>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";

template<typename T>
void fillmap(map<int, T>& myMap){
    string word;
    ifstream myReader;
    myReader.open("dictionary.txt");
    while(myReader >> word){
        if (myMap.find(word.length()) != myMap.end()){
                myMap.find(word.length())->second.push_back(word);
        }else{
            myMap.insert(pair<int, T>(word.length(), T{word}));
        }
    }
    myReader.close();
}

template<typename T>
map<string, T> getfamily(T& word_list, char c){
    map<string, T> ret;
    for(auto word:word_list){
        string line = "";
        for(auto symbol:word){
            if(symbol == c){
                line += c;
            }else{
                line += "-";
            }
        }
    }
}


int main() {
    map<int, vector<string> > myMap;
    fillmap(myMap);

    cout << myMap.find(2)->second[1] <<endl;
    cout << "Welcome to Hangman." << endl;

    // TODO: Finish the program!


    return 0;
}
