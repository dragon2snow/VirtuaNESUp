//////////////////////////////////////////////////////////////////////////
// Mapper198  Nintendo MMC3                                             //
//////////////////////////////////////////////////////////////////////////
void	Mapper198::Reset()
{
	for(int i=0; i<8; i++)
	{
		reg[i]=0;
		chr[i]=i;
	}

	prg[0] = 0;
	prg[1] = 1;
	prg[2] = PROM_8K_SIZE -2;
	prg[3] = PROM_8K_SIZE -1;

	SetBank_CPU();
	SetBank_PPU();

	irq_enable = 0;
	irq_counter = 0;
	irq_latch = 0;
	irq_request = 0;
}

void	Mapper198::WriteLow( WORD addr, BYTE data )
{
	if( addr >= 0x5000 && addr <= 0x5FFF ) {
		XRAM[addr-0x4000] = data;
	} else {
		Mapper::WriteLow( addr, data );
	}
}

BYTE	Mapper198::ReadLow( WORD addr )
{
	if( addr >= 0x5000 && addr <= 0x5FFF ) 
	{
			return	XRAM[addr-0x4000];
	}

	return	Mapper::ReadLow( addr );
}

void	Mapper198::Write( WORD addr, BYTE data )
{
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
					chr[0] = data & 0xFE;
					chr[1] = (data & 0xFE)|1;
					SetBank_PPU();
					break;
				case	0x01:
					chr[2] = data & 0xFE;
					chr[3] = (data & 0xFE)|1;
					SetBank_PPU();
					break;
				case	0x02:
					chr[4] = data;
					SetBank_PPU();
					break;
				case	0x03:
					chr[5] = data;
					SetBank_PPU();
					break;
				case	0x04:
					chr[6] = data;
					SetBank_PPU();
					break;
				case	0x05:
					chr[7] = data;
					SetBank_PPU();
					break;
				case	0x06:
					if(data>=0x50) data&=0x4F;
					prg[0] = data;
					SetBank_CPU();
					break;
				case	0x07:
					if(data>=0x50) data&=0x4F;
					prg[1] = data;
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
			break;
		case	0xC000:
			irq_counter = data;
			irq_request = 0;
			break;
		case	0xC001:
			irq_latch = data;
			irq_request = 0;
			break;
		case	0xE000:
			irq_enable = 0;
			irq_request = 0;
			nes->cpu->ClrIRQ( IRQ_MAPPER );
			break;
		case	0xE001:
			irq_enable = 1;
			irq_request = 0;
			break;
	}
}


void	Mapper198::HSync( INT scanline )
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if( nes->ppu->IsDispON() ) {
			if( irq_enable && !irq_request ) {
				if( scanline == 0 ) {
					if( irq_counter ) {
						irq_counter--;
					}
				}
				if( !(irq_counter--) ) {
					irq_request = 0xFF;
					irq_counter = irq_latch;
					nes->cpu->SetIRQ( IRQ_MAPPER );
				}
			}
		}
	}
}

void	Mapper198::SetBank_CPU()
{
	if( reg[0] & 0x40 ) {
		SetPROM_32K_Bank( prg[2], prg[1], prg[0],prg[3]);
	} else {
		SetPROM_32K_Bank( prg[0], prg[1], prg[2],prg[3]);
	}
}

void	Mapper198::SetBank_PPU()
{
	if( VROM_1K_SIZE ) {
		if( reg[0] & 0x80 ) {
			SetVROM_8K_Bank( chr[4], chr[5], chr[6], chr[7],
					 chr[0], chr[1], chr[2], chr[3] );
		} else {
			SetVROM_8K_Bank( chr[0], chr[1], chr[2], chr[3],
					 chr[4], chr[5], chr[6], chr[7] );
		}
	}
}

void	Mapper198::SaveState( LPBYTE p )
{
	for( INT i = 0; i < 8; i++ ) {
		p[i]   = reg[i];
		p[i+8] = chr[i];
	}
	p[16] = prg[0];
	p[17] = prg[1];
}

void	Mapper198::LoadState( LPBYTE p )
{
	for( INT i = 0; i < 8; i++ ) {
		reg[i] = p[i];
		chr[i] = p[i+8];
	}
	prg[0]  = p[ 16];
	prg[1]  = p[ 17];
}
