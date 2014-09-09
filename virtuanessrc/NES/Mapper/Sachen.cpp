Sachen::Sachen( NES* parent,int imap ) : Mapper(parent)
{
	iMapper = imap;
	bFirst  = 1;

	pWrite     = NULL;
	pRead      = NULL;
	pReadLow   = NULL;
	pWriteLow  = NULL;
}

void	Sachen::Reset()
{
	if(bFirst)
	{
	switch(iMapper)
	{
		case 150:	S74LS374NPower();break;
	}
	bFirst = 0;
	}

	switch(iMapper)
	{
		case 146:	SA0161MReset();break;
		case 147:	TCU01Reset();break;
		case 148:	SA0037Reset();break;
		case 149:	SA0036Reset();break;
		case 150:	S74LS374NReset();break;
	}
}

// $8000-$FFFF Memory write
void	Sachen::Write( WORD A, BYTE V )
{
	if(pWrite)
		(this->*pWrite)(A,V);
	else
		Mapper::Write(A,V);
}

void	Sachen::Read( WORD A, BYTE V )
{
	if(pRead)
		(this->*pRead)(A,V);
	//else
	//	Mapper::Read(A,V);
}
	
BYTE	Sachen::ReadLow ( WORD A )
{
	if(pReadLow)
		return (this->*pReadLow)(A);
	else
		return Mapper::ReadLow(A);
}
void	Sachen::WriteLow( WORD A, BYTE V )
{
	if(pWriteLow)
		(this->*pWriteLow)(A,V);
	else
		Mapper::WriteLow(A,V);
}

//ÊµÌå´úÂë
void Sachen::S74LS374MSync(uint8 mirr)
{
  switch(mirr&3)
  {
    case 0:SetVRAM_Mirror(VRAM_VMIRROR);break;
    case 1:SetVRAM_Mirror(VRAM_HMIRROR);break;
    case 2:SetVRAM_Mirror(0,1,1,1);break;
    case 3:SetVRAM_Mirror(VRAM_MIRROR4L);break;
  }
}

void Sachen::S74LS374NSynco(void)
{
  SetPROM_32K_Bank(latch[0]);
  SetVROM_8K_Bank(latch[1]|latch[3]|latch[4]);
  S74LS374MSync(latch[2]);
}

void Sachen::S74LS374NWrite( WORD A, BYTE V )
{
  A&=0x4101;
  if(A==0x4100)
    cmd=V&7;
  else
  {
    switch(cmd)
    {
      case 2:latch[0]=V&1; latch[3]=(V&1)<<3;break;
      case 4:latch[4]=(V&1)<<2;break;
      case 5:latch[0]=V&7;break;
      case 6:latch[1]=V&3;break;
      case 7:latch[2]=V>>1;break;
    }
    S74LS374NSynco();
  }
}


BYTE Sachen::S74LS374NRead(WORD A)
{
  if((A&0x4100)==0x4100)
    return ((~cmd)&0x3F)^dip;
  else
	  return Mapper::ReadLow(A);
}

void Sachen::S74LS374NPower(void)
{
   dip=0;
   latch[0]=latch[1]=latch[2]=latch[3]=latch[4]=0;
   S74LS374NSynco();

   pWriteLow  = &Sachen::S74LS374NWrite;
   pReadLow   = &Sachen::S74LS374NRead;
}

void Sachen::S74LS374NReset(void)
{
   dip^=1;
   latch[0]=latch[1]=latch[2]=latch[3]=latch[4]=0;
   S74LS374NSynco();
}
//mapper 146
void Sachen::SA0161MReset(void)
{
  WSync=&Sachen::SA0161MSynco;
  
  pWriteLow = &Sachen::SAWrite;

  latch[0]=0;
  (this->*WSync)();
}
void Sachen::SAWrite(WORD A,BYTE V)
{
  if(A&0x100)
  {
    latch[0]=V;
	(this->*WSync)();
  }
}


