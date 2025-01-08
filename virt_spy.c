/* AUTHOR: John Ebinyi Odey
 * DESCRIPTION: This piece of program (VirtSpy) detects if a malware or a benign program is running inside a VM.
 * DATE: Wednesday, January 08th, 2025
*/

#include "virt_spy.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#if defined(_MSC_VER)	// For MSVC (Windows)
#include <intrin.h>	// The cpuid MSVC provides is _cpuid
#elif defined(__GNUC__)  // For GCC (Linux or other UNIX-like OSes)
#include <cpuid.h>	// The cpuid GCC provides is __cpuid
#endif

int detect_vm(void)
{
	unsigned int eax, ebx, ecx, edx;
	
	// Setting eax = 1 is a common way of querying the processor for basic information about
	// its features and capabilities. And since we are trying to detect if we are running in a Virtual Environment
	eax = 1;

#if defined(_MSC_VER)
	__cpuid((int*)&eax, eax);
#elif defined(__GNUC__)
	__get_cpuid(eax, &eax, &ebx, &ecx, &edx);
#endif
	/* Inline assembly could be used also
	__asm__ __volatile__ (
		"cpuid"
		: "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
		: "a"(eax)
	);*/

	// Whe eax is set to 1 (as seen above), we check the 31st bit of the ECX register to see if this bit is set.
	// This bit is called the "Hypervisor Present Bit" and if it is set, it means we are in a hypervisor environment.
	return (ecx & (1 << 31)) ? VM_DETECTED : NO_VM_DETECTED;
}

const char* get_hypervisor_vendor(void)
{
	unsigned int eax, ebx, ecx, edx;
	
	// This check ensure the vendor buffer is large enough. Remember this will have a length of 13: EBX, EDX, EBX 
	// 4 + 4 + 4 = 12 plus null-byte = 13. 
	char hypervisor_vendor[13] = {0};
	
	// The value (0x40000000) given to eax below helps us to retrieve the processor vendor's information.
	// When cpuid runs, the processor populates EBX, ECX, and EDX registers with the hypervisor vendor string.
	// EBX ==> contains the low 4-bytes of the string
	// EDX ==> contains the middle 4-bytes of the string
	// ECX ==> contains the last 4-bytes of the string
	/*
	EBX:   31	23	15       7        0  
		 ---------------------------------
		| byte4 | byte3 | byte2 | byte1  |
		---------------------------------
		
	EDX:   31	23	15       7        0  
		 ---------------------------------
		| byte8 | byte7 | byte6 | byte5  |
		---------------------------------
		
	ECX:   31	23	15       7        0  
		 ---------------------------------
		| byte12 | byte11 | byte10 | byte9  |
		---------------------------------
	*/
	eax = 0x40000000;

	/* Inline assembly could be used also
	__asm__ __volatile__ (
		"cpuid"
		: "=b"(ebx), "=c"(ecx), "=d"(edx)
		: "a"(eax)
	);*/

#if defined(_MSC_VER)
	//__cpuid((int*)&eax, eax);
	int cpu_info[4] = {0};
	__cpuid(cpu_info, eax);
	
	*((unsigned int*)&hypervisor_vendor[0]) = cpu_info[1];	// EBX
	*((unsigned int*)&hypervisor_vendor[4]) = cpu_info[3];	// EDX
	*((unsigned int*)&hypervisor_vendor[8]) = cpu_info[2];	// ECX
	hypervisor_vendor[12] = '\0'; // Null-terminate it.
		
	if(strncmp(hypervisor_vendor, "KVM", 3) == 0) return "Running on KVM (Kernel-based Virtual Machine) hypervisor.\n";
	else if(strncmp(hypervisor_vendor, "VMware", 6) == 0) return "Running on VMware Hypervisor.\n";
	else if(strncmp(hypervisor_vendor, "Microsoft Hv", 12) == 0) return "Running on Microsoft Hyper-V.\n";
	else if(strncmp(hypervisor_vendor, "XenVMM", 6) == 0) return "Running on Xen Hypervisor.\n";
	else if(strncmp(hypervisor_vendor, "VBoxVBox", 8) == 0) return "Running on Oracle VirtualBox Hypervisor.\n";
	else return "Unknown Hypervisor.\n";
		
#elif defined(__GNUC__)
	//__get_cpuid(eax, &eax, &ebx, &ecx, &edx);
	__asm__ __volatile__ (
			"cpuid"
			: "=b"(ebx), "=c"(ecx), "=d"(edx)
			: "a"(eax)
	);
	
	memcpy(hypervisor_vendor, &ebx, 4);
	memcpy(hypervisor_vendor + 4, &ecx, 4);
	memcpy(hypervisor_vendor + 8, &edx, 4);

	if(strncmp(hypervisor_vendor, "KVM", 3) == 0) return "Running on KVM (Kernel-based Virtual Machine) hypervisor.\n";
	else if(strncmp(hypervisor_vendor, "VMware", 6) == 0) return "Running on VMware Hypervisor.\n";
	else if(strncmp(hypervisor_vendor, "Microsoft Hv", 12) == 0) return "Running on Microsoft Hyper-V.\n";
	else if(strncmp(hypervisor_vendor, "XenVMM", 6) == 0) return "Running on Xen Hypervisor.\n";
	else if(strncmp(hypervisor_vendor, "VBoxVBox", 8) == 0) return "Running on Oracle VirtualBox Hypervisor.\n";
	else return "Unknown Hypervisor.\n";

#endif
}

