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

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

protected:
	BYTE	reg5200,reg5000,reg480d;
	void	SetBank_CPU(void);
private:
};
