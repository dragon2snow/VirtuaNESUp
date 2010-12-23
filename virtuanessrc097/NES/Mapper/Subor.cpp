//////////////////////////////////////////////////////////////////////////
// MapperSubor999                                                          //
//////////////////////////////////////////////////////////////////////////
void	MapperSubor999::Reset()
{
	reg5200 = reg5000 = 0;
	SetBank_CPU();
}

void	MapperSubor999::WriteLow( WORD addr, BYTE data )
{
	if(addr==0x5000){
		reg5000 = data;
		SetBank_CPU();
	}else if(addr==0x5200){
		reg5200 = data&7;
		SetBank_CPU();
	}
}

	
void MapperSubor999::SetBank_CPU(void)
{	
	if(reg5200<7){
		SetPROM_16K_Bank(4,reg5000);
		SetPROM_16K_Bank(6,0);
	}else{
		SetPROM_32K_Bank(reg5000);
	}
}


void	MapperSubor999::SaveState( LPBYTE p )
{
	p[ 0] = reg5000;
	p[ 1] = reg5200;
}

void	MapperSubor999::LoadState( LPBYTE p )
{
	reg5000 = p[ 0];
	reg5200 = p[ 1];
}
