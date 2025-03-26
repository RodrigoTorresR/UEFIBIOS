#include <stdarg.h>
#include "efi.h"
//----------------
//Global varibles
//----------------
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *cout = NULL; // Console out
//EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*cin = NULL; //	console in
void *cin = NULL;
//void *bs;	//Boot services
//void *rs; //Runtime services

//================
//Set global vars
//================
void init_global_variables(EFI_SYSTEM_TABLE *systable){
	cout = systable->ConOut;
	cin = systable->ConIn;
}

//=========================
// Print formatted strings
//=========================
bool printf(CHAR16 *fmt, ...){
	bool result= false;
	CHAR16 str[2];		//TODO: Replace intializing this with memset and use = { } initializer
	va_list args;
	
	va_start(args, fmt);
	
	str[0] = u'\0', str[1] = u'\0';
	
	for (UINTN i = 0; fmt[i] != u'\0'; i++){
		if (fmt[i] == u'%'){
			i++;
		
			//Grab next argument type from input args, and print it
			switch (fmt[i]){
				case u's':{
					//printf("%s", string)
					CHAR16 *string = va_arg(args, CHAR16*);
					cout->OutputString(cout, string);
				}
				break;
				
				default:
					cout->OutputString(cout, u"(Invalid format specifier: %");
					cout->OutputString(cout, &fmt[i]);
					cout->OutputString(cout, u"\r\n");
					result= false;
					goto end;
					break;
			}
		}else{
			//Not formatted string, print next character
			str[0] = fmt[i];
			cout->OutputString(cout, str);
		}
	}
end:
	va_end(args);
	result= true;
	return result;
}


//================
//Entry Point
//================

//entry point set up
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
	//TODO: Remove this line when using input params 
	(void)ImageHandle, (void)SystemTable; //prevent compiler warnings
	
	init_global_variables(SystemTable);
	
	// Reset Console Output
	cout->Reset(SystemTable->ConOut, false);
	
	cout->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
	
	//CLEAR console outpout (clear screen to background color and
	// set cursor to 0.0
	cout->ClearScreen(SystemTable->ConOut); 
	
	// Write String
	cout->OutputString(SystemTable->ConOut, u"Current text mode:\r\n");
	printf(u"TESTING PRINTF %s\r\n\r\n", u"blah blah blah");
	
	cout->OutputString(SystemTable->ConOut, u"Avaible text modes:\r\n");
	
	
	
	//SetMode
	
	//QueryMode
	
	// Infinite Loop
	while (1);
	
	return EFI_SUCCESS;
}

