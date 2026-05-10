# My_zsh - simple Unix-like shell

A minimal command interpreter implemented in C for learning purposes.

## Overview

This project implements a small shell that reads lines from standard input,
parses them into a command and arguments, and executes either built-in
commands or external programs found in `PATH`.

Key built-ins: `cd`, `echo`, `env`, `setenv`, `unsetenv`, `exit`/`quit`.

## Build

Requirements: GCC and GNU Make (or build manually with `gcc`).

Using Make (recommended, tested under WSL/Linux):

```bash
make
./my_zsh
```

Manual compilation:

```bash
gcc -I zsh/h zsh/c/*.c -o my_zsh
./my_zsh
```

## Usage

The shell displays a prompt with the current directory, e.g.:

```
[/path/to/dir]>
```

Type a command and press Enter. Example session:

```
[/home/user/project]> echo Hello
Hello
[/home/user/project]> pwd
/home/user/project
[/home/user/project]> cd ..
[/home/user]> pwd
/home/user
[/home/user]> exit
```

## Built-in commands

- `cd <dir>`: change working directory. `cd -` swaps to the previous directory.
- `echo <text>`: print text; supports expanding environment variables when prefixed with `$` (e.g. `echo $HOME`).
- `env`: print the current environment variables.
- `setenv KEY=VALUE` (or `setenv KEY VALUE` in some cases): set an environment variable (note: implementation has known memory handling issues in some cases).
- `unsetenv KEY`: remove an environment variable.
- `exit` / `quit`: exit the shell.

External commands (e.g. `ls`, `which`) are executed by searching `PATH`.

## Notes and known issues

- `setenv` handling can cause a memory error in some input forms (observed when parsing `setenv TEST=123`). Use with care.
- This shell is intended for education and is not hardened for production use.

## Examples

Start the shell and run a few commands non-interactively (useful for quick tests):

```bash
# print working dir and exit
printf "pwd\nexit\n" | ./my_zsh

# set a var, list env, then exit (use with caution)
printf "setenv TEST=123\nenv\nexit\n" | ./my_zsh
```

## License

See the `LICENSE` file.

## Author

Akbar Usmonov
