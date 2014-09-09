//////////////////////////////////////////////////////////////////////////
// Sachen                                                               //
//////////////////////////////////////////////////////////////////////////
class Sachen : public Mapper
{
public:
	Sachen( NES* parent,int imap);

	// For Mapper
	void	Reset();

	// $8000-$FFFF Memory write
	void	Write( WORD addr, BYTE data );
	void	(Sachen::*pWrite)( WORD addr, BYTE data );

	// $8000-$FFFF Memory read(Dummy)
	void	Read( WORD A, BYTE V );
	void	(Sachen::*pRead)( WORD A, BYTE V );

	// $4100-$7FFF Lower Memory read/write
	BYTE	ReadLow ( WORD A );
	BYTE	(Sachen::*pReadLow) ( WORD A );

	void	WriteLow( WORD A, BYTE V );
	void	(Sachen::*pWriteLow)( WORD A, BYTE V );
	
	// H sync/V sync/Clock sync
	/*
	void	HSync( INT scanline );
	void	HSync( INT scanline );
	void	HSync( INT scanline );
	void	VSync();
	void	VSync();
	void	VSync();
	void	Clock( INT cycles );
	void	Clock( INT cycles );
	void	Clock( INT cycles );
	*/

	// For State save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ) {}
	void	LoadState( LPBYTE p ) {}

	//ÊµÌå´úÂë
	void S74LS374MSync(uint8 mirr);
	void S74LS374NSynco(void);
	void S74LS374NWrite( WORD A, BYTE V );
	BYTE S74LS374NRead(WORD A);
	void S74LS374NPower(void);
	void S74LS374NReset(void);

	//
	void(Sachen::*WSync)(void);

	//mapper 146 (SA0161M)
	void SA0161MReset(void);
	void SAWrite(WORD A,BYTE V);

	//mapper 147(TCU01)
	void TCU01Reset(void);
	void TCU01Synco();
	void TCU01Write(WORD A, BYTE V );

	//mapper 148 (SA0037)
	void SA0037Reset(void);
	void SA0161MSynco();
	void SADWrite(WORD A, BYTE V );

	//MAPPER 149 SA0036
	void SA0036Reset(void);
	void SA72007Synco();
	void SA009Synco();

	//mapper 150 (S74LS374N)
	//void S74LS374NReset(void);



protected:
	INT iMapper;
	BOOL bFirst;
	
	uint8 cmd, dip;
	uint8 latch[8];
};