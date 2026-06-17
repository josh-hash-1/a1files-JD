# a1files-JD

A collaborative **Compilers** class project by Josh and Daryl, built in C and organized with CMake for development, testing, and iteration.

## Overview

This repository contains an ongoing school project focused on compiler-related programming concepts. The codebase is split into coding and reading stages, with supporting headers, test files, and sample input/output artifacts used throughout development.

## Project Goals

- Practice core compiler-course programming concepts in C.
- Build and test multi-step file processing workflows.
- Organize the project using reusable source files and headers.
- Maintain a clear structure for class development and future assignment updates.

## Repository Structure

```text
.
├── code/
│   ├── CMakeLists.txt
│   ├── Compilers.c
│   ├── Compilers.h
│   ├── Main1Coder.c
│   ├── Main2Reader.c
│   ├── Step1Coder.c
│   ├── Step1Coder.h
│   ├── Step2Reader.c
│   ├── Step2Reader.h
│   ├── Big.sof
│   ├── Hello.jd
│   ├── CODED.txt
│   ├── DCODED.txt
│   ├── DECODED.txt
│   ├── HCODED.txt
│   └── launch.vs.json
└── input/
    ├── CODED.txt
    ├── ORIGINAL.txt
    ├── README.txt
    └── TEST.txt
```

## Key Files

| File | Purpose |
|------|---------|
| `Compilers.c` / `Compilers.h` | Shared logic and declarations used across the project. |
| `Main1Coder.c` | Main program for the coding stage. |
| `Main2Reader.c` | Main program for the reading/decoding stage. |
| `Step1Coder.c` / `Step1Coder.h` | Implementation for the first coding step. |
| `Step2Reader.c` / `Step2Reader.h` | Implementation for the second reading step. |
| `CMakeLists.txt` | Build configuration for compiling the project with CMake. |
| `input/` | Input and test files used during development. |
| `CODED.txt`, `DECODED.txt`, etc. | Generated or reference output files for checking results. |

## Build Instructions

This project uses CMake. A standard build process looks like this:

```bash
mkdir build
cd build
cmake ../code
cmake --build .
```

If your course setup expects a different build path, update the CMake command to match the location of `CMakeLists.txt`.

## Running the Project

After building, run the generated executable for the part of the project you want to test.

Typical workflow:

1. Compile the project with CMake.
2. Run the coder program to process input data.
3. Run the reader program to inspect or decode output.
4. Compare generated results with the provided sample files.

## Development Notes

- This is an active class project, so the repository may change as new assignment requirements are added.
- Sample files in `input/` and `code/` help verify output and debug program behavior.
- The structure is designed to keep implementation files separated by stage for easier maintenance.

## Authors

- Josh
- Daryl

## Course Context

This repository is part of an ongoing school project for a Compilers course.