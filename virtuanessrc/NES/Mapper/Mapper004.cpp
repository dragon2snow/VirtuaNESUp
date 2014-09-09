//////////////////////////////////////////////////////////////////////////
// Mapper004  Nintendo MMC3                                             //
//////////////////////////////////////////////////////////////////////////
#define	MMC3_IRQ_KLAX		1
#define	MMC3_IRQ_SHOUGIMEIKAN	2
#define	MMC3_IRQ_DAI2JISUPER	3
#define	MMC3_IRQ_DBZ2		4
#define	MMC3_IRQ_ROCKMAN3	5

void	Mapper004::Reset()
{
	for( INT i = 0; i < 8; i++ ) {
		reg[i] = 0x00;
	}

	prg0 = 0;
	prg1 = 1;
	SetBank_CPU();

	chr01 = 0;
	chr23 = 2;
	chr4  = 4;
	chr5  = 5;
	chr6  = 6;
	chr7  = 7;
	SetBank_PPU();

	we_sram  = 0;	// Disable
	irq_enable = 0;	// Disable
	irq_counter = 0;
	irq_latch = 0xFF;
	irq_request = 0;
	irq_preset = 0;
	irq_preset_vbl = 0;

	// IRQƒ^ƒCƒvÝ’è
	nes->SetIrqType( NES::IRQ_CLOCK );
	irq_type = 0;

	KT_bank = 0;
}

BYTE	Mapper004::ReadLow( WORD addr )
{
	if( !vs_patch ) {
		if( addr >= 0x5000 && addr <= 0x5FFF ) {
			return	XRAM[addr-0x4000];
		}
	} 

	if(reg[3]&0x80)
	return	Mapper::ReadLow( addr );
	else return 0xff;
}

void	Mapper004::WriteLow( WORD addr, BYTE data )
{
	if(addr==0x5000) KT_bank = data * 0x10;	//for KT CN games

	if( addr >= 0x5000 && addr <= 0x5FFF ) {
		XRAM[addr-0x4000] = data;
	} else {
		if(reg[3]&0x80)
		Mapper::WriteLow( addr, data );
	}
}

void	Mapper004::Write( WORD addr, BYTE data )
{
//DEBUGOUT( "MPRWR A=%04X D=%02X L=%3d CYC=%d\n", addr&0xFFFF, data&0xFF, nes->GetScanline(), nes->cpu->GetTotalCycles() );

	switch( addr & 0xE001 ) {
		case	0x8000:
			reg[0] = data;
			SetBank_CPU();
			SetBank_PPU();
			break;
		case	0x8001:
			reg[1] = data;

			switch( reg[0] & 0x07 ) {
				case	0x00:
					chr01 = data & 0xFE;
					SetBank_PPU();
					break;
				case	0x01:
					chr23 = data & 0xFE;
					SetBank_PPU();
					break;
				case	0x02:
					chr4 = data;
					SetBank_PPU();
					break;
				case	0x03:
					chr5 = data;
					SetBank_PPU();
					break;
				case	0x04:
					chr6 = data;
					SetBank_PPU();
					break;
				case	0x05:
					chr7 = data;
					SetBank_PPU();
					break;
				case	0x06:
					//prg0 = data;
					prg0 = data + KT_bank;
					SetBank_CPU();
					break;
				case	0x07:
					//prg1 = data;
					prg1 = data + KT_bank;
					SetBank_CPU();
					break;
			}
			break;
		case	0xA000:
			reg[2] = data;
			if( !nes->rom->Is4SCREEN() ) {
				if( data & 0x01 ) SetVRAM_Mirror( VRAM_HMIRROR );
				else		  SetVRAM_Mirror( VRAM_VMIRROR );
			}
			break;
		case	0xA001:
			reg[3] = data;
//DEBUGOUT( "MPRWR A=%04X D=%02X L=%3d CYC=%d\n", addr&0xFFFF, data&0xFF, nes->GetScanline(), nes->cpu->GetTotalCycles() );
			break;
		case	0xC000:
//DEBUGOUT( "MPRWR A=%04X D=%02X L=%3d CYC=%d\n", addr&0xFFFF, data&0xFF, nes->GetScanline(), nes->cpu->GetTotalCycles() );
			reg[4] = data;
			if( irq_type == MMC3_IRQ_KLAX || irq_type == MMC3_IRQ_ROCKMAN3 ) {
				irq_counter = data;
			} else {
				irq_latch = data;
			}
			if( irq_type == MMC3_IRQ_DBZ2 ) {
				irq_latch = 0x07;
			}
			break;
		case	0xC001:
			reg[5] = data;
			if( irq_type == MMC3_IRQ_KLAX || irq_type == MMC3_IRQ_ROCKMAN3 ) {
				irq_latch = data;
			} else {
				if( (nes->GetScanline() < 240) || (irq_type == MMC3_IRQ_SHOUGIMEIKAN) ) {
					irq_counter |= 0x80;
					irq_preset = 0xFF;
				} else {
					irq_counter |= 0x80;
					irq_preset_vbl = 0xFF;
					irq_preset = 0;
				}
			}
			break;
		case	0xE000:
			reg[6] = data;
			irq_enable = 0;
			irq_request = 0;

			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE001:
			reg[7] = data;
			irq_enable = 1;
			irq_request = 0;

//			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;
	}
}

