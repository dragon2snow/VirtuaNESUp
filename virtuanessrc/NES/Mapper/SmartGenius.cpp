//////////////////////////////////////////////////////////////////////////
// SmartGenius                                                            //
//////////////////////////////////////////////////////////////////////////
#pragma warning (disable:4554)

void	MapperSmartGenius::Reset()
{
//	nes->ppu->SetVromWrite(1);

	for( INT i = 0; i < 11; i++ ) {
		reg[i] = 0x00;
	}
	prg0 = 60;
	prg1 = 61;
	prg2 = 62;
	prg3 = 63;
//	SetBank_CPU();
	SetPROM_32K_Bank( prg0, prg1, prg2, prg3 );

	chr01 = 0;
	chr23 = 2;
	chr4  = 4;
	chr5  = 5;
	chr6  = 6;
	chr7  = 7;
	chr1  = chr01+1;
	chr3  = chr23+1;
	SetBank_PPU();

	we_sram  = 0;	// Disable
	irq_enable = 0;	// Disable
	irq_counter = 0;
	irq_latch = 0;
	irq_request = 0;

}

BYTE	MapperSmartGenius::ReadLow( WORD addr )
{
	return	Mapper::ReadLow( addr );
}

void	MapperSmartGenius::WriteLow( WORD addr, BYTE data )
{
	switch( addr ) {
		case	0x5010:
			reg[8] = data;
//			SetBank_CPU_L();
			break;
		case	0x5011:
			reg[9] = data;
			SetBank_CPU_L();
			break;
		case	0x5012:
			reg[10] = data;
			SetBank_CPU_L();
			break;
		default:
			//Mapper::WriteLow( addr, data );
			break;
	}
	if(addr>=0x6000){
		CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
	}
}


void	MapperSmartGenius::Write( WORD addr, BYTE data )
{
	switch( addr & 0xE001 ) {
		case	0x8000:
			reg[0] = data;
			SetBank_CPU();
			SetBank_PPU();
			break;
		case	0x8001:
			reg[1] = data;

			switch( reg[0] & 0x0f ) {
				case	0x00:
					chr01 = data;
					chr1 = chr01+1;
					SetBank_PPU();
					break;
				case	0x01:
					chr23 = data;
					chr3 = chr23+1;
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
					prg0 = data + (reg[10]<<1);
					SetBank_CPU();
					break;
				case	0x07:
					prg1 = data + (reg[10]<<1);
					SetBank_CPU();
					break;
				case	0x08:
					prg2 = data;
					SetBank_CPU();
					break;
				case	0x09:
					prg3 = data;
					SetBank_CPU();
					break;
				case	0x0a:
					chr1 = data;
					SetBank_PPU();
					break;
				case	0x0b:
					chr3 = data;
					SetBank_PPU();
					break;
			}
			break;
		case	0xA000:
			reg[2] = data;
			data &= 0x03;
			if( data == 0 )	     SetVRAM_Mirror( VRAM_VMIRROR );
			else if( data == 1 ) SetVRAM_Mirror( VRAM_HMIRROR );
			else if( data == 2 ) SetVRAM_Mirror( VRAM_MIRROR4L );
			else		     SetVRAM_Mirror( VRAM_MIRROR4H );
			break;
		case	0xA001:
			reg[3] = data;
			break;
		case	0xC000:
			reg[4] = data;
			irq_counter = data;
			irq_request = 0;
			break;
		case	0xC001:
			reg[5] = data;
			irq_latch = data;
			irq_request = 0;
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
			break;
	}	
	
}

void	MapperSmartGenius::HSync( INT scanline )
{
	if( (scanline >= 0 && scanline <= 239) ) {
		if( nes->ppu->IsDispON() ) {
			if( irq_enable && !irq_request ) {
				if( scanline == 0 ) {
					if( irq_counter ) {
						irq_counter -= 1;
					}
				}
				if(!(irq_counter)){
					irq_request = 0xFF;
					irq_counter = irq_latch;
					nes->cpu->SetIRQ( IRQ_MAPPER );
				}
				irq_counter--;
			}
		}
	}
}

