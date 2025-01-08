/* AUTHOR: John Ebinyi Odey
 * DESCRIPTION: This piece of program  (VirtSp) detects if a malware or a benign program is running inside a VM.
 * DATE: Wednesday, January 08th, 2025
*/

#ifndef VIRT_SPY_H
#define VIRT_SPY_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VM_DETECTED 1
#define NO_VM_DETECTED -1

#define NESTED_VTX_DETECTED 2
#define NO_NESTED_VTX_DETECTED -2

#define VTX_NW_DEV_DETECTED 3
#define NO_VTX_NW_DEV_DETECTED -3

#define VTX_BLK_DEV_DETECTED 4
#define NO_VTX_BLK_DEV_DETECTED  -4

#define VTX_KERN_MODS_DETECTED 5
#define NO_VTX_KERN_MODS_DETECTED -5

#define VTX_PCI_DEV_DETECTED 6
#define NO_VTX_PCI_DEV_DETECTED -6

// The below function helps to detect if the program is running inside a VM...
int detect_vm(void);

// This detects the processor vendor informaiton of the CPU.
const char *get_hypervisor_vendor(void);

// If there is any form of nested virtualization, this is going to be detected.
int detect_nested_virtualization(void);

// If there is any virtual network devices, this is going to be detected.
int detect_virtual_network_devices(void);

// If there is any virtual block devices, this is going to be detected.
int detect_virtual_block_devices(void);

// If there is any form of nested virtualization, this is going to be detected.
int check_kernel_modules(void);

// If there is any virtual PCI devices, this is going to be detected.
int detect_virtual_pci_devices(void);

#ifdef __cplusplus
}
#endif

#endif // VIRT_SPY_H
