# VirtSpy
A lightweight C library for detecting virtual machine environments and retrieving hypervisor vendor information using &lt;cpuid.h> and inline assembly with the CPUID instruction.

# VirtSpy Library Documentation

## Table of Contents
1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Usage](#usage)
4. [API Reference](#api-reference)
5. [Examples](#examples)
6. [Contributing](#contributing)
7. [License](#license)

## Introduction
The VirtSpy Library provides functions to detect whether the current system is running inside a virtual machine and retrieve the hypervisor vendor information. This library uses inline assembly to execute the `CPUID` instruction and analyze the results.

## Installation

### Prerequisites
- GCC or any compatible C compiler.
- Basic knowledge of C and Linux commands.

### Building the Library
1. **Clone the repository** (if applicable):
   ```bash
   git clone https://github.com/hotwrist/VirtSpy.git
   cd VirtSpy
   ```

2. **Compile the Library**:
   - **Static Library**:
     ```bash
     gcc -c virt_spy.c -o virt_spy.o
     ar rcs libvirt_spy.a virt_spy.o
     ```

   - **Dynamic Library**:
     ```bash
     gcc -shared -fPIC virt_spy.c -o libvirt_spy.so
     ```

3. **Install System-wide**:
   ```bash
   sudo mv libvirt_spy.a /usr/local/lib/
   sudo mv libvirt_spy.so /usr/local/lib/
   sudo ldconfig
   ```

## Usage

### Linking the Library
- **Static Library**:
  ```bash
  gcc main.c -L/usr/local/lib -lvirt_spy -o vm_check
  ```

- **Dynamic Library**:
  ```bash
  gcc main.c -L/usr/local/lib -lvirt_spy -o vm_check
  ```

### Include the Header File
In your C program, include the library's header file:
```c
#include "virt_spy.h"
```

## API Reference

### `int detected_vm()`
**Description**: Checks if the current system is running inside a virtual machine.

**Returns**:
- `1` if a VM is detected.
- `-1` if no VM is detected.

### `const char *get_hypervisor_vendor()`
**Description**: Retrieves the hypervisor vendor string.

### `int detect_nested_virtualization()`
**Description**: Detects if we have a nested virtualization.

**Returns**:
- `2` if nested virtualization is detected.
- `-2` if no nested virtualization is detected.

### `int detect_virtual_network_devices()`
**Description**: Detects if we have virtual network devices.

**Returns**:
- `3` if virtual network devices is detected.
- `-3` if no virtual network devices is detected.

### `int detect_virtual_block_devices()`
**Description**: Detects if we have virtual block devices.

**Returns**:
- `4` if virtual block devices is detected.
- `-4` if no virtual block devices is detected.

### `int check_kernel_modules()`
**Description**: Detects if we have virtual kernel modules.

**Returns**:
- `5` if virtual kernel module is detected.
- `-5` if no virtual kernel module is detected.

### `int detect_virtual_pci_devices()`
**Description**: Detects if we have virtual PCI devices.

**Returns**:
- `6` if virtual network devices is detected.
- `-6` if no virtual PCI devices is detected.

## Examples

### Example 1: Check for VM Presence
```c
#include <stdio.h>
#include "virt_spy.h"

int main() {
    if (detect_vm() == VM_DETECTED) {
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
#include "virt_spy.h"

int main() {
    const char *vendor = get_hypervisor_vendor();
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
This project is licensed under the GNU General Public License. See the [LICENSE](LICENSE) file for details.

