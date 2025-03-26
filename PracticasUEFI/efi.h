// NOTA: Usamos "VOID *" para marcar un elemento que aun
// No ha sido implementado, pero debe estar presente ya que
// la especificacion dicta que asi debe ser
#include <stdint.h>
#include <stdbool.h>
#if __has_include(<uchar.h>)
	#include <uchar.h>
#endif
//UEFI Especificacion 2.10 seccion 2.4
#define IN			//Ayudas visuales que nos permiten saber el flujo de los datos
#define OUT			//como no cuentan con definicion solo titulo se pueden tratar
#define OPTIONAL	//como etiquetas en el codigo similar a las usadas en Ensamblador 
#define CONST const
//EFIAPI define las llamadas de las funciones UEFI
#define EFIAPI __attribute__((ms_abi)) 	//Revise la informacion sobre las llamadas x86_64 impuestas por Microsoft
//Tipos de datos usados en UEFI Especificacion 2.10 seccion 2.3
//typedef uint8_t UNIT8;		//Redefinicion del lenguaje para enmascarar los tipos usados en la especificacion
typedef uint8_t			BOOLEAN;	//0 = False 1 = True
typedef int64_t 		INTN;
typedef uint64_t		UINTN;
typedef int8_t			INT8;
typedef uint8_t			UINT8;
typedef int16_t			INT16;
typedef uint16_t		UINT16;
typedef int32_t			INT32;
typedef uint32_t		UINT32;
typedef int64_t			INT64;
typedef uint64_t		UINT64;
typedef char			CHAR8;
//UTF-16 Es el equivalente de tipo , para caracteres del código UCS-2
//codepoints <= 0xFFFF_FFFF
#ifndef _UCHAR_H
	typedef uint_least16_t	char16_t;
#else
	typedef char16_t CHAR16;
#endif
typedef char16_t CHAR16;
typedef void VOID;
typedef struct EFI_GUID{	//Mas informacion sobre EFI_GUID en la especificacion UEFI APENDICE A
	UINT32		TimeLow;
	UINT16		TimeMid;
	UINT16		TimeHighAndVersion;
	UINT8		ClockSeqHighAndReserved;
	UINT8		ClockSeqLow;
	UINT8		Node[6];
}__attribute__ ((packed)) EFI_GUID;
typedef UINTN		EFI_STATUS;
typedef	VOID 		*EFI_HANDLE;
typedef VOID 		*EFI_EVENT;
typedef UINT64 		EFI_LBA;
typedef UINTN		EFI_TPL;
//EFI_STATUS  (Codigos de estado) - UEFI Especificacion 2.10 Appendice D
#define EFI_SUCCESS 0ULL
//PORDEFINIR: Agregar EFI_ERRO()para verificar los valores retornados por EFI_STATUS
//Si el Bit mas alto esta activo , verifcar si hubo un estado de ERROR
//EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL: UEFI Especificacion 2.10 seccion 12.2 
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL; //Resulta extraña la definicion del
																				//del protocolo
//EFI_SIMPLE_TEXT_RESET_PROTOCOL: UEFI Especificacion 2.10 seccion 12.4.1
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_RESET)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL		*This,
	IN BOOLEAN								ExtendedVerification
);
//EFI_SIMPLE_TEXT_STRING_PROTOCOL: : UEFI Especificacion 2.10 seccion 12.4.3
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_STRING)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL		*This,
	IN CHAR16								*String
);
//EFI_SIMPLE_TEXT_CLEAR_SCREEN_PROTOCOL: UEFI Spec 2.10 section 12.4.1
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_CLEAR_SCREEN)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL		*This
);
//EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL: UEFI Spec 2.10 section  12.4.8
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL{
	EFI_TEXT_RESET					Reset;
	EFI_TEXT_STRING					OutputString;
	void							*TestString;
	void							*QueryMode;
	void							*SetMode;
	void							*SetAttribute;
	EFI_TEXT_CLEAR_SCREEN			ClearScreen;
	void							*SetCursorPosition;
	void							*EnableCursor;
	void							*Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
// EFI_TABLE_HEADER: UEFI Spec 2.10 section 4.2.1
typedef struct{
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} EFI_TABLE_HEADER;
//EFI_SYSTEM_TABLE: Spec 2.10 section 4.3.1
typedef struct{
	EFI_TABLE_HEADER				Hdr;
	CHAR16							*FirmwareVendor;
	UINT32							FirmwareRevision;
	EFI_HANDLE						ConsoleInHandle;
	VOID 							*ConIn; //EFI_SIMPLE_TEXT_INPUT_PROTOCOL	*ConIn; //Protocolos no implementados AUN
	EFI_HANDLE						ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut; //VOID 							*Conout; //usamos *VOID para indicar que no esta implemntado
	EFI_HANDLE						StandardErrorHandle;
	VOID 							*StdErr; //EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*StdErr;
	VOID 							*RuntimeServices; //EFI_RUNTIME_SERVICES			*RuntimeServices;
	UINTN							NumberOfTablesEntries;
	VOID							*ConfigurationTable; //EFI_CONFIGURATION_TABLE			ConfigurationTable;
} EFI_SYSTEM_TABLE;
//EFI_IMAGE_ENTRY_POINT: UEFI Spec 2.10 section 4.1.1 
										//El compilador arroja este error cuando la funcion MAIN() no es la principal o no existe
										//Hay que especificar explicitamente cual es la funcion de Inicio del programa principal.
typedef EFI_STATUS						//efi.h:69:1: warning: ‘ms_abi’ attribute directive ignored [-Wattributes]
(EFIAPI *EFI_IMAGE_ENTRY_POINT)(		//check the MICROSOFT CALLING CONVENTIONS on Wiki
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable	//Nota IN solo indica la direccion del flujo de la informacion, esto es ayuda ilustrativa
);										// para el programador
