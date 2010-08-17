void	map0_3208cn::Reset()
{
	SetPROM_32K_Bank( 0 );	
}


void	map0_3208cn::Write( WORD addr, BYTE data )
{	
	//addr = 0x8000,ÇÐ»» PROM
	if(addr==0x8032)
		SetPROM_32K_Bank(data);
	if(addr==0x8416)
		SetPROM_16K_Bank(4,data);
	if(addr==0x8616)
		SetPROM_16K_Bank(6,data);
	if(addr==0x8408)
		SetPROM_8K_Bank(4,data);
	if(addr==0x8508)
		SetPROM_8K_Bank(5,data);
	if(addr==0x8608)
		SetPROM_8K_Bank(6,data);
	if(addr==0x8708)
		SetPROM_8K_Bank(7,data);
	
	//addr = 0x8001,ÇÐ»» VROM
	if(addr==0x9008)
		SetVROM_8K_Bank(data);
	if(addr==0x9004)
		SetVROM_4K_Bank(0,data);
	if(addr==0x9404)
		SetVROM_4K_Bank(4,data);
	if(addr==0x9001)
		SetVROM_1K_Bank(0,data);
	if(addr==0x9101)
		SetVROM_1K_Bank(1,data);
	if(addr==0x9201)
		SetVROM_1K_Bank(2,data);
	if(addr==0x9301)
		SetVROM_1K_Bank(3,data);
	if(addr==0x9401)
		SetVROM_1K_Bank(4,data);
	if(addr==0x9501)
		SetVROM_1K_Bank(5,data);
	if(addr==0x9601)
		SetVROM_1K_Bank(6,data);
	if(addr==0x9701)
		SetVROM_1K_Bank(7,data);
}


void	GeniusMerioBros::Reset()
{
	SetPROM_32K_Bank( 0 );	
	memcpy(WRAM, PROM+0x2000*4,0x800);
}

BYTE	GeniusMerioBros::ReadLow ( WORD A )
{
	if( A >= 0x6000 && A <= 0x6FFF ) {
		return	CPU_MEM_BANK[0][A&0x7FF];
	}
	else if( A >= 0x7000 && A <= 0x7FFF ) {
		return	XRAM[A&0x7FF];
	}
	else 
	return	(BYTE)(A>>8);

}
void	GeniusMerioBros::WriteLow( WORD A, BYTE V )
{
	if( A >= 0x7000 && A <= 0x7FFF ) {
		XRAM[A&0x7FF] = V;
	}	else
	if( A >= 0x6000 && A <= 0x6FFF ) {
		CPU_MEM_BANK[A>>13][A&0x1FFF] = V;
	}
}







void	smb2j::Reset()
{
  prg=~0;
  
		IRQa=0;
		IRQCount=0;
  //setprg4r(1,0x5000,1);
  //setprg8r(1,0x6000,1);
	SetPROM_32K_Bank( prg );	
	SetVROM_8K_Bank( 0 );
	memcpy(WRAM , PROM+0x2000*9,0x800);
}


void	smb2j::Write(WORD A, BYTE V )// (0x4020,0xffff)
{
	if(A==0x4022)
	{
		prg=V&1;
		//setprg4r(1,0x5000,1);
		//setprg8r(1,0x6000,1);
		SetPROM_32K_Bank( prg );	
		//SetVROM_8K_Bank( 0 );
	}
	if(A==0x4122)
	{
		IRQa=V;
		IRQCount=0;
		nes->cpu->ClrIRQ( IRQ_MAPPER );
	}
}

void	smb2j::WriteLow( WORD A, BYTE V )
{
	if(A==0x4022)
	{
		prg=V&1;
		//setprg4r(1,0x5000,1);
		//setprg8r(1,0x6000,1);
		SetPROM_32K_Bank( 1 );	
		//SetVROM_8K_Bank( 0 );
	}
	if(A==0x4122)
	{
		IRQa=V;
		IRQCount=0;
		nes->cpu->ClrIRQ( IRQ_MAPPER );
	}
}
void	smb2j::HSync( INT a )
{
	if(IRQa)
	{
		IRQCount+=a*3;
		if((IRQCount>>12)==IRQa)
			nes->cpu->SetIRQ( IRQ_MAPPER );
  }
}

BYTE	smb2j::ReadLow ( WORD addr )
{
	if(addr>=0x5000)
	{
		addr =24576;//(addr+0xC000);
		BYTE ch = CPU_MEM_BANK[0][addr&0x800];//14000		
		return WRAM[addr&0x800];
	}
	//return *prg.Source().Mem( address + prgLowerOffset );
	return (BYTE)(addr>>8);
}




