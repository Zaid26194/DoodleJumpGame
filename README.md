# DoodleJumpGame
This was a C++ project I did for class during December 2017. The purpose was to use the engine made by our instructor, read and understand
how it works, to code our own version of Doodle Jump. 

Most of my coding was done in the "doodlejump.cpp" and "gamerenderwidget.cpp" files. In the latter, I handled Key Press as well as 
Release Events.

In "doodlejump.cpp", I handled game logic like, what happens when the Doodler comes into contact with a platform or an enemy 
or falls below the screen. Also, dealt with random generation of the platforms and enemy Doodlers. In addition to that, I did the 
camera movement and firing bullets. I also did custom sprites for the bullets and the death state for the Doodler. All of that comes
together in the "tick()" function which is called every frame.

You will need to install QT to run the code.

Known Issues: While the game works perfect on Windows, I tested it on a Mac that I had available and the platforms would all spawn to one side
only. I believe the problem is RAND_MAX but I could be wrong.
