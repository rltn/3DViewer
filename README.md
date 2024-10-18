# 3DViewer

A simple 3D model visualizer built with Qt6 and OpenGL.

## Table of Contents

- [Introduction](#introduction)
- [Dependencies](#dependencies)
- [Building](#building)
  - [Using Make](#using-make)
  - [Using Qt Creator](#using-qtcreator)
- [Make Targets](#make-targets)
  - [Default Targets](#default-targets)
  - [Additional Targets](#additional-targets)
- [Installation](#installation)
- [Usage](#usage)
- [Documentation](#documentation)
- [Testing](#testing)
- [Code Quality](#code-quality)
- [Memory Leaks](#memory-leaks)
- [Contributing](#contributing)
- [License](#license)

## Introduction

3DViewer is a straightforward application designed to visualize 3D models using Qt6 and OpenGL. It provides a user-friendly interface for loading and viewing 3D models.

## Dependencies

To build and run 3DViewer, you need the following dependencies:

- **Qt6**: Ensure that Qt6 is installed on your system.
- **OpenGL**: The application uses OpenGL for rendering.
- **Doxygen** (optional): Required for generating documentation.
- **Valgrind** (optional): Required for checking memory leaks.
- **Clang-Format** (optional): Required for code formatting.

## Building

### Using Make

To build the project, simply run the following command in your terminal:

```bash
make
```

Ensure that Qt6 is installed on your system and that the Qt binaries are included in your `PATH`.

### Using Qt Creator

To build the project using Qt Creator, follow these steps:

1. Open Qt Creator.
2. Load the project file located at `src/3DViewer.pro`.
3. Build the project manually from within Qt Creator.

## Make Targets

### Default Targets

- **`all`**: Builds the application.
- **`clean`**: Deletes the build files.
- **`install`**: Installs the application to the directory specified in the `PATH_INSTALL` variable. **Warning: Do not use this target before specifying the install path.**
- **`uninstall`**: Removes the directory specified in the `PATH_INSTALL` variable. **Warning: Do not use this target before specifying the install path.**
- **`dist`**: Creates a distribution tar file.
- **`html`**: Builds HTML documentation for the project using `doxygen`.
- **`tests`**: Builds backend tests.

### Additional Targets

- **`gcov_report`**: Generates an lcov report of the tests run.
- **`clang-format`**: Applies the linter style to the code using `clang-format`.
- **`leaks`**: Checks for memory leaks using `valgrind`.

## Installation

Before using the `install` or `uninstall` targets, ensure that you have specified the `PATH_INSTALL` variable in your environment. This variable determines the installation path for the application.

## Usage

Once the application is built and installed, you can run it to load and view 3D models. The interface provides options to rotate, scale, and translate the model.

## Documentation

To generate HTML documentation for the project, use the `html` target:

```bash
make html
```

This will create documentation using `doxygen`.

## Testing

To build and run backend tests, use the `tests` target:

```bash
make tests
```

For a detailed coverage report, use the `gcov_report` target:

```bash
make gcov_report
```

## Code Quality

To apply the linter style to your code, use the `clang-format` target:

```bash
make clang-format
```

This will format your code according to the specified style guidelines.

## Memory Leaks

To check for memory leaks in your application, use the `leaks` target:

```bash
make leaks
```

This will run your application with `valgrind` to detect any memory leaks.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.