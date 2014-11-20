// anden259 andno037
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"
#include "lexicon.h"

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle)
{
    string input;

    boggle.reset();
    if (yesOrNo("Do you want to generate a random board? ")) {
        boggle.generate_board();
    } else {
        do {
            cout << "Enter a line of 16 chars from A-Z:\n";
            getline(cin, input);
        } while (!boggle.generate_user_board(input));

    }
    boggle.generate_words();

    //cout << boggle.get_computer_words() << endl; //top display what words the computer found.

    while (true) {

        cout << "It's your turn!" << endl;
        cout << boggle.board_string() << endl;

        getline(cin, input);
        //clearConsole();
        if (input.size() == 0) {
            break;
        }
        if (!boggle.add_user_word(input)) {
            cout << "Can't add word " << endl;
        }
        cout << "Your words (" << boggle.get_user_word_count() << "): " << boggle.get_user_words() << "\n";
        cout << "Your score: " << boggle.get_user_points() << endl;
    }

    cout << "It's my turn!" << endl;
    cout << "My words (" << boggle.get_computer_word_count() << "): " << boggle.get_computer_words() << endl;
    cout << "My score: " << boggle.get_computer_points() << endl;

    if (boggle.get_computer_points() > boggle.get_user_points()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!\n";
    } else {
        cout << endl;
    }
}

/*
 * Erases all currently visible text from the output console.
 */
void clearConsole()
{
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("clear");
#endif
}
