//////////////////////////////////////////////////////////////////////////
// Mapper002  UNROM                                                     //
//////////////////////////////////////////////////////////////////////////
class	Mapper002 : public Mapper
{
public:
	Mapper002( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	WriteLow(WORD addr, BYTE data);
	void	Write(WORD addr, BYTE data);

protected:
	BYTE	patch;
private:
};


class	Dream : public Mapper
{
public:
	Dream( NES* parent ) : Mapper(parent) {}

	void	Reset(){SetPROM_32K_Bank( 0, 1, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );}
	void	WriteLow(WORD A, BYTE V){if(A==0x5020) SetPROM_16K_Bank(4,V&7);}
private:
};