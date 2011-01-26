//////////////////////////////////////////////////////////////////////////
// Nintendo MMC3                                                        //
//////////////////////////////////////////////////////////////////////////
MMC3::MMC3( NES* parent ) : Mapper(parent)
{
	UpdateChr    = &MMC3::Mmc3_UpdateChr2p;
	UpdatePrg    = &MMC3::Mmc3_UpdatePrg2p;
	GetChrSource = &MMC3::Mmc3_GetChrSource;

	Poke_8000 = &MMC3::Poke_Mmc3_8000;
	Poke_8001 = &MMC3::Poke_Mmc3_8001;
	Poke_A000 = &MMC3::Poke_Mmc3_A000;
	Poke_A001 = &MMC3::Poke_Mmc3_A001;
	Poke_C000 = &MMC3::Poke_Mmc3_C000;
	Poke_C001 = &MMC3::Poke_Mmc3_C001;
	Poke_E000 = &MMC3::Poke_Mmc3_E000;
	Poke_E001 = &MMC3::Poke_Mmc3_E001;
}

void MMC3::Reset()
{	
	int i;
	
	ctrl0 = 0;
	ctrl1 = 0;	
	
	for (i=0; i < 8; ++i)
		chr[i] = i;
	
	prg[0] = 0x00;
	prg[1] = 0x01;
	prg[2] = 0x3E;
	prg[3] = 0x3F;
	
	count = 0;
	latch = 0;
	reload = 0;
	enabled = 0;
	
	Mmc3_UpdatePrg (); 
	Mmc3_UpdateChr ();	
}

void MMC3::Write( WORD A, BYTE V )
{
	switch( A & 0xE001 ) 
	{
		case	0x8000:	(this->*Poke_8000)(A,V);break;	
		case	0x8001:	(this->*Poke_8001)(A,V);break;	
		case	0xA000:	(this->*Poke_A000)(A,V);break;	
		case	0xA001:	(this->*Poke_A001)(A,V);break;	
		case	0xC000:	(this->*Poke_C000)(A,V);break;	
		case	0xC001:	(this->*Poke_C001)(A,V);break;				
		case	0xE000:	(this->*Poke_E000)(A,V);break;
		case	0xE001:	(this->*Poke_E001)(A,V);break;
	}
}

void MMC3::Poke_Mmc3_8000(uint32 address,uint8 data)
{
	const unsigned int diff = ctrl0 ^ data;
	ctrl0 = data;

	if (diff & 0x40)
	{
		const unsigned int v[2] =
		{
			prg[(data >> 5 & 0x2) ^ 0],
			prg[(data >> 5 & 0x2) ^ 2]
		};

		(this->*UpdatePrg)( 0x0000, v[0] );
		(this->*UpdatePrg)( 0x4000, v[1] );
	}

	if (diff & 0x80)
		Mmc3_UpdateChr();
}
void MMC3::Poke_Mmc3_8001(uint32 address,uint8 data)
{
	unsigned int addr = ctrl0 & 0x7;

	if (addr < 6)
	{
		unsigned int base = ctrl0 << 5 & 0x1000;

		if (addr < 2)
		{
			addr <<= 1;
			base |= addr << 10;
			(this->*UpdateChr)( base | 0x0000, (chr[addr+0] = data & 0xFE) );
			(this->*UpdateChr)( base | 0x0400, (chr[addr+1] = data | 0x01) );
		}
		else
		{
			(this->*UpdateChr)( (base ^ 0x1000) | (addr-2) << 10, (chr[addr+2] = data) );
		}
	}
	else
	{
		(this->*UpdatePrg)( (addr == 6) ? (ctrl0 << 8 & 0x4000) : 0x2000, (prg[addr-6] = data & 0x3F) );
	}		
}
void MMC3::Poke_Mmc3_A000(uint32 address,uint8 data)
{
	if( !nes->rom->Is4SCREEN())
	{
		if( data & 0x01 ) 
			SetVRAM_Mirror( VRAM_HMIRROR );
		else
			SetVRAM_Mirror( VRAM_VMIRROR );
	}
}

void MMC3::Poke_Mmc3_A001(uint32 address,uint8 data)
{
	ctrl1 = data;	
}

