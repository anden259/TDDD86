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


int main() {
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    fillmap();

    cout << "aal is a word? " << is_word("aa")<< endl;
    cout << "aal is a word? " << is_word("aal") << " vad fan " << is_word("aa")  << endl;
    cout << "aal is a word? " << is_word("aal") << " vad fan " << is_word("aa")  << endl;
    cout << "aal is a word? " << is_word("aal") << " vad fan " << is_word("aa")  << endl;


    cout << "Please type two words: ";

    // TODO: Finish the program!

    return 0;
}
