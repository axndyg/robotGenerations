# robotGenerations
Submitted final for my Object-Oriented Programming Class. 

## About
A replication of Harvard University 1968 experiment dealing with artificial intelligence tested through the use of the `C` language. The experiment demonstrated the evolution of artificial intelligence by favoring robots developed with better equipped attributes. This project is a direct reproduction of the experiment done in 1968 with the lanugage `C++`. 

### Robot's and Maps 
A robot's goal is to survive. A robot will be placed randomly in a `10x10` grid with `40` of those tiles filled with squares. A robot will attempt to navigate this map consuming energy per turn. A robot picking up a battery will gurantee it five more turns. 

Robots are established in the first generation with a random set of genes. These genes correspond to sensors that a robot will report at any given moment on the map. Upon finding a match, a Robot will execute movement in the directions its genes were coded with. The generatations after the first are no longer random as the top `50%` scorers will breed and fill the lower `50%`. As of now, the program is hard fixed to reproduce a population of `200` robots but can easily be adjusted in the future. 

## Notices and Errors
***The program runs fine with basic compilers. Some compilers throw issues with the way raw string data has been formatted in my code. As well, warnings may raise from strict / exclusive `C++ 11` features and `C++ 11` extensions. To run seamlessly, replit is an issueless compiler for this code.*** 

### Crediting
All code is written by myself, with instruction and knowledge from my professor. All asci art found in the code are from the ASCI Library. 
