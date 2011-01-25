//////////////////////////////////////////////////////////////////////////
// Nintendo MMC3                                                        //
//////////////////////////////////////////////////////////////////////////
class MMC3 : public Mapper
{
public:
	MMC3( NES* parent );
	//virtual	~MMC3();

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

	
	void	Reset();

	void	Write( WORD addr, BYTE data );
	void	HSync( INT scanline );

	//////////////////////////////////////////////////////////////////////////
	// MMC3
	//////////////////////////////////////////////////////////////////////////	
	unsigned int count;
	unsigned int latch;
	unsigned int reload;
	unsigned int enabled;				

	unsigned int ctrl0;
	unsigned int ctrl1;

	unsigned int chr[8];
	unsigned int prg[4];	

	//Memory Write 8000~FFFF
	//void Mmc3_MemoryWrite(uint32 address, uint8 data);
	void Poke_Mmc3_8000(uint32 address,uint8 data);
	void Poke_Mmc3_8001(uint32 address,uint8 data);
	void Poke_Mmc3_A000(uint32 address,uint8 data);
	void Poke_Mmc3_A001(uint32 address,uint8 data);
	void Poke_Mmc3_C000(uint32 address,uint8 data);
	void Poke_Mmc3_C001(uint32 address,uint8 data);
	void Poke_Mmc3_E000(uint32 address,uint8 data);
	void Poke_Mmc3_E001(uint32 address,uint8 data);
	void Poke_Nop(uint32,uint8);

	//初始化Mmc3函数指针
	//void Mmc3_Init();
	//void Mmc3_Reset();
	void Mmc3_UpdatePrg();
	void Mmc3_UpdateChr();
	void Mmc3_UpdatePrg2p(unsigned int addr,unsigned int bank);
	void Mmc3_UpdateChr2p(unsigned int addr,unsigned int bank);
	unsigned int Mmc3_GetChrSource(unsigned int);
	void Mmc3_HSync(uint32 scanline);

	void Mmc3_SwapChr1K(uint8 page,uint32 bank);
	void Mmc3_SwapChr2K(uint8 page,uint32 bank);

	void (MMC3::*UpdateChr)(unsigned int,unsigned int);
	void (MMC3::*UpdatePrg)(unsigned int,unsigned int);
	unsigned int (MMC3::*GetChrSource)(unsigned int);

	void (MMC3::*Poke_8000)(uint32 address,uint8 data);
	void (MMC3::*Poke_8001)(uint32 address,uint8 data);
	void (MMC3::*Poke_A000)(uint32 address,uint8 data);
	void (MMC3::*Poke_A001)(uint32 address,uint8 data);
	void (MMC3::*Poke_C000)(uint32 address,uint8 data);
	void (MMC3::*Poke_C001)(uint32 address,uint8 data);
	void (MMC3::*Poke_E000)(uint32 address,uint8 data);
	void (MMC3::*Poke_E001)(uint32 address,uint8 data);
};


//////////////////////////////////////////////////////////////////////////
// Nintendo MMC3                                                        //
//////////////////////////////////////////////////////////////////////////
class fceuMMC3 : public Mapper
{
public:
	fceuMMC3( NES* parent,int imap);
	//virtual	~MMC3();

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){};
	void	LoadState( LPBYTE p ){};

	
	void	Reset();

	void	Write( WORD addr, BYTE data );
	void	Mmc3Write( WORD addr, BYTE data );
	void	(fceuMMC3::*pWrite)( WORD addr, BYTE data );
	void	WriteLow( WORD addr, BYTE data );
	void	Mmc3WriteLow( WORD addr, BYTE data );
	void	(fceuMMC3::*pWriteLow)( WORD addr, BYTE data );
	BYTE	ReadLow( WORD addr );
	BYTE	Mmc3ReadLow( WORD addr );
	BYTE	(fceuMMC3::*pReadLow)( WORD addr );
	void	Read( WORD addr, BYTE data );
	void	Mmc3Read( WORD addr, BYTE data );
	void	(fceuMMC3::*pRead)( WORD addr, BYTE data );
	void	HSync( INT scanline );

	//////////////////////////////////////////////////////////////////////////
	// MMC3
	//////////////////////////////////////////////////////////////////////////	
	uint8 MMC3_cmd;
	uint8 EXPREGS[8];
	uint8 DRegBuf[8];	
	
	uint8 IRQCount,IRQLatch,IRQa;
	uint8 IRQReload;
	
	
	uint8 A000B,A001B;
	int mmc3opts;
	//int wrams;
	int isRevB;
	int iMapper;

	void (fceuMMC3::*pwrap)(uint32 A, uint8 V);
	void (fceuMMC3::*cwrap)(uint32 A, uint8 V);
	void (fceuMMC3::*mwrap)(uint8 V);

	void GenMMC3Power(void);
	void GenMMC3Restore(int version);
	void MMC3RegReset(void);
	void FixMMC3PRG(int V);
	void FixMMC3CHR(int V);
	void MMC3_CMDWrite(uint32 A, uint8 V);
	void MMC3_IRQWrite(uint32 A, uint8 V);
	
	void ClockMMC3Counter(void);	
	void GENCWRAP(uint32 A, uint8 V);
	void GENPWRAP(uint32 A, uint8 V);
	void GENMWRAP(uint8 V);
	void GENNOMWRAP(uint8 V);
	
	//mapper 194
	void M194CW(uint32 A, uint8 V);
	void Reset194();

	//SACHEN_STREETHEROES
	uint8 tekker;
	void MSHCW(uint32 A, uint8 V);
	void MSHWrite(uint16 A, uint8 V);
	BYTE MSHRead(uint16 A);
	void MSHMWRAP(uint8 V);
	void MSHReset();

	//Super 24-in-1
	void Super24Reset();
	void Super24Write(uint16 A, uint8 V);
	void Super24PW(uint32 A, uint8 V);;
	void Super24CW(uint32 A, uint8 V);
	
	//Fk23c
	uint8 unromchr;
	uint32 dipswitch;
	void BMCFK23CCW(uint32 A, uint8 V);
	void BMCFK23CPW(uint32 A, uint8 V);
	void BMCFK23CHiWrite(uint16 A, uint8 V);
	void BMCFK23CWrite(uint16 A, uint8 V);
	void BMCFK23CReset();

	void BMCFK23CAReset();
};