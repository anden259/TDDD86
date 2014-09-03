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

    for(unsigned int index=0;index<word.length();index++){
        string tmp=word;
        for(unsigned int c=0;c<alphabet.length();c++){
            //cout <<"befor word change : " << word << endl;
            tmp[index]=alphabet[c];
            //cout <<"after change word : " << word << endl;
            if(is_word(tmp)){
              //  cout <<"tha word I added is : " << word << endl;
                ret.insert(ret.end(),tmp);
            }
        }
    }
    return ret;
}

void print_stack(stack<string>& my_stack ){

    for(auto word:my_stack){
        cout << word << " ";
    }
    cout << endl;
}


void wordChain(string w1,string w2){
    queue<stack<string>> my_queue;
    my_queue.push(new stack <string> (w1));

    while(my_queue.size()!=0){
        stack <string> tmp_stack=my_queue.pop();
        if(tmp_stack.peek()==w2){
            printf("Chain from %s back to %s:\n",w1,w2);
            print_stack(tmp_stack);
            return;
        }else{

            for(auto word:get_near(w1)){
                tmp_stack.push(word);
                my_queue.push(tmp_stack);
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
    string w1,w2;

    cin >> w1 >> w2;

    wordChain(w1,w2);


    // TODO: Finish the program!

    return 0;
}
