//
#include "efi.h"
int power2(int num, int power);
//entry point set up
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
	//TODO: Remove this line when using input params 
	(void)ImageHandle, (void)SystemTable; //prevent compiler warnings
	
	// Reset Console Output
	SystemTable->ConOut->Reset(SystemTable->ConOut, false);
	
	//CLEAR console outpout (clear screen to background color and
	// set cursor to 0.0
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut); 
	
	// Write String
	SystemTable->ConOut->OutputString(SystemTable->ConOut, u"TESTING, Hello UEFI World!\r\n"); // power2(3, 5)
	//"3 times 2 to the power of 5 is %d\n", power2(3, 5)
	
	// Infinite Loop
	while (1);
	
	return EFI_SUCCESS;
}
int power2(int num, int power)
{
    __asm
    {
        mov eax, num; Get first argument
        mov ecx, power; Get second argument
        shl eax, cl; EAX = EAX * (2 to the power of CL)
    }
    // Return with result in EAX
}