#include <iostream>
#include <string>
#include "core/repr/MoveGeneration.h"
#include "core/repr/Board.h"
#include "core/util/str.h"

// The main board for the program
Board *main_board = nullptr;

// A variable to set whether the program is accepting commands (false -> ends program)
bool accepting_commands = false;

// Input prompt for the engine and other telemetry stuff
const char *INTRO = "Vela v0.0.0 by William Irwin (1.28.23)\n";
const char *INPUT_PROMPT = "ce2023vela >> ";

/**
 * @brief d is the command that prints the current board position
 */
void d() {
    main_board->PrintBoard();
}

/**
 * @brief quit is the function that closes the engine
 */
void quit() {
    accepting_commands = false;
}

void parse_command(const std::string& command) {
    char *cmd_arr = const_cast<char*> (command.c_str());
    char **cmd_parts = util::str::split(cmd_arr);
    // This is hideous - I should change this later
    if (std::strcmp(cmd_parts[0], "d") == 0) {
        d();
    }
}

// Main call from the cmd
// TODO: create a console to take requests from the user instead of having a pre-built test
int main() {

    // Set up the mainBoard object
    main_board = Board::FromFEN(CLASSIC_FEN);

    // TESTING CODE HERE

    // Take inputs from the user
    accepting_commands = true;
    std::cout << INTRO;
    while (accepting_commands) {

        // Get the newest command
        std::string command;
        std::cout << INPUT_PROMPT;
        std::cin >> command;
        parse_command(command);

    }

    return 0;
}