int detect_nested_virtualization(void)
{
	unsigned int eax, ebx, ecx, edx;
	
	// when checking for nested virtualization, set eax to the value below:
	eax = 0x8000001E;
	
#if defined(_MSC_VER)
	_cpuid((int*)&eax, eax);
#elif defined(__GNUC__)
	__get_cpuid(eax, &eax, &ebx, &ecx, &edx);
#endif
	if(ecx & (1 << 5)) return NESTED_VTX_DETECTED;
	else return NO_NESTED_VTX_DETECTED;
}

// Virtual machines often use virtual network interfaces such as eth0, ens33, or others, 
// which can be detected.
int detect_virtual_network_devices(void)
{		
	FILE *file_ptr;
	bool virt_nw_detected;
	char path[1035];
	
	// Let's open the /sys/class/net directory to check for network devices
	file_ptr = popen("ls /sys/class/net", "r");
	
	if(file_ptr == NULL) 
	{
		perror("Failed to run command: \'ls /sys/class/net\'\n");
		exit(1);
	}
	
	// Whew! it was successful, let's now read the output of the command we executed above, and look for
	// virtual network device names.
	while(fgets(path, sizeof(path), file_ptr) != NULL)
	{
		if(strstr(path, "eth") || strstr(path, "ens") || strstr(path, "veth")) 
		{
			//printf("Virtual Network Device Detected: %s\n", path); 
			virt_nw_detected = true;
			break;
		}
		else 
		{	
			virt_nw_detected = false;
		}
	}

	if(!virt_nw_detected) 
	{
		// close the file before leaving...
		fclose(file_ptr);
		return NO_VTX_NW_DEV_DETECTED;
	}
	// we are done reading...close it.
	fclose(file_ptr);
	return VTX_NW_DEV_DETECTED;
}

// Virtual machines may also use virtual block devices such as vda, or vdX (for KVM) or xvda (for Xen).
int detect_virtual_block_devices(void)
{	
	FILE *file_ptr;
	bool virt_blk_dv_detected;
	char path[1035];
	
	// Let's open the /sys/block directory to check for blockdevices
	file_ptr = popen("ls /sys/block", "r");
	
	if(file_ptr == NULL) 
	{
		perror("Failed to run command: \'ls /sys/block\'\n");
		exit(1);
	}
	
	// Whew! it was successful, let's now read the output of the command we executed above, and look for
	// virtual block device names.
	while(fgets(path, sizeof(path), file_ptr) != NULL)
	{
		if(strstr(path, "vda") || strstr(path, "xvda")) 
		{
			//printf("Virtual Block Device Detected: %s\n", path); 
			virt_blk_dv_detected = true;
			break;
		}
		else 
		{
			virt_blk_dv_detected = false;
		}
	}

	if(!virt_blk_dv_detected)
	{
		// close the file before leaving...
		fclose(file_ptr);
		return NO_VTX_BLK_DEV_DETECTED; 
	}
	
	// we are done reading...close it.
	fclose(file_ptr);
	return VTX_BLK_DEV_DETECTED;
}

// Hypervisors like KVM, Xen, or VMWare may load specific kernel modules that are unique to virtual environments.
// E.g kvm or kvm-intel (on Intel CPUs), or kvm-amd (on AMD CPUs).
int check_kernel_modules(void)
{
	FILE *file_ptr;
	bool kernel_mod_detected;
	char path[1035];
	
	// Let's open the lsmod command to check for loaded kernel modules
	file_ptr = popen("lsmod", "r");
	
	if(file_ptr == NULL) 
	{
		perror("Failed to run command: \'lsmod\'\n");
		exit(1);
	}
	
	// Whew! it was successful, let's now read the output of the command we executed above, and look for
	// loaded kernel modules.
	while(fgets(path, sizeof(path), file_ptr) != NULL)
	{
		if(strstr(path, "kvm") || strstr(path, "vbox")) 
		{
			//printf("Virtualization-related Kernel Module Detected: %s\n", path);  
			kernel_mod_detected = true;
			break;
		}
		else 
		{
			kernel_mod_detected = false;
		}
	}

	if(!kernel_mod_detected) 
	{
		// close the file before leaving...
		fclose(file_ptr);
		return NO_VTX_KERN_MODS_DETECTED;
	}
	
	// we are done reading...close it.
	fclose(file_ptr);
	return VTX_KERN_MODS_DETECTED;
}

// Many hypervisors expose virtualized devices with specific PCI IDs, which can be detected via the /sys/bus/pci/devices directory in Linux.
int detect_virtual_pci_devices(void)
{
	FILE *file_ptr;
	bool virt_pci_dev_detected;
	char path[1035];
	
	// Let's open the /sys/bus/pci/devices directory to check for PCI devices
	file_ptr = popen("ls /sys/bus/pci/devices", "r");
	
	if(file_ptr == NULL) 
	{
		perror("Failed to run command: \'ls /sys/bus/pci/devices\'\n");
		exit(1);
	}
	
	// Whew! it was successful, let's now read the output of the command we executed above, and look for
	// virtual pci devices
	while(fgets(path, sizeof(path), file_ptr) != NULL)
	{
		// This is an example of Virtual device's vendor IDs e.g VMWare, Intel.
		if(strstr(path, "1002") || strstr(path, "8086")) 
		{
			//printf("Virtual PCI Devices Detected: %s\n", path); 
			virt_pci_dev_detected = true;
			break;
		}
		else 
		{
			virt_pci_dev_detected = false;
		}
	}
	
	if(!virt_pci_dev_detected)
	{
		// close the file before leaving...
		fclose(file_ptr);
		return NO_VTX_PCI_DEV_DETECTED;
	}
	
	// we are done reading...close it.
	fclose(file_ptr);
	return VTX_PCI_DEV_DETECTED;
}
