//////////////////////////////////////////////////////////////////////////
// MapperFk23c  WaiXing San Guo Zhi - Xiong Ba Tian Xia (C)             //
//////////////////////////////////////////////////////////////////////////
class	MapperFk23c : public MMC3
{
public:
	MapperFk23c( NES* parent );

	
	void	Reset();
	void	Write( WORD addr, BYTE data );
	void	WriteLow( WORD addr, BYTE data );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

protected:
	BYTE	exRegs[8];
	BYTE	unromChr,mode;

private:
	void fk23c_UpdatePrg2p(unsigned int addr,unsigned int bank);
	void fk23c_UpdateChr2p(unsigned int addr,unsigned int bank);
	virtual void fk23c_UpdatePrg();
	void fk23c_UpdateChr();
};

class	MapperFk23ca : public MapperFk23c
{
public:
	MapperFk23ca( NES* parent );
	void	Reset();

private:
	void fk23c_UpdatePrg();
};