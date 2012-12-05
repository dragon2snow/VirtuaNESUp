//////////////////////////////////////////////////////////////////////////
// Mapper042  Mario Baby                                                //
//////////////////////////////////////////////////////////////////////////
class	Mapper042 : public Mapper
{
public:
	Mapper042( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );

	void	HSync( INT scanline );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

protected:
	BYTE	irq_enable;
	BYTE	irq_counter;

private:
};


class	MapperUNL_KS7008 : public Mapper
{
public:
	MapperUNL_KS7008( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){}
	void	LoadState( LPBYTE p ){}

private:
};



class	MapperUNL_LH09 : public Mapper
{
public:
	MapperUNL_LH09( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){}
	void	LoadState( LPBYTE p ){}

private:
};

class	MapperUNL_LE05 : public Mapper
{
public:
	MapperUNL_LE05( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){}
	void	LoadState( LPBYTE p ){}

private:
};
class	MapperUNL_BB : public Mapper
{
public:
	MapperUNL_BB( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){}
	void	LoadState( LPBYTE p ){}

private:
	BYTE reg, chr;
};

class	MapperUNL_LH52 : public Mapper
{
public:
	MapperUNL_LH52( NES* parent ) : Mapper(parent) {}

	void	Reset(){SetPROM_32K_Bank(0xe,0xd,0xe,0xf );}
	void	Write( WORD A, BYTE V ){SetPROM_8K_Bank(4,V&0xf);}

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){}
	void	LoadState( LPBYTE p ){}

private:
	BYTE reg[8], cmd;
};

class	MapperUNL_KS7037 : public Mapper
{
public:
	MapperUNL_KS7037( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );
	void	WriteLow( WORD addr, BYTE data );
	void	Sync();

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){}
	void	LoadState( LPBYTE p ){}

private:
	BYTE reg[8], cmd;
};


class	MapperUNL_AC08 : public Mapper
{
public:
	MapperUNL_AC08( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );
	void	ExWrite( WORD A, BYTE V );
	//void	Sync();

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){}
	void	LoadState( LPBYTE p ){}

private:
	BYTE reg, mirr;
};

//yoko
class	MapperUNL_YOKO : public Mapper
{
public:
	MapperUNL_YOKO( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );
	void	WriteLow( WORD A, BYTE V )
	{
		if( (A>=0x5400)&&(A<0x6000) )
			low[A & 3] = V;
	}
	BYTE	ReadLow ( WORD A )
	{
		if( (A>=0x5400)&&(A<0x6000) )
		return low[A & 3];
		else
		{
			if( (A>=0x5000)&&(A<=0x53ff))
			{
				BYTE b=Mapper::ReadLow(A);
				return (b&0xFC)|dip ;
			}

		} 
		BYTE b=Mapper::ReadLow(A);
		return b;
	}

	void	Clock( INT scanline );
	void	Sync(void);

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p ){}
	void	LoadState( LPBYTE p ){}

protected:
	BYTE mode, bank, reg[8], low[4], dip, IRQa;
	int IRQCount;

private:
};