//////////////////////////////////////////////////////////////////////////
// MMC3 IRQ
//////////////////////////////////////////////////////////////////////////
void MMC3::Poke_Mmc3_C000(uint32 address,uint8 data)
{
	count = data;
	reload = 0;
}
void MMC3::Poke_Mmc3_C001(uint32 address,uint8 data)
{
	latch = data;
	reload = 0;
}
void MMC3::Poke_Mmc3_E000(uint32 address,uint8 data)
{
	enabled = 0;
	reload = 0;
	nes->cpu->ClrIRQ( IRQ_MAPPER );
}
void MMC3::Poke_Mmc3_E001(uint32 address,uint8 data)
{
	enabled = 1;
	reload = 0;
}

void MMC3::HSync( int scanline )
//void MMC3::Mmc3_HSync(uint32 scanline)
{
	if( (scanline >= 0 && scanline <= 239) ) {
	if(nes->ppu->IsDispON())
	{
		if( enabled && !reload ) {
			if( scanline == 0 ) {
				if( count ) {
					count -= 1;
				}
			}
			if(!(count)){
				reload = 0xFF;
				count = latch;
				nes->cpu->SetIRQ( IRQ_MAPPER );
			}
			count--;
		}
	}
	}
}

void MMC3::Poke_Nop(uint32 addr,uint8 data)
{
	return;
}



void MMC3::Mmc3_SwapChr1K(uint8 page,uint32 bank)
{
	if((this->*GetChrSource)(bank))
	{
		SetCRAM_1K_Bank( page, bank );
	}else{
		SetVROM_1K_Bank( page, bank );
	}
}

void MMC3::Mmc3_SwapChr2K(uint8 page,uint32 bank)
{
	if((this->*GetChrSource)(bank))
	{
		SetCRAM_2K_Bank( page, bank );
	}else{
		SetVROM_2K_Bank( page, bank );
	}
}

void MMC3::Mmc3_UpdatePrg2p(unsigned int addr,unsigned int bank)
{
	SetPROM_8K_Bank( (addr>>13)+4, bank);
}

void MMC3::Mmc3_UpdateChr2p(unsigned int addr,unsigned int bank)
{
	Mmc3_SwapChr1K(addr>>10,bank);
}


void MMC3::Mmc3_UpdatePrg()
{
	const unsigned int x = ctrl0 >> 5 & 0x2;
		
	(this->*UpdatePrg)( 0x0000, prg[0^x] );
	(this->*UpdatePrg)( 0x2000, prg[1^0] );
	(this->*UpdatePrg)( 0x4000, prg[2^x] );
	(this->*UpdatePrg)( 0x6000, prg[3^0] );		
}

void MMC3::Mmc3_UpdateChr()
{
	const unsigned int x = ctrl0 >> 5 & 0x4;
	unsigned int i=0;
	for (i=0; i < 8; ++i)
		(this->*UpdateChr)( i * 0x400, chr[i^x] );
}

unsigned int MMC3::Mmc3_GetChrSource(unsigned int dummy){return 0;}


void	MMC3::SaveState( LPBYTE p )
{
	//没写好
}
void	MMC3::LoadState( LPBYTE p )
{
	//没写好
}





// ----------------------------------------------------------------------
// ------------------------- Generic MM3 Code ---------------------------
// ----------------------------------------------------------------------

void fceuMMC3::FixMMC3PRG(int V)
{
 if(V&0x40)
 {
  (this->*pwrap)(0xC000,DRegBuf[6]);
  (this->*pwrap)(0x8000,~1);
 }
 else
 {
  (this->*pwrap)(0x8000,DRegBuf[6]);
  (this->*pwrap)(0xC000,~1);
 }
   (this->*pwrap)(0xA000,DRegBuf[7]);
   (this->*pwrap)(0xE000,~0);
}

void fceuMMC3::FixMMC3CHR(int V)
{
 int cbase=(V&0x80)<<5;

 (this->*cwrap)((cbase^0x000),DRegBuf[0]&(~1));
 (this->*cwrap)((cbase^0x400),DRegBuf[0]|1);
 (this->*cwrap)((cbase^0x800),DRegBuf[1]&(~1));
 (this->*cwrap)((cbase^0xC00),DRegBuf[1]|1);

 (this->*cwrap)(cbase^0x1000,DRegBuf[2]);
 (this->*cwrap)(cbase^0x1400,DRegBuf[3]);
 (this->*cwrap)(cbase^0x1800,DRegBuf[4]);
 (this->*cwrap)(cbase^0x1c00,DRegBuf[5]);
}