//TCU01 mapper 147
void Sachen::TCU01Reset(void)
{
  latch[0]=0;
  
	pWrite = &Sachen::TCU01Write;
   pWriteLow    = &Sachen::TCU01Write;
  TCU01Synco();
}

void Sachen::TCU01Synco()
{
  SetPROM_32K_Bank(((latch[0]&0x80)>>6)|((latch[0]>>2)&1));
  SetVROM_8K_Bank((latch[0]>>3)&0xF);
}

void Sachen::TCU01Write(WORD A, BYTE V )
{
	if((A&0x103)==0x102)
  {
    latch[0]=V;
    TCU01Synco();
  }
}


//mapper 148 (SA0037)
void Sachen::SA0037Reset(void)
{
  WSync  = &Sachen::SA0161MSynco;
  pWrite = &Sachen::SADWrite;

  latch[0]=0;
  (this->*WSync)();
}

void Sachen::SA0161MSynco()
{
  SetPROM_32K_Bank((latch[0]>>3)&1);
  SetVROM_8K_Bank(latch[0]&7);
}

void Sachen::SADWrite(WORD A, BYTE V )
{
  latch[0]=V;
  (this->*WSync)();
}


//MAPPER 149 SA0036
void Sachen::SA0036Reset(void)
{
  WSync  = &Sachen::SA72007Synco;
  pWrite = &Sachen::SADWrite;

  latch[0]=0;
  (this->*WSync)();
}

void Sachen::SA009Synco()
{
  SetPROM_32K_Bank(0);
  SetVROM_8K_Bank(latch[0]&1);
}
void Sachen::SA72007Synco()
{
  SetPROM_32K_Bank(0);
  SetVROM_8K_Bank(latch[0]>>7);
}

//mapper 150 (S74LS374N)
/*void Sachen::S74LS374NReset(void)
{
}
*/














