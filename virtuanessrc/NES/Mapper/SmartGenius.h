//////////////////////////////////////////////////////////////////////////
// SmartGenius                                                            //
//////////////////////////////////////////////////////////////////////////
class	MapperSmartGenius : public Mapper
{
public:
	MapperSmartGenius( NES* parent ) : Mapper(parent) {}

	
	void	Reset();
	BYTE	ReadLow ( WORD addr );
	void	WriteLow( WORD addr, BYTE data );
	void	Write( WORD addr, BYTE data );

	void	HSync( INT scanline );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

protected:
	WORD	reg[11];
	WORD	prg0, prg1, prg2, prg3;
	BYTE	chr01, chr23, chr4, chr5, chr6, chr7;
	BYTE	chr1, chr3;
	BYTE	we_sram;

	BYTE	irq_enable;
	BYTE	irq_counter;
	BYTE	irq_latch;
	BYTE	irq_request;

private:
	void	SetBank_CPU();
	void	SetBank_CPU_L();
	void	SetBank_PPU();
};

class	WaiXing_FS005 : public Mapper
{
public:
	WaiXing_FS005( NES* parent ) : Mapper(parent) {}

	
	void	Reset();
	void	WriteLow( WORD addr, BYTE data );
	void	Write( WORD addr, BYTE data );


	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){}
	void	LoadState( LPBYTE p ){}
};

class	WaiXing_FW01 : public Mapper
{
public:
	WaiXing_FW01( NES* parent ) : Mapper(parent) {}

	
	void	Reset();
	void	Write( WORD addr, BYTE data );


	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){}
	void	LoadState( LPBYTE p ){}
};