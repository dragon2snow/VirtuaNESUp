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