/*

static void S74LS374NRestore(int version)
{
   S74LS374NSynco();
}

void S74LS374N_Init(CartInfo *info)
{
  info->Power=S74LS374NPower;
  info->Reset=S74LS374NReset;
  GameStateRestore=S74LS374NRestore;
  AddExState(latch, 5, 0, "LATC");
  AddExState(&cmd, 1, 0, "CMD");
  AddExState(&dip, 1, 0, "DIP");
}

static void S74LS374NASynco(void)
{
  setprg32(0x8000,latch[0]);
  setchr8(latch[1]);
  S74LS374MSync(latch[2]);
}

static DECLFW(S74LS374NAWrite)
{
  A&=0x4101;
  if(A==0x4100)
    cmd=V&7;
  else
  {
    switch(cmd)
    {
      case 0:latch[0]=0;latch[1]=3;break;
      case 2:latch[3]=(V&1)<<3;break;
      case 4:latch[1]=(latch[1]&6)|(V&3);break;
      case 5:latch[0]=V&1;break;
      case 6:latch[1]=(latch[1]&1)|latch[3]|((V&3)<<1);break;
      case 7:latch[2]=V&1;break;
    }
    S74LS374NASynco();
  }
}

static void S74LS374NAPower(void)
{
   latch[0]=latch[2]=latch[3]=latch[4]=0;
   latch[1]=3;
   S74LS374NASynco();
   SetReadHandler(0x8000,0xFFFF,CartBR);
   SetWriteHandler(0x4100,0x7FFF,S74LS374NAWrite);
}

void S74LS374NA_Init(CartInfo *info)
{
  info->Power=S74LS374NAPower;
  GameStateRestore=S74LS374NRestore;
  AddExState(latch, 5, 0, "LATC");
  AddExState(&cmd, 1, 0, "CMD");
}

static int type;
static void S8259Synco(void)
{
  int x;
  setprg32(0x8000,latch[5]&7);

  if(!UNIFchrrama)        // No CHR RAM?  Then BS'ing is ok.
  {
    for(x=0;x<4;x++)
    {
      int bank;
      if(latch[7]&1)
        bank=(latch[0]&0x7)|((latch[4]&7)<<3);
      else
        bank=(latch[x]&0x7)|((latch[4]&7)<<3);
      switch (type)
      {
        case 00: bank=(bank<<1)|(x&1); setchr2(0x800*x,bank); break;
        case 01: setchr2(0x800*x,bank); break;
        case 02: bank=(bank<<2)|(x&3); setchr2(0x800*x,bank); break;
        case 03: bank=latch[x]&7;
                 switch (x&3)
                 {
                   case 01: bank|=(latch[4]&1)<<4;break;
                   case 02: bank|=(latch[4]&2)<<3;break;
                   case 03: bank|=((latch[4]&4)<<2)|((latch[6]&1)<<3);break;
                 }
                 setchr1(0x400*x,bank);
                 setchr4(0x1000,~0);
                 break;
      }
    }
  }
  if(!(latch[7]&1))
    S74LS374MSync(latch[7]>>1);
  else
    setmirror(MI_V);
}

static DECLFW(S8259Write)
{
  A&=0x4101;
  if(A==0x4100)
    cmd=V;
  else
  {
    latch[cmd&7]=V;
    S8259Synco();
  }
}

static void S8259Reset(void)
{
  int x;
  cmd=0;

  for(x=0;x<8;x++) latch[x]=0;
  setchr8(0);

  S8259Synco();
  SetReadHandler(0x8000,0xFFFF,CartBR);
  SetWriteHandler(0x4100,0x7FFF,S8259Write);
}

static void S8259Restore(int version)
{
  S8259Synco();
}

void S8259A_Init(CartInfo *info) // Kevin's Horton 141 mapper
{
  info->Power=S8259Reset;
  GameStateRestore=S8259Restore;
  AddExState(latch, 8, 0, "LATC");
  AddExState(&cmd, 1, 0, "CMD");
  type=0;
}

void S8259B_Init(CartInfo *info) // Kevin's Horton 138 mapper
{
  info->Power=S8259Reset;
  GameStateRestore=S8259Restore;
  AddExState(latch, 8, 0, "LATC");
  AddExState(&cmd, 1, 0, "CMD");
  type=1;
}

void S8259C_Init(CartInfo *info) // Kevin's Horton 139 mapper
{
  info->Power=S8259Reset;
  GameStateRestore=S8259Restore;
  AddExState(latch, 8, 0, "LATC");
  AddExState(&cmd, 1, 0, "CMD");
  type=2;
}

void S8259D_Init(CartInfo *info) // Kevin's Horton 137 mapper
{
  info->Power=S8259Reset;
  GameStateRestore=S8259Restore;
  AddExState(latch, 8, 0, "LATC");
  AddExState(&cmd, 1, 0, "CMD");
  type=3;
}

static void(*WSync)(void);

static DECLFW(SAWrite)
{
  if(A&0x100)
  {
    latch[0]=V;
    WSync();
  }
}

static void SAPower(void)
{
  latch[0]=0;
  WSync();
  SetReadHandler(0x8000,0xFFFF,CartBR);
  SetWriteHandler(0x4100,0x5FFF,SAWrite);
}

static void SARestore(int version)
{
  WSync();
}

static DECLFW(SADWrite)
{
  latch[0]=V;
  WSync();
}

static void SADPower(void)
{
  latch[0]=0;
  WSync();
  SetReadHandler(0x8000,0xFFFF,CartBR);
  SetWriteHandler(0x8000,0xFFFF,SADWrite);
}

static void SA0161MSynco()
{
  setprg32(0x8000,(latch[0]>>3)&1);
  setchr8(latch[0]&7);
}

static void SA72007Synco()
{
  setprg32(0x8000,0);
  setchr8(latch[0]>>7);
}

static void SA009Synco()
{
  setprg32(0x8000,0);
  setchr8(latch[0]&1);
}

static void SA72008Synco()
{
  setprg32(0x8000,(latch[0]>>2)&1);
  setchr8(latch[0]&3);
}

void SA0161M_Init(CartInfo *info)
{
  WSync=SA0161MSynco;
  GameStateRestore=SARestore;
  info->Power=SAPower;
  AddExState(&latch[0], 1, 0, "LATC");
}

void SA72007_Init(CartInfo *info)
{
  WSync=SA72007Synco;
  GameStateRestore=SARestore;
  info->Power=SAPower;
  AddExState(&latch[0], 1, 0, "LATC");
}

void SA72008_Init(CartInfo *info)
{
  WSync=SA72008Synco;
  GameStateRestore=SARestore;
  info->Power=SAPower;
  AddExState(&latch[0], 1, 0, "LATC");
}

void SA009_Init(CartInfo *info)
{
  WSync=SA009Synco;
  GameStateRestore=SARestore;
  info->Power=SAPower;
  AddExState(&latch[0], 1, 0, "LATC");
}

void SA0036_Init(CartInfo *info)
{
  WSync=SA72007Synco;
  GameStateRestore=SARestore;
  info->Power=SADPower;
  AddExState(&latch[0], 1, 0, "LATC");
}

void SA0037_Init(CartInfo *info)
{
  WSync=SA0161MSynco;
  GameStateRestore=SARestore;
  info->Power=SADPower;
  AddExState(&latch[0], 1, 0, "LATC");
}

// -----------------------------------------------

static void TCU01Synco()
{
  setprg32(0x8000,((latch[0]&0x80)>>6)|((latch[0]>>2)&1));
  setchr8((latch[0]>>3)&0xF);
}

static DECLFW(TCU01Write)
{
  if((A&0x103)==0x102)
  {
    latch[0]=V;
    TCU01Synco();
  }
}

static void TCU01Power(void)
{
  latch[0]=0;
  SetReadHandler(0x8000,0xFFFF,CartBR);
  SetWriteHandler(0x4100,0xFFFF,TCU01Write);
  TCU01Synco();
}

static void TCU01Restore(int version)
{
  TCU01Synco();
}

void TCU01_Init(CartInfo *info)
{
  GameStateRestore=TCU01Restore;
  info->Power=TCU01Power;
  AddExState(&latch[0], 1, 0, "LATC");
}

//-----------------------------------------------

static void TCU02Synco()
{
  setprg32(0x8000,0);
  setchr8(latch[0]&3);
}

static DECLFW(TCU02Write)
{
  if((A&0x103)==0x102)
  {
    latch[0]=V+3;
    TCU02Synco();
  }
}

static DECLFR(TCU02Read)
{
  return (latch[0]&0x3F)|(X.DB&0xC0);
}

static void TCU02Power(void)
{
  latch[0]=0;
  SetReadHandler(0x8000,0xFFFF,CartBR);
  SetReadHandler(0x4100,0x4100,TCU02Read);
  SetWriteHandler(0x4100,0xFFFF,TCU02Write);
  TCU02Synco();
}

static void TCU02Restore(int version)
{
  TCU02Synco();
}

void TCU02_Init(CartInfo *info)
{
  GameStateRestore=TCU02Restore;
  info->Power=TCU02Power;
  AddExState(&latch[0], 1, 0, "LATC");
}

// ---------------------------------------------

static DECLFR(TCA01Read)
{
  uint8 ret;
  if((A&0x4100)==0x4100)
    ret=(X.DB&0xC0)|((~A)&0x3F);
  else
    ret=X.DB;
  return ret;
}

static void TCA01Power(void)
{
  setprg16(0x8000,0);
  setprg16(0xC000,1);
  setchr8(0);
  SetReadHandler(0x8000,0xFFFF,CartBR);
  SetReadHandler(0x4100,0x5FFF,TCA01Read);
}

void TCA01_Init(CartInfo *info)
{
  info->Power=TCA01Power;
}

*/