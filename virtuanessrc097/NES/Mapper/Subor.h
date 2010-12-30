//////////////////////////////////////////////////////////////////////////
// Subor                                                                //
//////////////////////////////////////////////////////////////////////////
class	MapperSubor999 : public Mapper
{
public:
	MapperSubor999( NES* parent ) : Mapper(parent) {}

	void	Reset();
	//void	Read( WORD addr, BYTE data );
	//BYTE	ReadLow( WORD addr, BYTE data );
	void	WriteLow( WORD addr, BYTE data );
	//void	Write( WORD addr, BYTE data );
	//void	HSync( INT scanline );
	void	PPU_ExtLatch( WORD ntbladr, BYTE& chr_l, BYTE& chr_h, BYTE& attr );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

protected:
	BYTE	reg5200,reg5000;
	void	SetBank_CPU(void);
private:
};
