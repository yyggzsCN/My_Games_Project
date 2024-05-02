
Compile the `main.cpp` file using the following command
```bash
clang++ -o main main.cpp -std=c++17 -Wall $(pkg-config --libs --cflags raylib) -framework IOKit -framework Cocoa -framework OpenGL
```
then run the compiled file.