void	MapperSmartGenius::SetBank_CPU()
{
	if( reg[0] & 0x40 ) {
		SetPROM_32K_Bank( prg2, prg1, prg0, prg3 );
	} else {
		//SetPROM_32K_Bank( prg0, prg1, prg2, prg3 );
		SetPROM_8K_Bank( 4, prg0 );
		SetPROM_8K_Bank( 5, prg1 );
	}
}

void	MapperSmartGenius::SetBank_CPU_L()
{
		switch( reg[8] & 0x07 ) {
			case	0x00:	//512K
				SetPROM_16K_Bank( 4, (((reg[9]&0x70)>>5)<<5) + 31 + reg[10] );
				SetPROM_16K_Bank( 6, (((reg[9]&0x70)>>5)<<5) + 31 + reg[10] );
				prg2 = ((((reg[9]&0x70)>>5)<<5) + 31 + reg[10])<<1;
				prg3 = ((((reg[9]&0x70)>>5)<<5) + 31 + reg[10])<<1 + 1;
				break;
			case	0x01:	//256K
				SetPROM_16K_Bank( 4, (((reg[9]&0x70)>>4)<<4) + 15 + reg[10] );
				SetPROM_16K_Bank( 6, (((reg[9]&0x70)>>4)<<4) + 15 + reg[10] );
				prg2 = ((((reg[9]&0x70)>>4)<<4) + 15 + reg[10])<<1;
				prg3 = ((((reg[9]&0x70)>>4)<<4) + 15 + reg[10])<<1 + 1;
				break;
			case	0x02:	//128K
				SetPROM_16K_Bank( 4, (reg[9] & 0x78) + 7 + reg[10] );
				SetPROM_16K_Bank( 6, (reg[9] & 0x78) + 7 + reg[10] );
				prg2 = ((reg[9] & 0x78) + 7 + reg[10])<<1;
				prg3 = ((reg[9] & 0x78) + 7 + reg[10])<<1 + 1;
				break;
			case	0x03:
				SetPROM_16K_Bank( 4, reg[9] + reg[10] );
				SetPROM_16K_Bank( 6, reg[9] + reg[10] );
				prg2 = (reg[9] + reg[10])<<1;
				prg3 = (reg[9] + reg[10])<<1 + 1;
				break;
			case	0x04:
				SetPROM_32K_Bank( (reg[9]>>1) + (reg[10]>>1) );
				prg2 = ((reg[9]>>1) + (reg[10]>>1))<<2;
				prg3 = ((reg[9]>>1) + (reg[10]>>1))<<2 + 1;
				break;
			case	0x05:
				SetPROM_16K_Bank( 4, (reg[9] & 0x78) + 7 + reg[10] );
				SetPROM_16K_Bank( 6, (reg[9] & 0x78) + 7 + reg[10] );
				prg2 = ((reg[9] & 0x78) + 7 + reg[10])<<1;
				prg3 = ((reg[9] & 0x78) + 7 + reg[10])<<1 + 1;
				break;
			case	0x06:
				SetPROM_8K_Bank( 4, (reg[9] & 0x78) + (reg[10]<<1) );
				SetPROM_8K_Bank( 5, (reg[9] & 0x78) + (reg[10]<<1) );
				SetPROM_8K_Bank( 6, (reg[9] & 0x78) + (reg[10]<<1) );
				SetPROM_8K_Bank( 7, (reg[9] & 0x78) + (reg[10]<<1) );
				prg2 = (reg[9] & 0x78) + (reg[10]<<1);
				prg3 = (reg[9] & 0x78) + (reg[10]<<1) + 1;
				break;
			case	0x07:
				SetPROM_8K_Bank( 4, (reg[9] & 0x78) + (reg[10]<<1) );
				SetPROM_8K_Bank( 5, (reg[9] & 0x78) + (reg[10]<<1) );
				SetPROM_8K_Bank( 6, (reg[9] & 0x78) + (reg[10]<<1) );
				SetPROM_8K_Bank( 7, (reg[9] & 0x78) + (reg[10]<<1) );
				prg2 = (reg[9] & 0x78) + (reg[10]<<1);
				prg3 = (reg[9] & 0x78) + (reg[10]<<1) + 1;
				break;
		}
}

