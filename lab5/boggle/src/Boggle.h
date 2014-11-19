// This is the .h file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// anden259 andno037

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "set.h"
#include "grid.h"
#include "lexicon.h"

using namespace std;

class Boggle
{
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;

    void generate_board();
    void generate_words();
    bool generate_user_board(string input);
    string board_string(); // Grid operator[] const is buggy, and does not compile, can't const declare

    void reset();

    bool add_user_word(string input);

    int get_user_word_count() const;
    int get_user_points() const;
    string get_user_words(); // Set<T>.toString() is NOT const, therfore we can't const declare.

    int get_computer_word_count() const;
    int get_computer_points() const;
    string get_computer_words(); // Set<T>.toString() is NOT const, therfore we can't const declare.

private:
    int user_points=0;
    Set<string> computer_set, user_set;
    Grid<string> board = Grid<string>(BOARD_SIZE, BOARD_SIZE);
    Grid<bool> visited = Grid<bool>(BOARD_SIZE, BOARD_SIZE);
    Lexicon lex {DICTIONARY_FILE};

    void generate_words_help(int r, int c, string current);
};

#endif
