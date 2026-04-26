# willow

Config library in C. This is, above all, a personnal project to be used in other projects.

## Structure

File structure. (Mostly reference for myself)

```
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

```
willow/
├── sys/ # Private utils package for system functions
├── io/ # Private utils package
├── test/ # Private utils package
├── err/ # Error management for the library
├── conf/ # Setup for the library
└── willow.h # Type definitions
```

## Using the Willow Configuration Language (WCL)

Files using the Willow Configuration Language (WCL) end with the `.wcl` extension.

Here is a short code sample written using the `WCL`:
```wcl
__WIL_VERSION = "1.0.0";

NUMBER_OF_ROWS = 2;

_user_name = env("USER");

if (_user_name == "me") {
  NUMBER_OF_ROWS = 5;
}
```

### Naming convention

Willow configuration entries are in fullcaps with a preceeding double underscore (`__`).

Configuration entries for the program to be configured are in fullcaps (and should be preceeded with an abbreviation of the program name).

Local variables in the WCL code should be lowercase with a preeceeding underscore (`_`).
