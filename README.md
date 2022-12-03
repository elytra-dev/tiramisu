# Tiramisu
## Version 0.0.1 (Pre-release)
Tiramisu is an open-source chat application.

### Requirements
- Windows SDK/Visual Studio/Header files mentioned here.
- Basic TCP/IP port connection knowledge.
- Basic compiling, executing and C++ knowledge.
- Windows 10 or above. (Linux and other platforms not tested.)

### How to use
1. Go download `client.cpp` and `server.cpp`, open a project in VS or make a folder named `Client` and `Server`. 
2. Place the cpp files in their respective folders. 
3. If you're creating a folder, make sure to download the header files and link them. 
4. In Visual Studio, press `Ctrl + F5` to run the program, always start by running the `Server.cpp` file first, then proceed on to `Client.cpp`.

After compiling, start with typing in Client's window, Both server and client cannot chat simultaneously, atleast 1 has to listen (`recv()`) while other sends (`send()`).
Change IP/PORT to allow access from other devices. (Adding functionality later).


> This is a basic beginner project, I created a community to not only help other newbie Winsock developers but also get help from experienced one. If you're anywhere from "I am interested" to "I am experienced", feel free to join!
https://discord.gg/7ZFGRYxzbP
