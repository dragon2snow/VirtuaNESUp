//////////////////////////////////////////////////////////////////////////
// Mapper253                                                            //
//////////////////////////////////////////////////////////////////////////
class	Mapper253 : public Mapper
{
public:
	Mapper253( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );

	void	Clock( INT cycles );
	void	SetBank_PPUSUB( int bank, int page );
	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

protected:
	BYTE	reg[8];
	BYTE	irq_enable;
	BYTE	irq_counter;
	BYTE	irq_latch;
	//BYTE	irq_occur;
	INT	irq_clock;
	
	BYTE	VRAM_switch;
	BYTE	rom_type ;
private:
};
