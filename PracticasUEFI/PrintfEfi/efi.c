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

//===============================
//Print a number (INT32 for now)
//===============================
bool print_number(INT32 number){
	const CHAR16 *digits = u"0123456789ABCDEF";
	CHAR16 buffer[11];		//Enough for INT32_MAX + sign character
	UINTN i = 0;
	const bool negative = (number < 0);
	
	do {
		buffer[i++] = digits[number % 10];
		number /= 10;
	}while(number > 0);
	
	//Pretend minus sign if negative
	if (negative) buffer[i++] = u'-';
	
	//NULL terminate string
	buffer[i--] = u'\0';
	
	//Reverse digits in buffer before printing 123 -> buffer = 321 
	for (UINTN j = 0; j < i; j++, i--){
		//Swap digts
		UINTN temp = buffer[i];
		buffer[i] = buffer[j];
		buffer[j] = temp;
	}
	
	//Print numberstring
	cout->OutputString(cout, buffer);
	
	return true;
}

//=====================
// Print formatted strings
//=========================
bool printf(CHAR16 *fmt, ...){
	bool result= false;
	CHAR16 charstr[2];		//TODO: Replace intializing this with memset and use = { } initializer
	
	va_list args;
	
	va_start(args, fmt);
	
	//Initialize buffers
	charstr[0] = u'\0', charstr[1] = u'\0';
	
	
	
	//Print formatted string values
	for (UINTN i = 0; fmt[i] != u'\0'; i++){
		if (fmt[i] == u'%'){
			i++;
		
			//Grab next argument type from input args, and print it
			switch (fmt[i]){
				case u's':{
					//Printf CHAR16 string; printf("%s", string)
					CHAR16 *string = va_arg(args, CHAR16*);
					cout->OutputString(cout, string);
				}
				break;
				
				case u'd':{
					//Printf INT32; printf("%D", number_int32)
					INT32 number = va_arg(args, INT32);
					print_number(number);
				}
				break;
				
				default:
					cout->OutputString(cout, u"Invalid format specifier: %");
					charstr[0] = fmt[i];
					cout->OutputString(cout, charstr);
					cout->OutputString(cout, u"\r\n");
					result= false;
					goto end;
					break;
			}
		}else{
			//Not formatted string, print next character
			charstr[0] = fmt[i];
			cout->OutputString(cout, charstr);
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
	printf(	u"Max Mode: %d\r\n"
			u"Current Mode: %d\r\n"
			//u"Attribute: %x\r\n"   //TODO: Change to %x and print hex
			u"Attribute: %d\r\n"
			u"CursorColumn: %d\r\n"
			u"CursorRow: %d\r\n"
			u"CursorVisible: %d\r\n",
			cout->Mode->MaxMode,
			cout->Mode->Mode,
			cout->Mode->Attribute,
			cout->Mode->CursorColumn,
			cout->Mode->CursorRow,
			cout->Mode->CursorVisible);
	
	cout->OutputString(SystemTable->ConOut, u"Avaible text modes:\r\n");
	
	
	
	//SetMode
	
	//QueryMode
	
	// Infinite Loop
	while (1);
	
	return EFI_SUCCESS;
}

