//////////////////////////////////////////////////////////////////////////
// Mapper198  Nintendo MMC3                                             //
//////////////////////////////////////////////////////////////////////////
class	Mapper198 : public Mapper
{
public:
	Mapper198( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );
	BYTE	ReadLow( WORD addr );
	void	WriteLow(WORD addr, BYTE data);

	void	HSync( INT scanline );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );


protected:
	BYTE	reg[8];
	BYTE	prg[4];
	BYTE	chr[8];

	BYTE	irq_enable;
	BYTE	irq_counter;
	BYTE	irq_latch;
	BYTE	irq_request;
private:
	void	SetBank_CPU();
	void	SetBank_PPU();
};