void	fceuMMC3::Reset()
{
 IRQCount=IRQLatch=IRQa=MMC3_cmd=0;
	
 mmc3opts = 0;
 isRevB = 1;
  
  A001B=A000B=0;
  //setmirror(1);

 DRegBuf[0]=0;
 DRegBuf[1]=2;
 DRegBuf[2]=4;
 DRegBuf[3]=5;
 DRegBuf[4]=6;
 DRegBuf[5]=7;
 DRegBuf[6]=0;
 DRegBuf[7]=1;
 
 EXPREGS[4]=0;
 
 unromchr=dipswitch=0;
 
 switch(iMapper)
 {
	case 194: Reset194(); break;
	case 199: Reset199(); break;
	case SACHEN_STREETHEROES: MSHReset();break;
	case BMC_SUPER_24IN1: Super24Reset();break;
	case FK23C: BMCFK23CReset();break;
	case FK23CA: BMCFK23CAReset();break;
 }

 FixMMC3PRG(0);
 FixMMC3CHR(0);
}


void fceuMMC3::MMC3_CMDWrite(uint32 A, uint8 V)
{
 switch(A&0xE001)
 {
  case 0x8000:
       if((V&0x40) != (MMC3_cmd&0x40))
       {
          FixMMC3PRG(V);
       }
       if((V&0x80) != (MMC3_cmd&0x80))
          FixMMC3CHR(V);
       MMC3_cmd = V;
       break;
  case 0x8001:
       {
        int cbase=(MMC3_cmd&0x80)<<5;
        DRegBuf[MMC3_cmd&0x7]=V;
        switch(MMC3_cmd&0x07)
        {
         case 0: (this->*cwrap)((cbase^0x000),V&(~1));
                 (this->*cwrap)((cbase^0x400),V|1);
                 break;
         case 1: (this->*cwrap)((cbase^0x800),V&(~1));
                 (this->*cwrap)((cbase^0xC00),V|1);
                 break;
         case 2: (this->*cwrap)(cbase^0x1000,V);
                 break;
         case 3: (this->*cwrap)(cbase^0x1400,V);
                 break;
         case 4: (this->*cwrap)(cbase^0x1800,V);
                 break;
         case 5: (this->*cwrap)(cbase^0x1C00,V);
                 break;
         case 6:
                 if(MMC3_cmd&0x40)
                    (this->*pwrap)(0xC000,V);
                 else
                    (this->*pwrap)(0x8000,V);
                 break;
         case 7:
                 (this->*pwrap)(0xA000,V);
                 break;
        }
       }
       break;
  case 0xA000:
       if(mwrap) (this->*mwrap)(V);
       break;
  case 0xA001:
       A001B=V;
       break;
 }
}

void fceuMMC3::MMC3_IRQWrite(uint32 A, uint8 V)
{
 switch(A&0xE001)
 {
  case 0xC000:IRQLatch=V;break;
  case 0xC001:IRQReload=1;break;
  case 0xE000:nes->cpu->ClrIRQ( IRQ_MAPPER );IRQa=0;break;
  case 0xE001:IRQa=1;break;
 }
}


void fceuMMC3::ClockMMC3Counter(void)
{
 int count = IRQCount;
 if(!count || IRQReload)
 {
    IRQCount = IRQLatch;
    IRQReload = 0;
 }
 else
    IRQCount--;
 if((count|isRevB) && !IRQCount)
 {
    if(IRQa)
    {
		nes->cpu->SetIRQ( IRQ_MAPPER );
    }
 }
}

void fceuMMC3::GENCWRAP(uint32 A, uint8 V)
{
   SetVROM_1K_Bank(A>>10,V);    // Business Wars NEEDS THIS for 8K CHR-RAM
}

void fceuMMC3::GENPWRAP(uint32 A, uint8 V)
{
	SetPROM_8K_Bank(A>>13,V&0x3F);
}

void fceuMMC3::GENMWRAP(uint8 V)
{
 A000B=V;
 if( !nes->rom->Is4SCREEN() ) 
	SetVRAM_Mirror((V&1)^1);
}

void fceuMMC3::GENNOMWRAP(uint8 V)
{
 A000B=V;
}

void	fceuMMC3::Write( WORD A, BYTE V ){	return (this->*pWrite)(A,V);}

void	fceuMMC3::Mmc3Write( WORD A, BYTE V )
{
	if(A<0xC000)
		MMC3_CMDWrite(A,V);
	else
		MMC3_IRQWrite(A,V);
}

