//efi.c programa simple que muestra en pantalla un mensaje, hay que reiniciar o apagar el sistema
//para terminar el programa
#include "efi.h"

//Punto de Entrada de nuestro programa, Note que no existe funcion main()
EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable){
	//PORDEFINIR: definir parametros de entrada de linea de comandos. 
	(void)ImageHandle, (void)SystemTable; //Esta linea evita advertencias del compilador
	
	// Reset Console Output Reinicia al PROTOCOLO  de salida a Consola
	SystemTable->ConOut->Reset(SystemTable->ConOut, false);
	
	//CLEAR console outpout (Borra la pantalla y por efecto colateral 
	// configura el color de fondo a Negro  y Reubica el cursor a la posicion LINEA 0 COLUMNA 0)
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut); 
	
	// Write String		Protocolo que permite mostrar cadenas de texto simple en el CLI
	SystemTable->ConOut->OutputString(SystemTable->ConOut, u"TESTING, Hello UEFI World!\r\n");
	
	// Loop Infinito para ver el mensaje, para terminar hay que apagar el sistema
	while (1);
	
	return EFI_SUCCESS;
}
