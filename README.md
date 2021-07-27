# opengl-cube
This is just a simple example in [OpenGL](https://www.opengl.org/) with a cube which rotates on its y-axis.

## Building
This is a [cmake](https://cmake.org/) project so cmake must be installed to compile this project.

First, create a `build` directory and change into it.
```bash
mkdir -v build && cd build
```

Then, run cmake over the parent directory as it contains the `CMakeLists.txt` project file.
```bash
cmake ..
```

Now, simply run `make` to create the final program executable. The final executable `cube` will get created in the same `build` directory.
```bash
make
```

## Running
Execute the executable `cube` to run the calculator.
```bash
./cube
```

## License
This project is under [MIT License](LICENSE)