BYTE	fceuMMC3::ReadLow( WORD addr ){	return (this->*pReadLow)(addr);}
BYTE	fceuMMC3::Mmc3ReadLow( WORD addr )
{
	if( addr >= 0x5000 && addr <= 0x5FFF ) {
		return	XRAM[addr-0x4000];
	}else{
		return	Mapper::ReadLow( addr );
	}
}

void	fceuMMC3::WriteLow( WORD addr, BYTE data ){	(this->*pWriteLow)(addr,data);}

void	fceuMMC3::Mmc3WriteLow( WORD addr, BYTE data )
{
	if( addr >= 0x5000 && addr <= 0x5FFF ) {
		XRAM[addr-0x4000] = data;
	} else {
		Mapper::WriteLow( addr, data );
	}
}

void	fceuMMC3::Read( WORD A, BYTE V )
{
	(this->*pRead)(A,V);
}

void	fceuMMC3::Mmc3Read( WORD addr, BYTE data )
{
}

void	fceuMMC3::HSync( INT scanline )
{
	if( (scanline >= 0 && scanline <= 239) )
	{
		if(nes->ppu->IsDispON())
		ClockMMC3Counter();
	}
}


fceuMMC3::fceuMMC3( NES* parent,int imap):Mapper(parent)
{
 iMapper = imap;
 pwrap=&fceuMMC3::GENPWRAP;
 cwrap=&fceuMMC3::GENCWRAP;
 mwrap=&fceuMMC3::GENMWRAP;

 pWrite   =&fceuMMC3::Mmc3Write;
 pWriteLow=&fceuMMC3::Mmc3WriteLow;
 pReadLow =&fceuMMC3::Mmc3ReadLow;
 pRead    =&fceuMMC3::Mmc3Read;

 tekker = 0;
}


//mapper 194
void fceuMMC3::M194CW(uint32 A, uint8 V)
{
  if(V<=1) //Dai-2-Ji - Super Robot Taisen (As).nes
    SetCRAM_1K_Bank(A>>10,V);
  else
    SetVROM_1K_Bank(A>>10,V);
}
void fceuMMC3::Reset194()
{ 
	cwrap=&fceuMMC3::M194CW;
}


//mapper 199
void fceuMMC3::Reset199()
{
  EXPREGS[0]=~1;
  EXPREGS[1]=~0;
  EXPREGS[2]=1;
  EXPREGS[3]=3;

  pWrite=&fceuMMC3::M199Write;

  cwrap=&fceuMMC3::M199CW;
  pwrap=&fceuMMC3::M199PW;
  mwrap=&fceuMMC3::M199MW;
}
void fceuMMC3::M199PW(uint32 A, uint8 V)
{
  SetPROM_8K_Bank(A>>13,V);
  SetPROM_8K_Bank(0xC000>>13,EXPREGS[0]);
  SetPROM_8K_Bank(0xE000>>13,EXPREGS[1]);
}
void fceuMMC3::M199CW(uint32 A, uint8 V)
{
#define CHECK_C_V(X,Y,Z) if(X) SetCRAM_1K_Bank(Y,Z); else SetVROM_1K_Bank(Y,Z);

	CHECK_C_V((V<8)?0x10:0x00,A>>10,V)

  CHECK_C_V((DRegBuf[0]<8)?0x10:0x00,0x0000>>10,DRegBuf[0])
  CHECK_C_V((EXPREGS[2]<8)?0x10:0x00,0x0400>>10,EXPREGS[2])
  CHECK_C_V((DRegBuf[1]<8)?0x10:0x00,0x0800>>10,DRegBuf[1])
  CHECK_C_V((EXPREGS[3]<8)?0x10:0x00,0x0c00>>10,EXPREGS[3])
}
void fceuMMC3::M199MW(uint8 V)
{
	if( !nes->rom->Is4SCREEN() ) 
	switch(V&3)
	{
		case 0: SetVRAM_Mirror(VRAM_VMIRROR); break;
		case 1: SetVRAM_Mirror(VRAM_HMIRROR); break;
		case 2: SetVRAM_Mirror(VRAM_MIRROR4L); break;
		case 3: SetVRAM_Mirror(VRAM_MIRROR4H); break;
	}
}
void fceuMMC3::M199Write(uint16 A, uint8 V)
{
  if((A==0x8001)&&(MMC3_cmd&8))
  {
    EXPREGS[MMC3_cmd&3]=V;
    FixMMC3PRG(MMC3_cmd);
    FixMMC3CHR(MMC3_cmd);
  }
  else    
    if(A<0xC000)
      MMC3_CMDWrite(A,V);
    else
      MMC3_IRQWrite(A,V);
}

