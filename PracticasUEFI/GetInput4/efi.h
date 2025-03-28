// NOTE: "VOID *" Fields in structs = NOT implemented!!
// They are definied on the UEFI specs, but they are not
// implemented YET. Using void pointers ensures they take up
// the same amount of space so that the actually definded functions
// at the correct offsets:
//
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

#define TOP_BIT 0x8000000000000000
#define ENCODE_ERROR(x) (TOP_BIT | (x))
#define EFI_ERROR(x) ((INTN)((UINTN)(x)) < 0)

#define EFI_UNSUPPORTED  ENCODE_ERROR(3)
#define EFI_DEVICE_ERROR ENCODE_ERROR(7)

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
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL 	*This,
	IN UINTN				ModeNumber
);

//EFI_TEXT_SET_ATTRIBUTE: UEFI Spec 2.10 section 12.4.7
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_SET_ATTRIBUTE)(
	IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL		*This,
	IN UINTN					Attribute
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

//EFI_TEXT_SET_CURSOR_POSITTION: UEFI Spec 2.10 section 12.4.9
typedef
EFI_STATUS
(EFIAPI *EFI_TEXT_SET_CURSOR_POSITION) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,
    IN UINTN                           Column,
    IN UINTN                           Row
);

//SIMPLE_TEXT_OUTPUT_MODE
typedef struct{
		INT32	MaxMode;
		
		//Current settings
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
	void						*TestString;
	EFI_TEXT_QUERY_MODE				QueryMode;
	EFI_TEXT_SET_MODE				SetMode;
	EFI_TEXT_SET_ATTRIBUTE				SetAttribute;
	EFI_TEXT_CLEAR_SCREEN				ClearScreen;
	EFI_TEXT_SET_CURSOR_POSITION 			SetCursorPosition;
	void						*EnableCursor;
	SIMPLE_TEXT_OUTPUT_MODE				*Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

//EFI_SIMPLE_TEXT_INPUT_PROTOCOL: UEFI Spec 2.10 section 12.3.1
typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

//EFI_INPUT_RESET: UEFI Spec 2.10 section 12.3.2
typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_RESET) (
	IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL	*This,
	IN BOOLEAN							ExtendedVerification
);

//EFI_INPUT_KEY
typedef struct{
	UINT16		ScanCode;
	CHAR16		UnicodeChar;
} EFI_INPUT_KEY;


//EFI_INPUT_RESET: UEFI Spec 2.10 section 12.3.2
typedef
EFI_STATUS
(EFIAPI *EFI_INPUT_READ_KEY) (
	IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL	*This,
	OUT EFI_INPUT_KEY					*Key
);

