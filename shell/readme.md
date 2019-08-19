# Usage #

Both the shell and unit test binaries can be compiled with make:

```bash
make clean
make shell
./shell
```

```bash
make clean
make test
./unit
```

## Restrictions ##

This shell implementation currently only supports single commands (cannot chain commands with ';'),
and assumes there is appropriate spacing (some white space) between
command arguments (e.g. "echo abc > x.txt will work, but "echo abc>x.txt will not)*

```
exit --> exit the terminal

cd <DIR> --> change to specified DIR, go to $HOME otherwise

<CMD> > <FILE>  --> overwrites specified file with result of command

<CMD> >> <FILE> --> appends result of command to specified file

<CMD> | <CMD2> --> pipes CMD output to CMD2 input
```

> In addition to the above implemented functionality, this shell generally supports any bash commands
> that rely on underlying binaries (e.g. ls, pwd, cat, echo etc.)
