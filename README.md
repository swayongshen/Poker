# Poker

This is a personal project to create an OOP constructed C++ based poker game.

## Goals:
1. Finish terminal-based local mode. [x]
1. Add networking component to enable multiplayer mode. [x]
1. Maybe implement GUI using something simple like SFML. []

## Issues:
1. Poker hand comparison was suprisingly tedious to program even though it is easy to see who is the winner in a real life poker game.
1. Threading was unexpectedly difficult to deal with. Had to learn to deal with concurrency in shared variables.
1. Understanding variables that could not be copied and had to be handled differently such as using std::move and wrapping
some variables in std::unique_ptr.
1. Difficulties in implementing timeouts for user input and syncing it with server. Ended up removing timeouts.