void	MapperSmartGenius::SetBank_PPU()
{
	if( VROM_1K_SIZE ) {
		if( reg[0] & 0x80 ) {
			SetVROM_1K_Bank( 4, chr01);
			SetVROM_1K_Bank( 5, chr1 );
			SetVROM_1K_Bank( 6, chr23);
			SetVROM_1K_Bank( 7, chr3 );
			SetVROM_1K_Bank( 0, chr4 );
			SetVROM_1K_Bank( 1, chr5 );
			SetVROM_1K_Bank( 2, chr6 );
			SetVROM_1K_Bank( 3, chr7 );

		} else {
			SetVROM_1K_Bank( 0, chr01);
			SetVROM_1K_Bank( 1, chr1 );
			SetVROM_1K_Bank( 2, chr23);
			SetVROM_1K_Bank( 3, chr3 );
			SetVROM_1K_Bank( 4, chr4 );
			SetVROM_1K_Bank( 5, chr5 );
			SetVROM_1K_Bank( 6, chr6 );
			SetVROM_1K_Bank( 7, chr7 );

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

void	MapperSmartGenius::SaveState( LPBYTE p )
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
	p[17] = irq_counter;
	p[18] = irq_latch;
	p[19] = irq_request;
	p[20] = prg2;
	p[21] = prg3;
	p[22] = chr1;
	p[23] = chr3;
}

void	MapperSmartGenius::LoadState( LPBYTE p )
{
	for( INT i = 0; i < 8; i++ ) {
		reg[i] = p[i];
	}
	prg0  = p[ 8];
	prg1  = p[ 9];
	prg2  = p[20];
	prg3  = p[21];
	chr01 = p[10];
	chr1  = p[22];
	chr23 = p[11];
	chr3  = p[23];
	chr4  = p[12];
	chr5  = p[13];
	chr6  = p[14];
	chr7  = p[15];
	irq_enable  = p[16];
	irq_counter = p[17];
	irq_latch   = p[18];
	irq_request = p[19];
}






void	WaiXing_FS005::Reset()
{
	SetPROM_32K_Bank( 0, 1, 0x3e, 0x3f );
}

void	WaiXing_FS005::WriteLow( WORD addr, BYTE data )
{
	switch( addr ) {
		case	0x5010:			break;
		case	0x5011:			
			SetPROM_32K_Bank(data>>1);
			break;
		case	0x5012:			break;
		default:
			//Mapper::WriteLow( addr, data );
			break;
	}
	if(addr>=0x6000){
		CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
	}
}


void	WaiXing_FS005::Write( WORD addr, BYTE data )
{
	switch( addr & 0xE001 ) 
	{
		case	0xA000:
			if( !nes->rom->Is4SCREEN() ) {
				if( data & 0x01 ) SetVRAM_Mirror( VRAM_HMIRROR );
				else		  SetVRAM_Mirror( VRAM_VMIRROR );
			}
			break;		
	}		
}


void	WaiXing_FW01::Reset()
{
	SetPROM_32K_Bank( 0, 1,0 ,1);
	SetVRAM_Mirror( VRAM_HMIRROR );
}

void	WaiXing_FW01::Write( WORD A, BYTE V )
{
	if( A>=0x8083 )
	{
		int bank = (A-0x8083)/8;
		if(bank>0x20) bank-=0x10;
			SetPrg32(0x8000,bank );
	}	
}