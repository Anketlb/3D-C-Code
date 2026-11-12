# Prerequisites
- Install C++
  - You should be able to run the following in a terminal and get the cmake version: 
    - Windows: `g++ --version`
    - MacOS: `clang --version`

- Install VS Code [Optional]
  - Install [C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
 

# Build on Windows
`g++ main.cpp src/glad.c -o ./bin/OpenGLApp.exe -I./dependencies/include -L./dependencies/lib/win_x64 -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32`

# Build on MacOS
`clang++ main.cpp src/glad.c -o ./bin/OpenGLApp -std=c++17 -I./dependencies/include ./dependencies/lib/mac_arm64/libglfw3.a -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo`


