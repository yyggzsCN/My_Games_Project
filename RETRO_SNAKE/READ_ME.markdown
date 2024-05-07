
Compile the `main.cpp` file using the following command
```bash
clang++ -o main main.cpp -std=c++17 -Wall $(pkg-config --libs --cflags raylib) -framework IOKit -framework Cocoa -framework OpenGL
```
then run the compiled file.

This creates a macOS executable. To distribute your game on macOS, you may want to package it into an app bundle or use a tool like create-dmg to make a distributable DMG file.