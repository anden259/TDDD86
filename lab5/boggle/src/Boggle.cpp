// anden259 andno037

#include <sstream>
#include <algorithm>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = {        // the letters on all 6 sides of every cube
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// Genrate a random board, and set our visited grid to false everywhere.
void Boggle::generate_board()
{
    for (int i = 0; i < NUM_CUBES; ++i) {
        random_shuffle(CUBES[i].begin(), CUBES[i].end());
    }
    shuffle(CUBES, NUM_CUBES);

    for (int i = 0; i < 4; ++i) {
        board[i][0] = CUBES[4 * i][0];
        board[i][1] = CUBES[4 * i + 1][0];
        board[i][2] = CUBES[4 * i + 2][0];
        board[i][3] = CUBES[4 * i + 3][0];

        visited[i][0] = false;
        visited[i][1] = false;
        visited[i][2] = false;
        visited[i][3] = false;
    }
}

// Get all letters from the board, so it can be printed to screen.
string Boggle::board_string()
{
    string ret;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            ret += board[r][c];
        }
        ret += "\n";
    }
    return ret;
}

// ressets the board to a valid starting state.
void Boggle::reset()
{
    user_points = 0;
    user_set.clear();
    computer_set.clear();
}

// generate all possible words for the computer
void Boggle::generate_words()
{
    // loop over all starting letters
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            generate_words_help(r, c, "");
        }
    }
}

// build all words from the starting letter recursevly.
void Boggle::generate_words_help(int r, int c, string current)
{
    visited[r][c] = true;
    current += board[r][c];

    if (!lex.containsPrefix(current)) {
        visited[r][c] = false;
        return;
    }
    if (current.size() >= 4 && lex.contains(current)) {
        computer_set.insert(current);
    }

    // continue to all neighbours that is unvisited.
    for (int dr = -1; dr <= 1; ++dr) {
        int new_row = r + dr;
        if (new_row >= 0 && new_row < BOARD_SIZE) {
            for (int dc = -1; dc <= 1; ++dc) {
                int new_col = c + dc;
                if (new_col >= 0 && new_col < BOARD_SIZE && !visited[new_row][new_col]) {
                    generate_words_help(new_row, new_col, current);
                }
            }
        }
    }
    visited[r][c] = false;
    return;
}

// If the user wants to input a particulare starting board, we accept the input here.
bool Boggle::generate_user_board(string input)
{
    if (input.size() == 16) {
        for (auto c : input) {
            // Check if characters in the input is NOT in A-Z
            if (c < 'A' || c > 'Z') {
                return false;
            }
        }
    } else {
        return false;
    }

    // if it was valid input, genereate the board.
    int i = 0;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            board[r][c] = input[i++];
            visited[r][c] = false;
        }
    }

    return true;
}

// adds a word to the users list if it is valid.
bool Boggle::add_user_word(string input)
{
    //if (computer_set.contains(input)) { // This is much better, but we are not allowed to use it.
    if (input.size() >= 4 && lex.contains(input)  && is_posible(input)) {
        user_set.add(input);
        computer_set.remove(input);
        user_points += input.size() - 3;
        return true;
    } else {
        return false;
    }


}

// counts the words in the users list
int Boggle::get_user_word_count() const
{
    return user_set.size();
}

// get the users word list as a string
string Boggle::get_user_words()
{
    return user_set.toString();
}

int Boggle::get_user_points() const
{
    return user_points;
}

int Boggle::get_computer_word_count() const
{
    return computer_set.size();
}

int Boggle::get_computer_points() const
{
    int ret = 0;
    for (auto s : computer_set) {
        ret += s.size() - 3;
    }
    return ret;
}

// get the computers word list as a string.
string Boggle::get_computer_words()
{
    return computer_set.toString();
}


////////////////////////////////////////////////////////////

// is it possible to build the word by traversing the board?
bool Boggle::is_posible(string input)
{
    bool ret = false;
    // start from all possitions on the board, and check if we can build the word from there.
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            is_posible_help(ret, r, c, input);
            // If we find a true result, return true
            if (ret) return true;
        }
    }
    return ret;
}

// help function to the above, tries to build the words by traversing the board
// stops a branch if it is impossible and continues done a correct one
// returns true if the word can be built from the board, false otherwise.
void Boggle::is_posible_help(bool& pos, int r, int c, string input)
{

    if (input.size() == 1 && input[0] == board[r][c].at(0)) {
        pos = true;
        return;
    }
    if (input.empty() || pos || input.at(0) != board[r][c].at(0)) {
        return;
    } else {
        input.erase(0, 1);
    }

    visited[r][c] = true;

    // Is it possible to walk the word on the board?
    for (int dr = -1; dr <= 1; ++dr) {
        int new_row = r + dr;
        if (new_row >= 0 && new_row < BOARD_SIZE) {
            for (int dc = -1; dc <= 1; ++dc) {
                int new_col = c + dc;
                if (new_col >= 0 && new_col < BOARD_SIZE && !visited[new_row][new_col]) {
                    is_posible_help(pos, new_row, new_col, input);
                    if (pos) {
                        visited[r][c] = false;
                        return;
                    }
                }
            }
        }
    }
    visited[r][c] = false;
    return;

}


