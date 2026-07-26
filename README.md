# willow

Config library in C. This is, above all, a personnal project to be used in other projects.

## Structure

File structure. (Mostly reference for myself)

```sh
willow/
├── src/
│   ├── willow/
│   │   └── ... # See next section
├── include/
│   ├── willow/
│   │   └── ... # See next section
├── tests/
│   ├── CMakeLists.txt
│   ├── test_foo.c
│   ├── test_bar.c
│   └── ...
├── CMakeLists.txt
├── Doxyfile
└── ...
```

Module structure.

```sh
willow/
├── conf/ # Setup for the library
├── io/ # Private utils package
├── lexer/ # Package for lexical analysis
├── sys/ # Private utils package for system functions
└── willow.h # Type definitions
```

## The Willow Configuration Language (WCL)

Files using the Willow Configuration Language (WCL) end with the `.wcl` extension.

Here is a code sample written using `WCL`:
```c
string __WIL_VERSION = "1.0.0";

int NUMBER_OF_ROWS = 2;

string _user_name = env("USER");

if (_user_name == "me") {
  NUMBER_OF_ROWS = 5;
}
```

The syntax and philosophy is based on C.

### Naming convention

Willow configuration entries are in fullcaps with a preceeding double underscore (`__`).

Configuration entries for the program to be configured are in fullcaps (and should be preceeded with an abbreviation of the program name).

Local variables in the WCL code should be lowercase with a preeceeding underscore (`_`).

## Cherries

Cherries are the extra features I want when this is done. Things that will make me smile (〃＾▽＾〃)

- Support for radices: hex, bin, oct (keep the 0 prefix? idk)
- Support for escape sequences in strings
