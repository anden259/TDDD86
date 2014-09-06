#include <iostream>
#include <string>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <fstream>

using namespace std;

const string alphabet  = "abcdefghijklmnopqrstuvwxyz";
// fillmap files the map with nummber of letters and a vector with all words with that length
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
// get_family_string return the family string of that word
// baste on the word start
string get_family_string(string word, char c, string start){
   for (size_t i = 0; i < word.length(); i++){
       if (start[i] == '-' && word[i] == c){
           start[i] = c;

       }
   }
   return start;
}

// getfamily returns a map with familly string as key and a vector of string as word list
template<typename T>
map<string, T> getfamily(T& word_list, char c, string start){
    map<string, T> ret;
    for(auto word:word_list){
        string family_string = get_family_string(word, c, start);
        if(ret.find(family_string) == ret.end()){
            ret.insert(ret.begin(), pair<string, T>(family_string, T{word}));
        }else{
            ret.find(family_string)->second.push_back(word);
        }
    }
    return ret;
}
// returns a pair with family string as key and a vector as element
// the pair returnd is the one with bigest vector
//template<typename T>
pair<string, vector<string> > get_bigest_word_list(map<string, vector<string> >& family_map){
    pair <string, vector<string> > ret_map_pair;
    int max_len(0), tmp;

    for(auto pair_in_map:family_map){
        tmp = pair_in_map.second.size();
        if(tmp > max_len){
            max_len = tmp;
            ret_map_pair = pair_in_map;
        }
    }
    return ret_map_pair;
}
// returns the number of dashes in word
int count_dash(string word){
    int ret(0);
    for (char c : word){
        if(c=='-') ret++;
    }
    return ret;
}

// returns the pair with moste dashes in the key string
//template<typename T>
pair<string, vector<string> > get_most_dashes_word_list(map<string, vector<string> >& family_map){
    pair <string, vector<string> > ret_map_pair;
    int max_dash(0), tmp_dash(0);
    for(auto pair_in_map:family_map){
        tmp_dash = count_dash(pair_in_map.first);
        if(tmp_dash > max_dash){
            max_dash = tmp_dash;
            ret_map_pair = pair_in_map;
        }
    }
    return ret_map_pair;
}

// loop to get the user to enter valid number of letters
template<typename T>
int get_valid_length(T& myMap){
    int input;
    do{

        cout << "number of letters : ";
        cin >> input;

    }while(myMap.find(input) == myMap.end());
    return input;
}
//loop to get nummber of guesses
int get_guesses(){
    int input;
    do{
        cout << "number of guesses min is 1 and max is 26 : ";
        cin >> input;

    }while(input <= 0 || input > 26);

    return input;
}
// return true if the user wonts to see the word list
bool show_list(){
    char input;
    cout << "do you wont to see the word list ? type y or n : ";
    cin >> input;
    return  input == 'y';
}
// print a word list
template<typename T>
void print_word_list(T& word_list){
    for (auto word:word_list){
        cout << word << " ";
    }
    cout << endl;
    return;
}
// returns the new! letter enterd by the user
char get_letter(string guesst){
    char ret;
    do{
        cout << "guess a letter :";
        cin >> ret;
        ret = tolower(ret);
    }while(alphabet.find(ret) == string::npos || guesst.find(ret) != string::npos);
    return ret;
}

// start the game
void play_game(int len, int gue, bool show, vector<string> word_list){
    string letters_guesst = "", word_to_guess(len, '-');
    char letter;
    while(word_to_guess.find('-') != string::npos && gue > 0){
        cout << "you have " << gue <<" guesses left" << endl;
        cout << "letters you have guesst : " << letters_guesst << endl;
        cout << "the word to guess locks like this : " << word_to_guess << endl;
        if (show) print_word_list(word_list);
        letter = get_letter(letters_guesst);
        letters_guesst += letter;

        map<string, vector<string> > family = getfamily(word_list, letter, word_to_guess);
        pair<string, vector<string> > word_list_pair;
        if (gue>1){
            word_list_pair = get_bigest_word_list(family);
        }else{
            word_list_pair = get_most_dashes_word_list(family);
        }

        if(word_to_guess == word_list_pair.first){
            gue--;
        }

        word_to_guess = word_list_pair.first;
        word_list = word_list_pair.second;
    }

    if(word_to_guess.find("-") == string::npos){
        cout << "nice you won!!! " << word_to_guess << endl;
    }else{
        cout << "you lost!!! the word was : " << word_list[0] << endl;
    }
    return;
}
// the loop to restart the game after played
template<typename T>
void play_game_loop(T& myMap){
    char input;
    int len, gue;
    bool show;
    do{
        len = get_valid_length(myMap);
        gue = get_guesses();
        show = show_list();
        play_game(len, gue, show, myMap.find(len)->second);

        cout << "to quit type q : ";
        cin >> input;

    }while(input != 'q');
    return;
}
// main to start the program
int main() {
    map<int, vector<string> > myMap;
    fillmap(myMap);

    cout << "Welcome to Hangman." << endl;
    play_game_loop(myMap);

    return 0;
}
