// NOTE: "VOID *" Fields in structs = NOT implemented!!
// They are definied on the UEFI specs, but they are not
// implemented YET
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> //NULL

#if __has_include(<uchar.h>)
	#include <uchar.h>
#endif

//UEFI Spec 2.10 section 2.4
#define IN			//visual helpers to remark the flow of the data
#define OUT			//this is not has any meaning they are Vacum CONST
#define OPTIONAL
#define CONST const

//EFIAPI defines the calling conventions for EFI defined functions
#define EFIAPI __attribute__((ms_abi)) 	//x86_64 Microsoft calling convention

//Data types UEFI Spec 2.10 sections 2.3
//typedef uint8_t UNIT8;
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

//UTF-16 Equivalent is type, for UCS-2 characters
//codepoints <= 0xFFFF_FFFF
#ifndef _UCHAR_H
	typedef uint_least16_t	char16_t;
#else
	typedef char16_t CHAR16;
#endif
typedef char16_t CHAR16;

typedef void VOID;

typedef struct EFI_GUID{	//look for EFI_GUID on specs UEFI APPENDIX A
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

//EFI_STATUS Codes - UEFI Spec 2.10 Appendix D
#define EFI_SUCCESS 0ULL

//TODO: Add EFI_ERROR() macro/other for checking if EFI_STATUS
//high bit is set, if so it is an ERROR status

//EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

//EFI_SIMPLE_TEXT_RESET_PROTOCOL: UEFI Spec 2.10 section 12.4.1
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_RESET)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL		*This,
	IN BOOLEAN								ExtendedVerification
);

//EFI_SIMPLE_TEXT_STRING_PROTOCOL: UEFI Spec 2.10 section 12.4.1
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_STRING)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL		*This,
	IN CHAR16								*String
);

//EFI_TEXT_QUERY_MODE: UEFI Spec 2.10. section 12.4.5
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_QUERY_MODE) (
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN UINTN							ModeNumber,
	OUT UINTN							*Columns,
	OUT UINTN							*Rows
);

//EFI_TEXT_SET_MODE: UEFI Spec 2.10 section 12.4.6
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_SET_MODE)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
	IN UINTN							ModeNumber
);

//EFI_TEXT_SET_ATTRIBUTE: UEFI Spec 2.10 section 12.4.7
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL		*This,
	IN UINTN								Attribute
);

//Attributes (text colors)
#define EFI_BLACK								0X00
#define EFI_BLUE								0X01
#define EFI_GREEN								0X02
#define EFI_CYAN								0X03
#define EFI_RED									0X04
#define EFI_MAGENTA								0X05
#define EFI_BROWN								0X06
#define EFI_LIGHTGRAY							0X07
#define EFI_BRIGHT								0X08
#define EFI_DARKGRAY (EFI_BLACK | EFI_BRIGHT)	0X08
#define EFI_LIGHTBLUE							0X09
#define EFI_LIGHTGREEN							0X0A
#define EFI_LIGTHCYAN							0X0B
#define EFI_LIGHTRED							0X0C
#define EFI_LIGTHMAGENTA						0X0S
#define EFI_YELLOW								0X0E
#define EFI_WHITE								0X0F

//Back ground colors
#define EFI_BACKGROUND_BLACK					0X00
#define EFI_BACKGROUND_BLUE					0X10
#define EFI_BACKGROUND_GREEN					0X20
#define EFI_BACKGROUND_CYAN					0X30
#define EFI_BACKGROUND_RED						0X40
#define EFI_BACKGROUND_MAGENTA					0X50
#define EFI_BACKGROUND_BROWN					0X60
#define EFI_BACKGROUND_LIGHTGRAY				0X70 

#define EFI_TEXT_ATTR(Foreground, Background) \
		((Foreground) | ((Background) <<4))

//EFI_SIMPLE_TEXT_CLEAR_SCREEN_PROTOCOL: UEFI Spec 2.10 section 12.4.1
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_CLEAR_SCREEN)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL		*This
);

//SIMPLE_TEXT_OUTPUT_MODE
typedef struct{
		INT32	Mode;
		INT32	Attribute;
		INT32	CursorColumn;
		INT32	CursorRow;
		BOOLEAN	CursorVisible;
}SIMPLE_TEXT_OUTPUT_MODE;

//EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL: UEFI Spec 2.10 section  12.4.1
typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL{
	EFI_TEXT_RESET					Reset;
	EFI_TEXT_STRING					OutputString;
	void							*TestString;
	EFI_TEXT_QUERY_MODE				*QueryMode;
	EFI_TEXT_SET_MODE				*SetMode;
	EFI_TEXT_SET_ATTRIBUTE			SetAttribute;
	EFI_TEXT_CLEAR_SCREEN			ClearScreen;
	void							*SetCursorPosition;
	void							*EnableCursor;
	SIMPLE_TEXT_OUTPUT_MODE			*Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;


// EFI_TABLE_HEADER: UEFI Spec 2.10 section 4.2.1
typedef struct{
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} EFI_TABLE_HEADER;

//EFI_SYSTEM_TABLE:
typedef struct{
	EFI_TABLE_HEADER				Hdr;
	CHAR16							*FirmwareVendor;
	UINT32							FirmwareRevision;
	EFI_HANDLE						ConsoleInHandle;
	VOID 							*ConIn; //EFI_SIMPLE_TEXT_INPUT_PROTOCOL	*ConIn; 
	EFI_HANDLE						ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut; //VOID 							*Conout; 
	EFI_HANDLE						StandardErrorHandle;
	VOID 							*StdErr; //EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*StdErr;
	VOID 							*RuntimeServices; //EFI_RUNTIME_SERVICES			*RuntimeServices;
	UINTN							NumberOfTablesEntries;
	VOID							*ConfigurationTable; //EFI_CONFIGURATION_TABLE			ConfigurationTable;
} EFI_SYSTEM_TABLE;

//EFI_IMAGE_ENTRY_POINT: UEFI Spec 2.10 section 4.1.1 

typedef EFI_STATUS						//efi.h:69:1: warning: ‘ms_abi’ attribute directive ignored [-Wattributes]
(EFIAPI *EFI_IMAGE_ENTRY_POINT)(		//check the MICROSOFT CALLING CONVENTIONS on Wiki
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable	//Note IN is only meaning the flow of the information
);
