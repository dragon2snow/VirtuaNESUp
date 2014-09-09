//////////////////////////////////////////////////////////////////////////
// Mapper042  Mario Baby                                                //
//////////////////////////////////////////////////////////////////////////
void	Mapper042::Reset()
{
	irq_enable = 0;
	irq_counter = 0;

	SetPROM_8K_Bank( 3, 0 );
	SetPROM_32K_Bank( PROM_8K_SIZE-4, PROM_8K_SIZE-3, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );

	if( VROM_1K_SIZE ) {
		SetVROM_8K_Bank( 0 );
	}
}

void	Mapper042::Write( WORD addr, BYTE data )
{
	switch( addr & 0xE003 ) {
		case	0xE000:
			SetPROM_8K_Bank( 3, data&0x0F );
			break;

		case	0xE001:
			if( data&0x08 ) SetVRAM_Mirror( VRAM_HMIRROR );
			else		SetVRAM_Mirror( VRAM_VMIRROR );
			break;

		case	0xE002:
			if( data&0x02 ) {
				irq_enable = 0xFF;
			} else {
				irq_enable = 0;
				irq_counter = 0;
			}
			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;
	}
}

void	Mapper042::HSync( INT scanline )
{
	nes->cpu->ClrIRQ( IRQ_MAPPER );
	if( irq_enable ) {
		if( irq_counter < 215 ) {
			irq_counter++;
		}
		if( irq_counter == 215 ) {
			irq_enable = 0;
//			nes->cpu->IRQ();
			nes->cpu->SetIRQ( IRQ_MAPPER );
		}
	}
}

void	Mapper042::SaveState( LPBYTE p )
{
	p[0] = irq_enable;
	p[1] = irq_counter;
}

void	Mapper042::LoadState( LPBYTE p )
{
	irq_enable  = p[0];
	irq_counter = p[1];
}


void	MapperUNL_KS7008::Reset()
{
	SetPrg8r(1,0x6000,0);
	SetPrg32r(2,0x8000,0);

	SetVRAM_Mirror( VRAM_VMIRROR );
}

void	MapperUNL_KS7008::Write( WORD A, BYTE V )
{
	if(A==0x8001)
	{
		int v_bank = (V/2)&0xF;
		if(v_bank<8)
			SetPrg8r(1,0x6000,v_bank);
		else
			SetPrg8r(0,0x6000,v_bank-8);
	}
}


void	MapperUNL_LH09::Reset()
{
	SetPROM_8K_Bank( 3, 0 );
	SetPROM_32K_Bank( PROM_8K_SIZE-4, PROM_8K_SIZE-3, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );
}

void	MapperUNL_LH09::Write( WORD A, BYTE data )
{
	if(A==0x8001)
		SetPROM_8K_Bank( 3, (data/2)&0xF );	
}


void	MapperUNL_LE05::Reset()
{
	SetPROM_Bank(3, PROM+0x6800, BANKTYPE_ROM);
	SetPROM_Bank(4, PROM +0x800, BANKTYPE_ROM);
	SetPROM_Bank(5, PROM+0x2800, BANKTYPE_ROM);
	SetPROM_Bank(6, PROM+0x4800, BANKTYPE_ROM);
	memcpy(&MRAM[  0x0],PROM,0x800);
	memcpy(&MRAM[0x800],PROM,0x800);
	memcpy(&MRAM[0x1000],PROM,0x800);
	memcpy(&MRAM[0x1800],PROM,0x800);
	SetPROM_Bank(7, MRAM, BANKTYPE_ROM);
	
	SetVROM_8K_Bank( 0 );
}

void	MapperUNL_LE05::Write( WORD A, BYTE V )
{
	SetVROM_8K_Bank(V&1);
	SetVRAM_Mirror( VRAM_VMIRROR );
}

void	MapperUNL_BB::Reset()
{
	reg = ~0;
	chr = 0;
  SetPrg8(0x6000,reg&3);
  SetPrg32(0x8000,~0);
  SetVROM_8K_Bank(chr&3);
}

void	MapperUNL_BB::Write( WORD A, BYTE V )
{
  if((A & 0x9000) == 0x8000)
    reg=chr=V;
  else
    chr=V&1; 

  SetPrg8(0x6000,reg&3);
  SetPrg32(0x8000,~0);
  SetVROM_8K_Bank(chr&3);
}


void	MapperUNL_KS7037::Reset()
{
	reg[0]=reg[1]=reg[2]=reg[3]=reg[4]=reg[5]=0;
	reg[7] = reg[6]=PROM_8K_SIZE-1;

	ZeroMemory(MRAM,0x2000*2);	
	memcpy(&MRAM[0x1000],PROM+0x3c*0x400,0x1000);//ok 
	SetPROM_Bank(3, MRAM, BANKTYPE_ROM);//6000~7FFF

	memcpy(&MRAM[0x2000],PROM+0x70*0x400,0x1000);//ok
	SetPROM_Bank(5, &MRAM[0x2000], BANKTYPE_ROM);//A000~BFFF

	Sync();
}

