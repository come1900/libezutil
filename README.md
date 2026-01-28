# libezutil

## Description

libezutil is a lightweight C/C++ utility library that provides common functions including encryption/decryption, encoding/decoding, CRC checksums, file operations, and network protocol parsing. It addresses the common utility function needs in embedded systems and server development, featuring cross-platform support, no external dependencies, and easy integration.

## Quick Start

### Requirements

- C99 or higher compiler (GCC 4.8+ / Clang 3.3+)
- Make build tool

### Installation

```bash
make
make install
```

The library files will be installed to `$(HOME)/libs/lib/`, and header files will be installed to `$(HOME)/libs/include/ezutil/`.

## Usage

Include the corresponding header files in your project and link the library as needed, for example:

```c
#include <ezutil/ez_log.h>
#include <ezutil/str_opr.h>
...
```

Add link options when compiling: `-L$(HOME)/libs/lib -lezutil-$(PLATFORM)`

## License

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
