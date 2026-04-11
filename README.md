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
