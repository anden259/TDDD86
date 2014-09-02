#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <map>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";

map<string,bool> myMap;




void fillmap(){
    string word;
    ifstream myReader;
    myReader.open("dictionary.txt");
    while(myReader >> word){
        myMap.insert(pair<string,bool>(word,true));
    }
    myReader.close();
}
bool is_word(string word){
    map<string,bool>::iterator it;
    it=myMap.find(word);
    if(it!=myMap.end() && it->second){
        it->second=false;
        return true;
    }else{
        return false;
    }
}

vector<string> get_near(string word){
    vector<string> ret;
    // word false !!!?
    for(int index=0;index<word.length();index++){
        for(int c=0;c<alphabet.length();c++){
            word[index]=c;
            if(is_word(word)){
                ret.insert(word);
            }
        }
    }
}



int main() {
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    fillmap();

    cout << "Please type two words: ";

    // TODO: Finish the program!

    return 0;
}