//SACHEN_STREETHEROES
void fceuMMC3::MSHCW(uint32 A, uint8 V)
{
  if(EXPREGS[0]&0x40)
    SetCRAM_8K_Bank(0);
  else
  {
    if(A<0x800)
      SetVROM_1K_Bank(A>>10,V|((EXPREGS[0]&8)<<5));
    else if(A<0x1000)
      SetVROM_1K_Bank(A>>10,V|((EXPREGS[0]&4)<<6));
    else if(A<0x1800)
      SetVROM_1K_Bank(A>>10,V|((EXPREGS[0]&1)<<8));
    else
      SetVROM_1K_Bank(A>>10,V|((EXPREGS[0]&2)<<7));
  }
}

void fceuMMC3::MSHWrite(uint16 A, uint8 V)
{
	if(A=0x4100)
	{
	  EXPREGS[0]=V;
	  FixMMC3CHR(MMC3_cmd);
	}
	else
		Mapper::WriteLow( A, V );
}

BYTE fceuMMC3::MSHRead(uint16 A)
{
	if(A=0x4100)
	{
		return(tekker);
	}
	else
	return	Mapper::ReadLow( A );
}

void fceuMMC3::MSHMWRAP(uint8 V)
{
 A000B=V;
 if( !nes->rom->Is4SCREEN() ) 
	SetVRAM_Mirror(VRAM_MIRROR4);
}

void fceuMMC3::MSHReset()
{
  //MMC3RegReset();
  cwrap=&fceuMMC3::MSHCW;
  pWriteLow=&fceuMMC3::MSHWrite;
  pReadLow =&fceuMMC3::MSHRead;
  mwrap    =&fceuMMC3::MSHMWRAP;
  tekker^=0xFF; 
}


//24in1
void fceuMMC3::Super24PW(uint32 A, uint8 V)
{
  const int masko8[8]={63,31,15,1,3,0,0,0};
  uint32 NV=V&masko8[EXPREGS[0]&7];
  NV|=(EXPREGS[1]<<1);
  //setprg8r((NV>>6)&0xF,A,NV);
  SetPROM_8K_Bank(A>>13,NV);
}
void fceuMMC3::Super24CW(uint32 A, uint8 V)
{
  if(EXPREGS[0]&0x20)
    SetCRAM_1K_Bank(A>>10,V);
  else
  {
    uint32 NV=V|(EXPREGS[2]<<3);
    //setchr1r((NV>>9)&0xF,A,NV);
	SetVROM_1K_Bank(A>>10,NV);
  }
}
void fceuMMC3::Super24Write(uint16 A, uint8 V)
{
  switch(A)
  {
    case 0x5FF0: EXPREGS[0]=V;
                 FixMMC3PRG(MMC3_cmd);
                 FixMMC3CHR(MMC3_cmd);
                 break;
    case 0x5FF1: EXPREGS[1]=V;
                 FixMMC3PRG(MMC3_cmd);
                 break;
    case 0x5FF2: EXPREGS[2]=V;
                 FixMMC3CHR(MMC3_cmd);
                 break;
  }
  if(A<0x5000)
  {
	  Mapper::WriteLow(A,V);
	  }
}

void fceuMMC3::Super24Reset()
{
  EXPREGS[0]=0x24;
  EXPREGS[1]=159;
  EXPREGS[2]=0;

  cwrap=&fceuMMC3::Super24CW;
  pwrap=&fceuMMC3::Super24PW;
  pWriteLow =&fceuMMC3::Super24Write;
}