void	MapperUNL_KS7037::WriteLow( WORD A, BYTE V )
{
	if( (A<0x4020)||(A>0x5FFF) )
	{
		return Mapper::WriteLow(A,V);
	}
	
	if( (A>=0x6000)&&(A<0x7000) )
		MRAM[A-0x6000]=V;


	switch(A & 0xE001)
	{
		case 0x8000: cmd = V & 7; break;
		case 0x8001: reg[cmd] = V; Sync(); break;
	}
}

void	MapperUNL_KS7037::Write( WORD A, BYTE V )
{
	
	if( (A>=0xB000)&&(A<0xC000) )
		MRAM[A-0x8000]=V;

	if( (A>=0xA000)&&(A<0xC000) )
		return Mapper::Write(A,V);

	switch(A & 0xE001)
	{
		case 0x8000: cmd = V & 7; break;
		case 0x8001: reg[cmd] = V; Sync(); break;
	}
}


void	MapperUNL_KS7037::Sync()
{
  SetPROM_8K_Bank(4,reg[6]);//8000~9FFF
  SetPROM_8K_Bank(6,reg[7]);//C000~DFFF
  SetPROM_8K_Bank(7,PROM_8K_SIZE-1);//E000~FFFF
  SetVROM_8K_Bank( 0 );
  SetVRAM_Mirror(reg[2]&1,reg[4]&1,reg[3]&1,reg[5]&1);
}




//¶ñÄ§³Ç
void	MapperUNL_AC08::Reset()
{
	reg = 0;
	SetPrg8(0x6000, 0);
	SetPrg32r(1,0x8000, 0);
}
void	MapperUNL_AC08::ExWrite( WORD A, BYTE V )
{
	if(A==0x4025)
		SetVRAM_Mirror( ((V&8)>>3)^1 );
}
void	MapperUNL_AC08::Write( WORD A, BYTE V )
{
  if(A == 0x8001)              // Green Berret bank switching is only 100x xxxx xxxx xxx1 mask
   reg = (V >> 1) & 0xf;
  else
   reg = V & 0xf; 
	SetPrg8(0x6000, reg);
}



//yoko
void	MapperUNL_YOKO::Reset()
{
  mode = bank = 0;
  dip = 1;

  //dip = (dip + 1)&3;
  mode = bank = 0;
  Sync();
}

void	MapperUNL_YOKO::Write( WORD A, BYTE V )
{
	  switch(A & 0x8C17)
  {
    case 0x8000: bank=V; break;
    case 0x8400: mode=V; break;
    case 0x8800: IRQCount&=0xFF00; IRQCount|=V; nes->cpu->ClrIRQ( IRQ_MAPPER ); break;
    case 0x8801: IRQa=mode&0x80; IRQCount&=0xFF; IRQCount|=V<<8; break;
    case 0x8c00: reg[0]=V; break;
    case 0x8c01: reg[1]=V; break;
    case 0x8c02: reg[2]=V; break;
    case 0x8c10: reg[3]=V; break;
    case 0x8c11: reg[4]=V; break;
    case 0x8c16: reg[5]=V; break;
    case 0x8c17: reg[6]=V; break;
  }
  Sync();	
}

void	MapperUNL_YOKO::Clock( INT a )
{
//	if( (a >= 0 && a <= 239) ) 
	{
	if(IRQa)
	{
		IRQCount-=a;
		if(IRQCount<0)
		{
			nes->cpu->SetIRQ( IRQ_MAPPER );
			IRQa=0;
			IRQCount=0xFFFF;
		}
	}
	}
}

void MapperUNL_YOKO::Sync(void)
{
  SetVRAM_Mirror((mode & 1)^1);
  SetVROM_2K_Bank(0,reg[3]);
  SetVROM_2K_Bank(2,reg[4]);
  SetVROM_2K_Bank(4,reg[5]);
  SetVROM_2K_Bank(6,reg[6]);
  if(mode & 0x10)
  {
    uint32 base = (bank & 8) << 1;
    SetPROM_8K_Bank(4,(reg[0]&0x0f)|base);
    SetPROM_8K_Bank(5,(reg[1]&0x0f)|base);
    SetPROM_8K_Bank(6,(reg[2]&0x0f)|base);
    SetPROM_8K_Bank(7,0x0f|base);
  }
  else
  {
    if(mode & 8)
      SetPROM_32K_Bank(bank >> 1);
    else
    {
      SetPROM_16K_Bank(4,bank);
      SetPROM_8K_Bank(6,PROM_8K_SIZE-2);//PROM_8K_SIZE
	  SetPROM_8K_Bank(7,PROM_8K_SIZE-1);
    }
  }
}