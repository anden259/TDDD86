// anden259, ando037

#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <map>
using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";

//fill a map with words to be used
void fillmap(map<string,bool>& myMap){
    string word;
    ifstream myReader;
    myReader.open("dictionary.txt");
    while(myReader >> word){
        myMap.insert(pair<string,bool>(word,true));
    }
    myReader.close();
}

//if the word is in the map and available to use
bool is_word(string word,map<string,bool>& myMap){
    map<string,bool>::iterator it;
    it = myMap.find(word);
    if(it != myMap.end() && it->second){
        it->second = false;
        return true;
    }else{
        return false;
    }
}

//return all the words with one letter differance.
vector<string> get_near(string word,map<string,bool>& myMap){
    vector<string> ret;
    is_word(word,myMap);  //to make word false!!!
    for(unsigned int index = 0; index < word.length(); index++){
        string tmp = word;
        for(unsigned int c = 0; c < alphabet.length(); c++){
            tmp[index] = alphabet[c];
            if(is_word(tmp,myMap)){
                ret.insert(ret.end(),tmp);
            }
        }
    }
    return ret;
}

//print a stack of strings
void print_stack(stack<string> my_stack ){

    while(!my_stack.empty()){
        cout << my_stack.top() << " ";
        my_stack.pop();
    }
    cout << endl;
}

//Create a word chain from one word to the other
void wordChain(string w1,string w2,map<string,bool>& myMap){
    queue<stack<string> > my_queue;
    stack <string> tmp_stack;
    tmp_stack.push(w1);
    my_queue.push(tmp_stack);

    // width first search
    // loop through all stacks in the queue (adding and removing from them) until
    // there is none left or we find the word.
    do{
        tmp_stack = my_queue.front();
        my_queue.pop();
        if(tmp_stack.top() == w2){
            printf("Chain from %s back to %s:\n",w1.c_str(),w2.c_str());
            print_stack(tmp_stack);
            return;
        }else{
            for(auto word:get_near(tmp_stack.top(), myMap) ){
                stack<string> tmp2_stack = tmp_stack;
                tmp2_stack.push(word);
                my_queue.push(tmp2_stack);
            }
        }
    } while(!my_queue.empty());
}

//main
int main() {
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;
    map<string,bool> myMap;
    fillmap(myMap);

    cout << "Please type two words: ";
    string w1,w2;

    cin >> w1 >> w2;

    wordChain(w1,w2,myMap);

    cout <<"Have a nice day."<<endl;

    return 0;
}
