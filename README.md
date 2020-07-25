# Reversi AI

I created an AI that looks through possible future states of the game to make a decision.

## Navigation

* [GUI Reversi](https://github.com/mattlourenco27/Reversi-AI/tree/master)
* [Running the program](##RunningTheProgram)
* [Compiling if Necessary](##CompilingifNecessary)

## Running The Program

To run the console version of the reversi AI travel to the `/exec/` folder in the repository and run `./console_reversi`.
This compiled version is for unix at the moment.

## Compiling if Necessary

To recompile the program navigate to the home directory of the repository in your console and run the command
`g++ main.cpp ai.cpp board.cpp boardAnalysis.cpp -o ./exec/console_reversi`.