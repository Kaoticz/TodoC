# TodoC

University assignment for a simple "To Do" program written in C. It performs CRUD operations with strings in a SQLite database.

## How to use
In the root directory, execute:
```
make
```
The build binaries are placed at `obj/`
The final binary is placed at `bin/`, which can be executed directly.

```
./bin/main
```
The application connects to a SQLite database in the same location it is being executed. If it doesn't find one, a new database is created.

To delete all binaries, execute:
```
make clean
```

## Details

This program is overengineered for its purpose, but it showcases very well most (if not all) features in the C language. Including, but not limited to:
- Local and global variables.
- Use of control flow structures in accordance to the Structured Programming paradigm (ie. `goto` not allowed).
- Pointers.
- Function pointers and callbacks.
- Manual memory management.
- Variadic functions and macros.
- Signal interception/handling.

Additionally, there is also:
- Use of a relational database.
- A Makefile.
- Object oriented-like structure for source files.
    - `static` modifier for "private" members.
    - "Public" members declared in the header file.