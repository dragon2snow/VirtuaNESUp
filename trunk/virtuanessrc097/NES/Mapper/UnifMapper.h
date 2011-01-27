class	map0_3208cn : public Mapper
{
public:
	map0_3208cn( NES* parent ) : Mapper(parent) {}
	
	void	Reset();
	void	Write( WORD addr, BYTE data );	
};


class	GeniusMerioBros : public Mapper
{
public:
	GeniusMerioBros( NES* parent ) : Mapper(parent) {}
	
	void	Reset();
	BYTE	ReadLow ( WORD addr );
	void	WriteLow( WORD addr, BYTE data );
};

class	smb2j : public Mapper
{
public:
	smb2j( NES* parent ) : Mapper(parent) {}
	
	void	Reset();
	void	Write( WORD addr, BYTE data );// (0x4020,0xffff,UNLSMB2JWrite);
	void	WriteLow( WORD addr, BYTE data );
	BYTE	ReadLow ( WORD addr );
	void	HSync( INT scanline );
	
protected:
	
	BYTE prg, IRQa;
	WORD IRQCount;
private:
};

class	Mapper8157 : public Mapper
{
public:
	Mapper8157( NES* parent );
	
	void	Reset();
	void	Write( WORD addr, BYTE data );
	//void	Read( WORD addr, BYTE data );
	
protected:
	WORD mode;
	BYTE trash;
private:
};

class	MapperT262 : public Mapper
{
public:
	MapperT262( NES* parent );
	
	void	Reset();
	void	Write( WORD addr, BYTE data );
	
protected:
	 uint16 addrreg;
	 uint8 datareg;
	 uint8 busy;
private:
};