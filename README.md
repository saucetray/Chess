# Chess Engine for Play

This is a Chess Engine developed in C. It is going to get a huge clean-up soon in terms of code and build-system. 
I am currently moving the entire project over to a CMAKE system and cleaning up the code for performance and maintainability.
To further this goal, some code will also be updated to C++. This will become a multi-staged program.

## About

I decided to create a chess engine using bitboards for performance and memory in mind.

### Compile (Soon Outdated)

```
cd src
./build production
./client_run
```

### Some New Updates To Come

#### AI Player (Bot)
I am planning on adding a bot that you can play again using some sort of learning algorithm. 
It will learn and update itself as it plays. 
I will probably use some sort of AdaBoost or Decision Tree algorithm

#### Encrypted P-2-P
An encrypted peer to peer system for playing games. 
Will include a checksum check to make sure there is no cheating or modified code.

#### Stats and Users
Users will be stored in a database along with a password hash. The game will then support stats and looking up players.