void	Mapper004::Clock( INT cycles )
{
//	if( irq_request && (nes->GetIrqType() == NES::IRQ_CLOCK) ) {
//		nes->cpu->IRQ_NotPending();
//	}
}

void	Mapper004::HSync( INT scanline )
{
	if( (scanline >= 0 && scanline <= 239) && nes->ppu->IsDispON() ) 
	{
		if( irq_preset_vbl ) 
		{
				irq_counter = irq_latch;
				irq_preset_vbl = 0;
		}
		if( irq_preset ) 
		{
				irq_counter = irq_latch;
				irq_preset = 0;
		} 
		else if( irq_counter > 0 ) 
		{
				irq_counter--;
		
		}

		if( irq_counter == 0 ) 
		{
			if( irq_enable ) 
			{
				irq_request = 0xFF;
				nes->cpu->SetIRQ( IRQ_MAPPER );
			}
			irq_preset = 0xFF;
			
		}
	}
}

void	Mapper004::SetBank_CPU()
{
	if( reg[0] & 0x40 ) {
		SetPROM_32K_Bank( PROM_8K_SIZE-2, prg1, prg0, PROM_8K_SIZE-1 );
	} else {
		SetPROM_32K_Bank( prg0, prg1, PROM_8K_SIZE-2, PROM_8K_SIZE-1 );
	}
}

void	Mapper004::SetBank_PPU()
{
	if( VROM_1K_SIZE ) {
		if( reg[0] & 0x80 ) {
			SetVROM_8K_Bank( chr4, chr5, chr6, chr7,
					 chr01, chr01+1, chr23, chr23+1 );
		} else {
			SetVROM_8K_Bank( chr01, chr01+1, chr23, chr23+1,
					 chr4, chr5, chr6, chr7 );
		}
	} else {
		if( reg[0] & 0x80 ) {
			SetCRAM_1K_Bank( 4, (chr01+0)&0x07 );
			SetCRAM_1K_Bank( 5, (chr01+1)&0x07 );
			SetCRAM_1K_Bank( 6, (chr23+0)&0x07 );
			SetCRAM_1K_Bank( 7, (chr23+1)&0x07 );
			SetCRAM_1K_Bank( 0, chr4&0x07 );
			SetCRAM_1K_Bank( 1, chr5&0x07 );
			SetCRAM_1K_Bank( 2, chr6&0x07 );
			SetCRAM_1K_Bank( 3, chr7&0x07 );
		} else {
			SetCRAM_1K_Bank( 0, (chr01+0)&0x07 );
			SetCRAM_1K_Bank( 1, (chr01+1)&0x07 );
			SetCRAM_1K_Bank( 2, (chr23+0)&0x07 );
			SetCRAM_1K_Bank( 3, (chr23+1)&0x07 );
			SetCRAM_1K_Bank( 4, chr4&0x07 );
			SetCRAM_1K_Bank( 5, chr5&0x07 );
			SetCRAM_1K_Bank( 6, chr6&0x07 );
			SetCRAM_1K_Bank( 7, chr7&0x07 );
		}
	}
}

void	Mapper004::SaveState( LPBYTE p )
{
	for( INT i = 0; i < 8; i++ ) {
		p[i] = reg[i];
	}
	p[ 8] = prg0;
	p[ 9] = prg1;
	p[10] = chr01;
	p[11] = chr23;
	p[12] = chr4;
	p[13] = chr5;
	p[14] = chr6;
	p[15] = chr7;
	p[16] = irq_enable;
	p[17] = (BYTE)irq_counter;
	p[18] = irq_latch;
	p[19] = irq_request;
	p[20] = irq_preset;
	p[21] = irq_preset_vbl;
}

void	Mapper004::LoadState( LPBYTE p )
{
	for( INT i = 0; i < 8; i++ ) {
		reg[i] = p[i];
	}
	prg0  = p[ 8];
	prg1  = p[ 9];
	chr01 = p[10];
	chr23 = p[11];
	chr4  = p[12];
	chr5  = p[13];
	chr6  = p[14];
	chr7  = p[15];
	irq_enable  = p[16];
	irq_counter = (INT)p[17];
	irq_latch   = p[18];
	irq_request = p[19];
	irq_preset  = p[20];
	irq_preset_vbl = p[21];
}
