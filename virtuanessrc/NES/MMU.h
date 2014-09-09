//////////////////////////////////////////////////////////////////////////
//                                                                      //
//      NES Memory Management Unit                                      //
//                                                           Norix      //
//                                               written     2001/02/21 //
//                                               last modify ----/--/-- //
//////////////////////////////////////////////////////////////////////////

#ifndef	__MMU_INCLUDED__
#define	__MMU_INCLUDED__

#include "typedef.h"
#include "macro.h"

// CPU 儊儌儕僶儞僋
extern  BYTE	CPU_BACKUP[256];
extern	LPBYTE	CPU_MEM_BANK[8];	// 8K扨埵
extern	BYTE	CPU_MEM_TYPE[8];
extern	INT	CPU_MEM_PAGE[8];	// 僗僥乕僩僙乕僽梡

// PPU 儊儌儕僶儞僋
extern	LPBYTE	PPU_MEM_BANK[12];	// 1K扨埵
extern	BYTE	PPU_MEM_TYPE[12];
extern	INT	PPU_MEM_PAGE[12];	// 僗僥乕僩僙乕僽梡
extern	BYTE	CRAM_USED[16];		// 僗僥乕僩僙乕僽梡

// NES儊儌儕
extern	BYTE	RAM [  8*1024];		// NES撪憻RAM
extern	BYTE	WRAM[128*1024];		// 儚乕僋/僶僢僋傾僢僾RAM
extern	BYTE	DRAM[ 40*1024];		// 僨傿僗僋僔僗僥儉RAM
extern	BYTE	XRAM[  8*1024];		// 僟儈乕僶儞僋
extern	BYTE	ERAM[ 32*1024];		// 奼挘婡婍梡RAM
extern  BYTE	MRAM[128*1024];		//byemu 专用

extern	BYTE	CRAM[ 32*1024];		// 僉儍儔僋僞僷僞乕儞RAM
extern	BYTE	VRAM[  4*1024];		// 僱乕儉僥乕僽儖/傾僩儕價儏乕僩RAM

extern	BYTE	SPRAM[0x100];		// 僗僾儔僀僩RAM
extern	BYTE	BGPAL[0x10];		// BG僷儗僢僩
extern	BYTE	SPPAL[0x10];		// SP僷儗僢僩

// 儗僕僗僞
extern	BYTE	CPUREG[0x18];		// Nes $4000-$4017
extern	BYTE	PPUREG[0x04];		// Nes $2000-$2003

// Frame-IRQ儗僕僗僞($4017)
extern	BYTE	FrameIRQ;

// PPU撪晹儗僕僗僞
extern	BYTE	PPU56Toggle;		// $2005-$2006 Toggle
extern	BYTE	PPU7_Temp;		// $2007 read buffer
extern	WORD	loopy_t;		// same as $2005/$2006
extern	WORD	loopy_v;		// same as $2005/$2006
extern	WORD	loopy_x;		// tile x offset

// ROM僨乕僞億僀儞僞
extern	LPBYTE	PROM;		// PROM ptr
extern	LPBYTE	VROM;		// VROM ptr

extern LPBYTE	PROMPTR[16];
extern LPBYTE	VROMPTR[16];

extern INT PPROM_8K_SIZE[16];
extern INT PVROM_1K_SIZE[16];

#ifdef	_DATATRACE
// For dis...
extern	LPBYTE	PROM_ACCESS;
#endif

// ROM 僶儞僋僒僀僘
extern	INT	PROM_8K_SIZE, PROM_16K_SIZE, PROM_32K_SIZE;
extern	INT	VROM_1K_SIZE, VROM_2K_SIZE, VROM_4K_SIZE,  VROM_8K_SIZE;


// 娭悢
extern	void	NesSub_MemoryInitial();

//增加代码
//extern	void	SetPrg2 ( WORD A, INT bank );
//extern	void	SetPrg4 ( WORD A, INT bank );
extern	void	SetPrg8 ( WORD A, WORD bank );
extern	void	SetPrg16( WORD A, WORD bank );
extern	void	SetPrg32( WORD A, WORD bank );

//extern	void	SetPrg2r (int r, WORD A, INT bank);
//extern	void	SetPrg4r (int r, WORD A, INT bank);
extern	void	SetPrg8r (int r, WORD A, WORD bank);
extern	void	SetPrg16r(int r, WORD A, WORD bank);
extern	void	SetPrg32r(int r, WORD A, WORD bank);

extern	void	SetPROM_Bank( BYTE page, LPBYTE ptr, BYTE type );
extern	void	SetPROM_8K_Bank ( BYTE page, INT bank );
extern	void	SetPROM_16K_Bank( BYTE page, INT bank );
extern	void	SetPROM_32K_Bank( INT bank );
extern	void	SetPROM_32K_Bank( INT bank0, INT bank1, INT bank2, INT bank3 );

extern	void	SetVROM_Bank( BYTE page, LPBYTE ptr, BYTE type );
extern	void	SetVROM_1K_Bank( BYTE page, INT bank );
extern	void	SetVROM_2K_Bank( BYTE page, INT bank );
extern	void	SetVROM_4K_Bank( BYTE page, INT bank );
extern	void	SetVROM_8K_Bank( INT bank );
extern	void	SetVROM_8K_Bank( INT bank0, INT bank1, INT bank2, INT bank3,
				 INT bank4, INT bank5, INT bank6, INT bank7 );

extern	void	SetChr1( WORD A, INT bank );
extern	void	SetChr2( WORD A, INT bank );
extern	void	SetChr4( WORD A, INT bank );
extern	void	SetChr8( WORD A, INT bank );
extern	void	SetChr1r(int r, WORD A, INT bank);
extern	void	SetChr2r(int r, WORD A, INT bank);
extern	void	SetChr4r(int r, WORD A, INT bank);
extern	void	SetChr8r(int r, WORD A, INT bank);

extern	void	SetCRAM_1K_Bank( BYTE page, INT bank );
extern	void	SetCRAM_2K_Bank( BYTE page, INT bank );
extern	void	SetCRAM_4K_Bank( BYTE page, INT bank );
extern	void	SetCRAM_8K_Bank( INT bank );

extern	void	SetVRAM_1K_Bank( BYTE page, INT bank );
extern	void	SetVRAM_Bank( INT bank0, INT bank1, INT bank2, INT bank3 );
extern	void	SetVRAM_Mirror( INT type );
extern	void	SetVRAM_Mirror( INT bank0, INT bank1, INT bank2, INT bank3 );


// 儊儌儕僞僀僾
// For PROM (CPU)
#define	BANKTYPE_ROM	0x00
#define	BANKTYPE_RAM	0xFF
#define	BANKTYPE_DRAM	0x01
#define	BANKTYPE_MAPPER	0x80
// For VROM/VRAM/CRAM (PPU)
#define	BANKTYPE_VROM	0x00
#define	BANKTYPE_CRAM	0x01
#define	BANKTYPE_VRAM	0x80

// 儈儔乕僞僀僾
#define	VRAM_HMIRROR	0x00	// Horizontal
#define	VRAM_VMIRROR	0x01	// Virtical
#define	VRAM_MIRROR4	0x02	// All screen
#define	VRAM_MIRROR4L	0x03	// PA10 L屌掕 $2000-$23FF偺儈儔乕
#define	VRAM_MIRROR4H	0x04	// PA10 H屌掕 $2400-$27FF偺儈儔乕

#endif	// !__MMU_INCLUDED__

