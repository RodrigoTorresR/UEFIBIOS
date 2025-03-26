#include <stdarg.h>
#include "efi.h"

//entry point set up
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
	//TODO: Remove this line when using input params 
	(void)ImageHandle, (void)SystemTable; //prevent compiler warnings
	
	// Reset Console Output
	SystemTable->ConOut->Reset(SystemTable->ConOut, false);
	
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_YELLOW, EFI_RED));
	
	//CLEAR console outpout (clear screen to background color and
	// set cursor to 0.0
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut); 
	
	// Write String
	SystemTable->ConOut->OutputString(SystemTable->ConOut, u"TESTING, Hello UEFI World!\r\n");
	
	// Write String in new colors
	//SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nHotdog color test\r\n");
	
	// Infinite Loop
	while (1);
	
	return EFI_SUCCESS;
}

