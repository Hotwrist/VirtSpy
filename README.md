# VirtSpy
A lightweight C library for detecting virtual machine environments and retrieving hypervisor vendor information using &lt;cpui.h> and inline assembly with the CPUID instruction.

# VM Detection Library Documentation

## Table of Contents
1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Usage](#usage)
4. [API Reference](#api-reference)
5. [Examples](#examples)
6. [Contributing](#contributing)
7. [License](#license)

## Introduction
The VM Detection Library provides functions to detect whether the current system is running inside a virtual machine and retrieve the hypervisor vendor information. This library uses inline assembly to execute the `CPUID` instruction and analyze the results.

## Installation

### Prerequisites
- GCC or any compatible C compiler.
- Basic knowledge of C and Linux commands.

### Building the Library
1. **Clone the repository** (if applicable):
   ```bash
   git clone https://github.com/yourusername/vm_detect_lib.git
   cd vm_detect_lib
   ```

2. **Compile the Library**:
   - **Static Library**:
     ```bash
     gcc -c vm_detect.c -o vm_detect.o
     ar rcs libvm_detect.a vm_detect.o
     ```

   - **Dynamic Library**:
     ```bash
     gcc -shared -fPIC vm_detect.c -o libvm_detect.so
     ```

3. **Install System-wide**:
   ```bash
   sudo mv libvm_detect.a /usr/local/lib/
   sudo mv libvm_detect.so /usr/local/lib/
   sudo ldconfig
   ```

## Usage

### Linking the Library
- **Static Library**:
  ```bash
  gcc main.c -L/usr/local/lib -lvm_detect -o vm_check
  ```

- **Dynamic Library**:
  ```bash
  gcc main.c -L/usr/local/lib -lvm_detect -o vm_check
  ```

### Include the Header File
In your C program, include the library's header file:
```c
#include "vm_detect.h"
```

## API Reference

### `int is_vm_detected()`
**Description**: Checks if the current system is running inside a virtual machine.

**Returns**:
- `1` if a VM is detected.
- `0` if no VM is detected.

### `void get_hypervisor_vendor(char *vendor, size_t length)`
**Description**: Retrieves the hypervisor vendor string.

**Parameters**:
- `char *vendor`: A buffer to store the vendor string.
- `size_t length`: The length of the buffer. Should be at least 13 bytes.

**Notes**:
- The `vendor` buffer must be large enough to hold the 12-character vendor string plus a null terminator.

## Examples

### Example 1: Check for VM Presence
```c
#include <stdio.h>
#include "vm_detect.h"

int main() {
    if (is_vm_detected()) {
        printf("VM Detected\n");
    } else {
        printf("No VM Detected\n");
    }
    return 0;
}
```

### Example 2: Get Hypervisor Vendor
```c
#include <stdio.h>
#include "vm_detect.h"

int main() {
    char vendor[13];
    get_hypervisor_vendor(vendor, sizeof(vendor));
    printf("Hypervisor Vendor: %s\n", vendor);
    return 0;
}
```

## Contributing
1. Fork the repository.
2. Create a new branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add your feature"
   ```
4. Push to the branch:
   ```bash
   git push origin feature/your-feature-name
   ```
5. Create a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