//EFI_SIMPLE_TEXT_INPUT_PROTOCOL: UEFI Spec 2.10 section 12.3.1
typedef struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL{
	EFI_INPUT_RESET				Reset;
	EFI_INPUT_READ_KEY			ReadKeyStroke;
	EFI_EVENT					WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

//EFI_WAIT_FOR_EVENT: UEFI Sec 1.10 section 7.1.5
typedef
EFI_STATUS
(EFIAPI *EFI_WAIT_FOR_EVENT) (
	IN UINTN		NumberOfEvents,
	IN EFI_EVENT	*Event,
	OUT UINTN		*Index
);

//UEFI_RESET_TYPE: UEFI Spec 2.10 section 8.5.1
typedef enum {
	EfiRsetCold,
	EfiResetWarm,
	EfiResetShutdown,
	EfiResetPlatformSpecific
} EFI_RESET_TYPE;

//EFI_RESET_SYSTEM: UEFI Spec 2.10 section 8.5.1
typedef
VOID
(EFIAPI *EFI_RESET_SYSTEM) (
    IN EFI_RESET_TYPE		ResetType,
    IN EFI_STATUS		ResetStatus,
    IN UINTN			DataSize,
    IN VOID			*ResetData OPTIONAL
);

// EFI_TABLE_HEADER: UEFI Spec 2.10 section 4.2.1
typedef struct{
	UINT64 Signature;
	UINT32 Revision;
	UINT32 HeaderSize;
	UINT32 CRC32;
	UINT32 Reserved;
} EFI_TABLE_HEADER;

// EFI_RUNTIME_SERVICES: UEFI Spec 2.10 section 4.5.1
typedef struct {
    EFI_TABLE_HEADER Hdr;
    
    //
    // Time Services
    //
    void *GetTime;
    void *SetTime;
    void *GetWakeupTime;
    void *SetWakeupTime;
    
    //
    // Virtual Memory Services
    //
    void *SetVirtualAddressMap;
    void *ConvertPointer;
    
    //
    // Variable Services
    //
    void *GetVariable;
    void *GetNextVariableName;
    void *SetVariable;
    
    //
    // Miscellaneous Services
    //
    void *GetNextHighMonotonicCount;
    EFI_RESET_SYSTEM ResetSystem;
    
    //
    // UEFI 2.0 Capsule Services
    //
    void *UpdateCapsule;
    void *QueryCapsuleCapabilities;
    
    //
    // Miscellaneous UEFI 2.0 Service
    //
    void *QueryVariableInfo;
} EFI_RUNTIME_SERVICES;



//EFI_BOOT_SERVICES: UEFI Spec section 4.4.1
typedef struct {
    EFI_TABLE_HEADER 	Hdr;

    //
    // Task Priority Services
    //
    void		*RaiseTPL;
    void		*RestoreTPL;

    //
    // Memory Services
    //
    void*         	AllocatePages;
    void*         	FreePages;
    void*         	GetMemoryMap;
    void* 		AllocatePool;
    void*     		FreePool;

    //
    // Event & Timer Services
    //
    void   		*CreateEvent;
    void      		*SetTimer;
    EFI_WAIT_FOR_EVENT 	WaitForEvent;
    void              	*SignalEvent;
    void    		*CloseEvent;
    void              	*CheckEvent;

    //
    // Protocol Handler Services
    //
    void		*InstallProtocolInterface;
    void		*ReinstallProtocolInterface;
    void		*UninstallProtocolInterface;
    void		*HandleProtocol;
    VOID		*Reserved;
    void		*RegisterProtocolNotify;
    void		*LocateHandle;
    void		*LocateDevicePath;
    void		*InstallConfigurationTable;

    //
    // Image Services
    //
    void		*LoadImage;
    void		*StartImage;
    void		*Exit;
    void		*UnloadImage;
    void		*ExitBootServices;

    //
    // Miscellaneous Services
    //
    void		*GetNextMonotonicCount;
    void		*Stall;	
    void		*SetWatchdogTimer;

    //
    // DriverSupport Services
    //
    void		*ConnectController;
    void		*DisconnectController;

    //
    // Open and Close Protocol Services
    //
    void		*OpenProtocol;
    void		*CloseProtocol;
    void		*OpenProtocolInformation;

    //
    // Library Services
    //
    void		*ProtocolsPerHandle;
    void		*LocateHandleBuffer;
    void		*LocateProtocol;
    void		*InstallMultipleProtocolInterfaces;
    void		*UninstallMultipleProtocolInterfaces;

    //
    // 32-bit CRC Services
    //
    void		*CalculateCrc32;

    //
    // Miscellaneous Services
    //
    void		*CopyMem;
    void		*SetMem;
    void		*CreateEventEx;
} EFI_BOOT_SERVICES;

//EFI_SYSTEM_TABLE: UEFI Spec 2.10 section 4.3.1
typedef struct{
	EFI_TABLE_HEADER					Hdr;
	CHAR16							*FirmwareVendor;
	UINT32							FirmwareRevision;
	EFI_HANDLE						ConsoleInHandle;
	EFI_SIMPLE_TEXT_INPUT_PROTOCOL	*ConIn; 
	EFI_HANDLE						ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut; //VOID 							*Conout; 
	EFI_HANDLE						StandardErrorHandle;
	VOID 							*StdErr; //EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*StdErr;
	VOID 							*RuntimeServices; //EFI_RUNTIME_SERVICES			*RuntimeServices;
	EFI_BOOT_SERVICES					*BootServices;
	UINTN							NumberOfTablesEntries;
	VOID							*ConfigurationTable; //EFI_CONFIGURATION_TABLE			ConfigurationTable;
} EFI_SYSTEM_TABLE;

//EFI_IMAGE_ENTRY_POINT: UEFI Spec 2.10 section 4.1.1 

typedef EFI_STATUS						//efi.h:69:1: warning: ‘ms_abi’ attribute directive ignored [-Wattributes]
(EFIAPI *EFI_IMAGE_ENTRY_POINT)(		//check the MICROSOFT CALLING CONVENTIONS on Wiki
	IN EFI_HANDLE ImageHandle,
	IN EFI_SYSTEM_TABLE *SystemTable	//Note IN is only meaning the flow of the information
);
