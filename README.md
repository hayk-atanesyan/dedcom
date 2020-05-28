# DEDCOM (dedcom)

This is a failed gamejam project. It contains some very bad code, please do not replicate it.

--------------------------------------------------------------------------------

This is an incomplete turn based limited information attrition game built around client/server interaction utilizing boost::asio. Its main goal was to test/learn boost::asio. The project showcases an example of how to create/package/send/receive/unpack commands, most of which use strings just because i was in a hurry but there's one with an integer. The program uses some outdated practices with raw memory management and coding, including gigantic switch statements. This is mainly for flavour/showcase (the memory part) and because I failed to make better code under time constraints (the spaghetti code). This should not be seen as a good example of how to do things.

# How to play

DEDCOM is a game about remotely controlling a party of exploration drones in hostile environment. it is played through console (type help for available commands). You need to launch the server and the client, then proceed to type in the client console. The client will attempt to connect to localhost for ease of use, but this can be changed manually to any other address.
Main game loop consists of exploring new locations, resolving events if any, resolving encounters if any and delving as deep as possible while suffering from constant attrition. This, however, is NOT fun, since the project is incomplete and there is little balance in those interactions. This is, above all else, a public monument to my shame.

# Warning

The project utilizes multithreading, however the multi-client part was left incomplete (or rather nonexistant), so attempting to connect with multiple clients or sending multiple commands before receiving a response from server will most likely cause issues.

# How To Build

Project is set up for CMake, Doxygen and Boost Unit Testing (no tests provided).

```bash

cd build
cmake ..
make

./dedcom_server
./dedcom_client

```
