//////////////////////////////////////////////////////////////////////////
// MapperSubor999                                                          //
//////////////////////////////////////////////////////////////////////////
void	MapperSubor999::Reset()
{
	reg5200 = reg5000 = 0;
	nes->ppu->SetExtLatchMode( TRUE );
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
	
	// $6000-$7FFF WRAM
	if( addr >= 0x6000 && addr <= 0x7FFF ) {
		CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
	}
}



void MapperSubor999::PPU_ExtLatch( WORD ntbladr, BYTE& chr_l, BYTE& chr_h, BYTE& attr )
{
	INT loopy_v = nes->ppu->GetPPUADDR();
	INT loopy_y = nes->ppu->GetTILEY();
	INT tileofs = (PPUREG[0]&PPU_BGTBL_BIT)<<8;
	INT attradr = 0x23C0+(loopy_v&0x0C00)+((loopy_v&0x0380)>>4);
	INT attrsft = (ntbladr&0x0040)>>4;
	LPBYTE pNTBL = PPU_MEM_BANK[ntbladr>>10];
	INT ntbl_x  = ntbladr&0x001F;
	INT tileadr, ntb;

	ntb = (ntbladr>>10)&3;
	if(ntb==2)
	tileofs = 0x1000;
	else if(ntb==1 && reg5200==2)
	tileofs = 0x1000;
	else
	tileofs = 0x0000;

	attradr &= 0x3FF;
	attr = ((pNTBL[attradr+(ntbl_x>>2)]>>((ntbl_x&2)+attrsft))&3)<<2;
	tileadr = tileofs+pNTBL[ntbladr&0x03FF]*0x10+loopy_y;

	chr_l = PPU_MEM_BANK[tileadr>>10][ tileadr&0x03FF   ];
	chr_h = PPU_MEM_BANK[tileadr>>10][(tileadr&0x03FF)+8];
}


	
void MapperSubor999::SetBank_CPU(void)
{	
	if(reg5200==2)
	{
		SetVRAM_Mirror( VRAM_VMIRROR );
	}else{
		SetVRAM_Mirror( VRAM_HMIRROR );	
	}

	if(reg5200<4){
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
