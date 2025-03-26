#include <stdarg.h>
#include "efi.h"
//----------------
//Global constants
//----------------
#define scancode_ESC 0x17

//----------------
//Global varibles
//----------------
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *cout = NULL; // Console out
EFI_SIMPLE_TEXT_INPUT_PROTOCOL	*cin = NULL; //	console in
EFI_BOOT_SERVICES 				*bs;	//Boot services
EFI_RUNTIME_SERVICES 			*rs; //Runtime services
EFI_HANDLE image = NULL;		//Image handler


//================
//Set global vars
//================
void init_global_variables(EFI_HANDLE handle,EFI_SYSTEM_TABLE *systable){
	cout = systable->ConOut;
	cin = systable->ConIn;
	bs = systable->BootServices;
	rs = systable->RuntimeServices;
	image = handle;
}

//===============================
//Print an integer (INT32 for now)
//===============================
bool print_int(INT32 number){
	const CHAR16 *digits = u"0123456789";
	CHAR16 buffer[11];		//Enough for INT32_MAX + sign character
	UINTN i = 0;
	const bool negative = (number < 0);
	
	if (negative) number = -number;   //to ensure the next loop works propperly
	
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

//===================================
//Print a hexadecimal integer (UINTN)
//===================================
bool print_hex(UINTN number){
	const CHAR16 *digits = u"0123456789ABCDEF";
	CHAR16 buffer[20];		//Enough for UINTN_MAX, hopefully
	UINTN i = 0;
	
	do {
		buffer[i++] = digits[number % 16];
		number /= 16;
	}while(number > 0);
	
	
	//Prepend final string with 0x
	buffer[i++] = u'x';
	buffer[i++] = u'0';
	
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

//========================
// Print formatted strings
//=========================
bool printf(CHAR16 *fmt, ...){
	bool result= true;
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
					//Printf INT32; printf("%d", number_int32)
					INT32 number = va_arg(args, INT32);
					print_int(number);
				}
				break;
				
				case u'x':{
					//Printf hex UINTN; printf("%x", number_uintn)
					UINTN number = va_arg(args, UINTN);
					print_hex(number);
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
	
	return result;
}

//==================
//Get key from user 
//==================
EFI_INPUT_KEY get_key(void){
	EFI_EVENT events[1];
	EFI_INPUT_KEY key;
	
	key.ScanCode = 0;
	key.UnicodeChar = u'\0';
	
	events[0] = cin->WaitForKey;
	UINTN index = 0;
	bs->WaitForEvent(1, events, &index);
	
	if (index == 0) cin->ReadKeyStroke(cin, &key);
	
	return key;
}

//================
//ASM
//================
INT32	 gcd( INT32	 a, INT32	 b ) {
    INT32 result ;
    /* Compute Greatest Common Divisor using Euclid's Algorithm */
    __asm__ __volatile__ ( "movl %1, %%eax;"
                          "movl %2, %%ebx;"
                          "CONTD: cmpl $0, %%ebx;"
                          "je DONE;"
                          "xorl %%edx, %%edx;"
                          "idivl %%ebx;"
                          "movl %%ebx, %%eax;"
                          "movl %%edx, %%ebx;"
                          "jmp CONTD;"
                          "DONE: movl %%eax, %0;" : "=g" (result) : "g" (a), "g" (b)
    );

    return result ;
}

//================
//Entry Point
//================

//entry point set up
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
	//Initialize global varibales
	init_global_variables(ImageHandle, SystemTable);
	
	// Reset Console Output
	cout->Reset(SystemTable->ConOut, false);
	
	cout->SetAttribute(SystemTable->ConOut, EFI_TEXT_ATTR(EFI_YELLOW, EFI_BLUE));
	
	//CLEAR console outpout (clear screen to background color and
	// set cursor to 0.0
	cout->ClearScreen(SystemTable->ConOut); 
	
	//Probamos la funcion ASM en linea
	printf(u"Testing ASM: %d\r\n\r\n", (INT32) gcd(4, 2));
	
	//Probamos ingresar numeros
	//Numero1
	printf(u"\rEscriba un nuevo numero1: ");
	//Move the cursor left by 1, para recibir el dato
	cout->SetCursorPosition(cout, cout->Mode-> CursorColumn-1, cout->Mode->CursorRow);
	//Obtenemos el dato y lo procesamos
	EFI_INPUT_KEY key = get_key();
	//Get key info
	CHAR16 cbuf[2];
	cbuf[0] = key.UnicodeChar;
	cbuf[1] = u'\0';
	printf(u"%s", cbuf);
	//Guardamos el dato
	UINTN Respuesta1 = key.UnicodeChar -u'0';
	printf(u"\r\nEl nuevo numero1 es: %d\r\n ", (INT32) Respuesta1);
	
	//Probamos ingresar numeros
	//Numero 2
	printf(u"\rEscriba un nuevo numero2 es: ");
	//Move the cursor left by 1, para recibir el dato
	cout->SetCursorPosition(cout, cout->Mode-> CursorColumn-1, cout->Mode->CursorRow);
	//Obtenemos el dato y lo procesamos
	key = get_key();//EFI_INPUT_KEY key = get_key();
	//Get key info
	//CHAR16 cbuf[2];
	cbuf[0] = key.UnicodeChar;
	cbuf[1] = u'\0';
	printf(u"%s", cbuf);
	//Guardamos el dato
	UINTN Respuesta2 = key.UnicodeChar -u'0';
	printf(u"\r\nEl nuevo numero 2 es: %d\r\n ", (INT32) Respuesta2);
	
	printf(u"\r\nEl nuevo numero1: %d y numero2: %d \r\n ", (INT32) Respuesta1, (INT32) Respuesta2);
	
	//Probamos los nuevos valores en la funcion
	printf(u"\r\nTesting again ASM: %d\r\n\r\n", (INT32) gcd(Respuesta1, Respuesta2));
	
	// Infinite Loop
	while (1);
	
	return EFI_SUCCESS;
}

