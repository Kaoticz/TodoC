# TodoC

This repository contains a university assignment for a simple "To-Do" program written in C. The program performs CRUD operations with strings in a SQLite database.

## How to use

To build the program, navigate to the root directory and execute the following command:

```
make
```

The build binaries will be located in the `obj/` directory, and the final binary can be found in the `bin/` directory. To run the program, execute the following command:

```
./bin/main
```

The application connects to an SQLite database in the same location where it is being executed. If it does not find an existing database, a new one is created.

To delete all binaries and clean the project, execute:

```
make clean
```

## Details

While this program may be considered overengineered for its purpose, it effectively showcases various features of the C language, including but not limited to:

- Declaration and usage of both local and global variables.
- Implementation of control flow structures in accordance with the Structured Programming paradigm (i.e., the use of goto is prohibited).
- Utilization of pointers.
- Implementation of function pointers and callbacks.
- Manual memory management.
- Utilization of variadic functions and macros.
- Signal interception and handling, providing an event-like behavior.

Additionally, the program demonstrates:

- Integration with a relational database (SQLite).
- Inclusion of a Makefile for streamlined compilation.
- An object-oriented-like structure for source files, with "private" members declared using the static modifier and "public" members declared in the header file.

## License
TodoC, a terminal-based "to-do" application written in C.

Copyright (C) 2023 Kotz

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.