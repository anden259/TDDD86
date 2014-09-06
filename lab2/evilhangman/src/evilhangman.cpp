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

string get_family_string(string word,char c,string start){
   for (int i = 0; i<word.length(); i++){
       if (start[i]=='-' && word[i] == c){
           start[i]=c;

       }
   }
   return start;

}


template<typename T>
map<string, T> getfamily(T& word_list, char c,string start){
    map<string, T> ret;
    for(auto word:word_list){
        string family_string=get_family_string(word,c,start);
        if(ret.find(family_string)==ret.end()){
            ret.insert(ret.begin(),pair<string,T>(family_string,T{word}));
        }else{
            ret.find(family_string)->second.push_back(word);
        }
    }
    return ret;
}
//template<typename T>
pair<string,vector<string> > get_bigest_word_list(map<string,vector<string> >& family_map){
    pair <string,vector<string> > ret_map_pair;
    int max_len,tmp;

    for(auto pair_in_map:family_map){
        tmp=pair_in_map.second.size();
        if(tmp>max_len){
            max_len=tmp;
            ret_map_pair=pair_in_map;
        }
    }
    return ret_map_pair;
}
template<typename T>
int get_valid_length(T& myMap){
    int input;
    do{

        cout << "number of letters : ";
        cin >> input;

    }while(myMap.find(input)==myMap.end());
    return input;
}
int get_guesses(){
    int input;
    do{
        cout << "number of guesses min is 1 and max is 26 : ";
        cin >> input;

    }while(input<=0 || input>26);

    return input;
}

bool show_list(){
    char input;
    cout << "do you wont to see the word list ? type y or n : ";
    cin >> input;
    return  input=='y';
}
template<typename T>
void print_word_list(T& word_list){
    for (auto word:word_list){
        cout << word <<" ";
    }
    cout <<endl;
}

char get_letter(string guesst){
    char ret;
    do{
        cout << "guess a letter :";
        cin >> ret;
        ret=tolower(ret);
    }while(guesst.find(ret)!=string::npos && alphabet.find(ret)!=string::npos);
    return ret;

}


void play_game(int len,int gue,bool show,vector<string> word_list){
    string letters_guesst="",word_to_guess(len,'-');
    char letter;
    while(word_to_guess.find('-')!=string::npos && gue>0){
        cout << "you have " << gue <<" guesses left" <<endl;
        cout << "letters you have guesst : " <<letters_guesst <<endl;
        cout << "the word to guess locks like this : " << word_to_guess<<endl;
        if (show) print_word_list(word_list);
        letter=get_letter(letters_guesst);
        letters_guesst+=letter;

        map<string,vector<string> > family= getfamily(word_list,letter,word_to_guess);
        pair<string,vector<string> > word_list_pair=get_bigest_word_list(family);

        word_to_guess=word_list_pair.first;
        word_list=word_list_pair.second;

        gue--;

    }

    if(word_to_guess.find("-")==string::npos){
        cout << "nice you won!!!" <<endl;
    }else{
        cout << "you lost" <<endl;
    }




    return;
}

template<typename T>
void play_game_loop(T& myMap){
    char input;
    int len,gue;
    bool show;
    do{
        len=get_valid_length(myMap);
        gue=get_guesses();
        show=show_list();
        play_game(len,gue,show,myMap.find(len)->second);

        cout << "to quit type q : ";
        cin >> input;

    }while(input!='q');
    return;
}


int main() {
    map<int, vector<string> > myMap;
    fillmap(myMap);

    play_game_loop(myMap);

//    cout << getfamily(myMap.find(2)->second,'a',"--").find("--")->second[0] <<endl;

//    for(auto entry:getfamily(myMap.find(2)->second,'a',"--")){
//        cout << "family string "<< entry.first << endl;
//        for(auto string_and_word:entry.second){
//            cout << "string : " << string_and_word << endl;
//        }
//    }
//    cout << "the bigest list it this :" <<endl;
//    map<string,vector<string> >tmp=getfamily(myMap.find(2)->second,'a',"--");
//    for(auto string_and_word: get_bigest_word_list(tmp).second) {
//        cout << "string : " << string_and_word << endl;
//    }



    cout << "Welcome to Hangman." << endl;





    // TODO: Finish the program!


    return 0;
}
