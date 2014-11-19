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

void Boggle::reset()
{
    user_points = 0;
    user_set.clear();
    computer_set.clear();
}

void Boggle::generate_words()
{
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            generate_words_help(r, c, "");
        }
    }
}

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


bool Boggle::generate_user_board(string input)
{
    //cerr << "\n!!" << input << "!!\n";
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

    int i = 0;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            board[r][c] = input[i++];
            visited[r][c] = false;
        }
    }

    return true;
}

bool Boggle::add_user_word(string input)
{
    //if (computer_set.contains(input)) { // This is much better, but we are not allowed to use it.
    if (lex.contains(input)  && is_posible(input)) {
        user_set.add(input);
        computer_set.remove(input);
        user_points += input.size() - 3;
        return true;
    } else {
        return false;
    }


}

int Boggle::get_user_word_count() const
{
    return user_set.size();
}

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

string Boggle::get_computer_words()
{
    return computer_set.toString();
}


////////////////////////////////////////////////////////////

bool Boggle::is_posible(string input)
{
    bool ret = false;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            is_posible_help(ret, r, c, input);
            if (ret) return ret;
        }
    }

    return ret;

}

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


