//////////////////////////////////////////////////////////////////////////
// Mapper163  NanJing Games                                             //
//////////////////////////////////////////////////////////////////////////
void Mapper163::Reset()
{
	reg[1] = 0xFF;
	strobe = 1;
	security = trigger = reg[0] = 0x00;
	rom_type = 0;
	SetPROM_32K_Bank(15);

	DWORD	crc = nes->rom->GetPROM_CRC();
	if( crc == 0xb6a10d5d ) {	// Hu Lu Jin Gang (NJ039) (Ch) [dump]
		SetPROM_32K_Bank(0);
	}
	if( crc == 0xf52468e7 ) {	// San Guo Wu Shuang - Meng Jiang Zhuan (NJ047) (Ch) [dump]
		rom_type = 1;
	}

	//nes->ppu->SetVromWrite(1);
	//SetCRAM_8K_Bank(0);
}

BYTE Mapper163::ReadLow( WORD addr )
{
	if((addr>=0x5000 && addr<0x6000))
	{
		switch (addr & 0x7700)
		{
			case 0x5100:
					return security;
					break;
			case 0x5500:
					if(trigger)
						return security;
					else
						return 0;
					break;
		}
		return 4;
	}
	else if( addr>=0x6000 ) {
		return	CPU_MEM_BANK[addr>>13][addr&0x1FFF];
	}
	return Mapper::ReadLow( addr );
}

void Mapper163::WriteLow(WORD addr, BYTE data)
{
	if((addr>=0x4020 && addr<0x6000))
	{
		if(addr==0x5101){
			if(strobe && !data){
				trigger ^= 1;
//				trigger ^= 0xFF;
			}
			strobe = data;
		}else if(addr==0x5100 && data==6){
			SetPROM_32K_Bank(3);
		}
		else{
			switch (addr & 0x7300)
			{
				case 0x5000:
						reg[1]=data;
						SetPROM_32K_Bank( (reg[1] & 0xF) | (reg[0] << 4) );
						if(!(reg[1]&0x80)&&(nes->ppu->GetScanlineNo()<128))
							SetCRAM_8K_Bank(0);
						if(rom_type==1) SetCRAM_8K_Bank(0);
						break;
				case 0x5200:
						reg[0]=data;
						SetPROM_32K_Bank( (reg[1] & 0xF) | (reg[0] << 4) );
						break;
				case 0x5300:
						security=data;
						break;
			}
		}
	}
	else if( addr>=0x6000 ) {
		CPU_MEM_BANK[addr>>13][addr&0x1FFF] = data;
	}
}

void	Mapper163::HSync(int scanline)
{
	if( (reg[1]&0x80) && nes->ppu->IsDispON() ) {
		if(scanline==127){
			SetCRAM_4K_Bank(0, 1);
			SetCRAM_4K_Bank(4, 1);
		}
		if (rom_type==1){
			if(scanline<127){
				SetCRAM_4K_Bank(0, 0);
				SetCRAM_4K_Bank(4, 0);
			}
		}else{
			if(scanline==239){
				SetCRAM_4K_Bank(0, 0);
				SetCRAM_4K_Bank(4, 0);
			}
		}
	}
}

void	Mapper163::SaveState( LPBYTE p )
{
	p[0] = reg[0];
	p[1] = reg[1];
}

void	Mapper163::LoadState( LPBYTE p )
{

	reg[0] = p[0];
	reg[1] = p[1];
}