//fk23c
void fceuMMC3::BMCFK23CCW(uint32 A, uint8 V)
{
  if(EXPREGS[0]&0x40)
    SetVROM_8K_Bank(EXPREGS[2]|unromchr);
  else
  {
    uint16 base=(EXPREGS[2]&0x7F)<<3;
    SetVROM_1K_Bank(A>>10,V|base);
    if(EXPREGS[3]&2)
    {
      SetVROM_1K_Bank(0x0400>>10,EXPREGS[6]|base);
      SetVROM_1K_Bank(0x0C00>>10,EXPREGS[7]|base);
    }
  }
}
void fceuMMC3::BMCFK23CPW(uint32 A, uint8 V)
{
  uint32 bank = (EXPREGS[1] & 0x1F);
  uint32 block = (EXPREGS[1] & 0x60);
  uint32 extra = (EXPREGS[3]&2);
  switch(EXPREGS[0]&7)
  {
   case 0: SetPROM_8K_Bank(A>>13, (block << 1) | (V & 0x3F));
           if(extra)
           {
            SetPROM_8K_Bank(0xC000>>13,EXPREGS[4]);
            SetPROM_8K_Bank(0xE000>>13,EXPREGS[5]);
           }
           break;
   case 1: SetPROM_8K_Bank(A>>13, ((EXPREGS[1] & 0x70) << 1) | (V & 0x1F));
           if(extra)
           {
            SetPROM_8K_Bank(0xC000>>13,EXPREGS[4]);
            SetPROM_8K_Bank(0xE000>>13,EXPREGS[5]);
           }
           break;
   case 2: SetPROM_8K_Bank(A>>13, ((EXPREGS[1] & 0x78) << 1) | (V & 0x0F));
           if(extra)
           {
            SetPROM_8K_Bank(0xC000>>13,EXPREGS[4]);
            SetPROM_8K_Bank(0xE000>>13,EXPREGS[5]);
           }
           break;  
   case 3: SetPROM_16K_Bank(0x8000>>13,(bank + block));
           SetPROM_16K_Bank(0xC000>>13,(bank + block));
           break;
   case 4: SetPROM_32K_Bank((bank + block) >> 1);
           break;
  }
}


void fceuMMC3::BMCFK23CHiWrite(uint16 A, uint8 V)
{
  if(EXPREGS[0]&0x40)
  {
    if(EXPREGS[0]&0x30)
      unromchr=0;
    else
    {
      unromchr=V&3;
      FixMMC3CHR(MMC3_cmd);
    }
  }
  else
  {
    if((A==0x8001)&&(EXPREGS[3]&2&&MMC3_cmd&8))
    {
      EXPREGS[4|(MMC3_cmd&3)]=V;
      FixMMC3PRG(MMC3_cmd);
      FixMMC3CHR(MMC3_cmd);
    }
    else    
      if(A<0xC000)
        MMC3_CMDWrite(A,V);
      else
        MMC3_IRQWrite(A,V);
  }
}

void fceuMMC3::BMCFK23CWrite(uint16 A, uint8 V)
{
	if(A&(1<<(dipswitch+4)))
  {
    EXPREGS[A&3]=V;
    FixMMC3PRG(MMC3_cmd);
    FixMMC3CHR(MMC3_cmd);
  }
}

void fceuMMC3::BMCFK23CReset()
{	
  cwrap =&fceuMMC3::BMCFK23CCW;
  pwrap =&fceuMMC3::BMCFK23CPW;
  pWrite=&fceuMMC3::BMCFK23CHiWrite;
  pWriteLow=&fceuMMC3::BMCFK23CWrite;

  /*
  EXPREGS[0]=4;
  EXPREGS[1]=0xFF;
  EXPREGS[2]=EXPREGS[3]=0;
  EXPREGS[4]=EXPREGS[5]=EXPREGS[6]=EXPREGS[7]=0xFF;*/

  dipswitch++;
  dipswitch&=7;
  EXPREGS[0]=EXPREGS[1]=EXPREGS[2]=EXPREGS[3]=0;
  EXPREGS[4]=EXPREGS[5]=EXPREGS[6]=EXPREGS[7]=0xFF;

  FixMMC3PRG(MMC3_cmd);
  FixMMC3CHR(MMC3_cmd);
}

void fceuMMC3::BMCFK23CAReset()
{
  cwrap =&fceuMMC3::BMCFK23CCW;
  pwrap =&fceuMMC3::BMCFK23CPW;
  pWrite=&fceuMMC3::BMCFK23CHiWrite;
  pWriteLow=&fceuMMC3::BMCFK23CWrite;

  dipswitch++;
  dipswitch&=7;
  EXPREGS[0]=EXPREGS[1]=EXPREGS[2]=EXPREGS[3]=0;
  EXPREGS[4]=EXPREGS[5]=EXPREGS[6]=EXPREGS[7]=0xFF;
 
  FixMMC3PRG(MMC3_cmd);
  FixMMC3CHR(MMC3_cmd);
}