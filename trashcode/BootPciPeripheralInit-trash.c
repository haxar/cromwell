
/*
DWORD PciReadDword(unsigned int bus, unsigned int dev, unsigned int func, unsigned int reg_off)
{
	DWORD base_addr = 0x80000000;
	base_addr |= ((bus & 0xFF) << 16);	// bus #
	base_addr |= ((dev & 0x1F) << 11);	// device #
	base_addr |= ((func & 0x07) << 8);	// func #

		IoOutputDword(0xcf8, (base_addr + (reg_off & 0xfc)));
		return IoInputDword(0xcfc + (reg_off & 3));
}


DWORD PciWriteDword(unsigned int bus, unsigned int dev, unsigned int func, unsigned int reg_off, unsigned int dw) 
{
		
	DWORD base_addr = 0x80000000;
	base_addr |= ((bus & 0xFF) << 16);	// bus #
	base_addr |= ((dev & 0x1F) << 11);	// device #
	base_addr |= ((func & 0x07) << 8);	// func #

	IoOutputDword(0xcf8, (base_addr + (reg_off & 0xfc)));	
	IoOutputDword(0xcfc + ((reg_off) & 3), dw);

//	IoOutputDword(0xcf8, (base_addr + (reg_off & 0xfc)));	
//	IoOutputDword(0xcfc + ((reg_off) & 3), dw);
	return 0; 
}
  
*/



void BootPciPeripheralInitialization()
{

	__asm__ __volatile__ ( "cli" );

	PciWriteDword(BUS_0, DEV_1, 0, 0x80, 2);  // v1.1 2BL kill ROM area
	if(PciReadByte(BUS_0, DEV_1, 0, 0x8)>=0xd1) { // check revision
		PciWriteDword(BUS_0, DEV_1, 0, 0xc8, 0x8f00);  // v1.1 2BL <-- death
	}

// Bus 0, Device 0, Function 0 = PCI Bridge Device - Host Bridge
//   
   
	PciWriteDword(BUS_0, DEV_0, FUNC_0, 0x48, 0x00000114);
	PciWriteDword(BUS_0, DEV_0, FUNC_0, 0x44, 0x80000000); // new 2003-01-23 ag  trying to get single write actions on TSOP


//	PciWriteDword(BUS_0, DEV_0, FUNC_0, 0xa4, 0x0000e35a);  // AGP register ? Xbeboot-compare
	PciWriteByte(BUS_0, DEV_0, FUNC_0, 0x87, 3); // kern 8001FC21
	
		
//	PciWriteDword(BUS_0, DEV_0, 3, 0x40, 0x0f0fc0c0);  // Xbeboot-compare
//	PciWriteDword(BUS_0, DEV_0, 3, 0x58, 0x00000000);  // Xbeboot-compare
//	PciWriteDword(BUS_0, DEV_0, 3, 0x64, 0x00e08001);  // Xbeboot-compare


	PciWriteByte(BUS_0, DEV_0, 8, 0, 0x42);       // Xbeboot-compare
	
	
	PciWriteDword(BUS_0, DEV_0, 3, 0x40, 0x0017cc00);  // Orginal Andy
	PciWriteDword(BUS_0, DEV_0, 3, 0x58, 0x00008000);  // original Andy

//	PciWriteByte(BUS_0, DEV_0, FUNC_0, 0x4b,0x00); --> BAD !!! -- Xbox Dies sometimes



//	PciWriteDword(BUS_0, DEV_9, FUNC_0, 0x4c, 0xffff00ff); // 2x30nS address setup on IDE
	
	IoOutputByte(0x2e, 0x55);
	IoOutputByte(0x2e, 0x26);

	IoOutputByte(0x61, 0xff);
	IoOutputByte(0x92, 0x01);

//	IoOutputByte(0xcf9, 0x08);	// Reset Port
	IoOutputByte(0xcf9, 0x0);	// Reset Port

	// gah, failure to do this caused long delays and timeouts on boot and shutdown of Linux
	// RTC CMOS was disabled, rtc driver was polling "safe to read" bit which was never okay
	/*
	IoOutputByte(0x70, 0x30);	
	IoOutputByte(0x71, 0x03);  // activate RTC bank1, bank0 and RTC
	IoOutputByte(0x72, 0xf0);	
	IoOutputByte(0x73, 0x00);  // nothing locked


	IoOutputByte(0x70, 0x0a);	
	IoOutputByte(0x71, 0x2d);
	
	IoOutputByte(0x70, 0x0b); 
	IoOutputByte(0x71, (IoInputByte(0x71)&1)|2);

	IoOutputByte(0x70, 0x0c);
	IoInputByte(0x71);
	IoOutputByte(0x70, 0x0d);
	IoInputByte(0x71);
 
          */
          
                
        
        IoOutputByte(0x43, 0x36);         	// Timer 0 (system time): mode 3
        IoOutputByte(0x40, 0xFF);              // 18.2Hz (1.19318MHz/65535)
        IoOutputByte(0x40, 0xFF);
        IoOutputByte(0x43, 0x54);         	// Timer 1 (ISA refresh): mode 2
        IoOutputByte(0x41, 18);                // 64KHz (1.19318MHz/18)

        IoOutputByte(0x00, 0);                 // clear base address 0
        IoOutputByte(0x00, 0);
        IoOutputByte(0x01, 0);                 // clear count 0
        IoOutputByte(0x01, 0);
        IoOutputByte(0x02, 0);                 // clear base address 1
        IoOutputByte(0x02, 0 );
        IoOutputByte(0x03, 0);                 // clear count 1
        IoOutputByte(0x03, 0);
        IoOutputByte(0x04, 0);                 // clear base address 2
        IoOutputByte(0x04, 0);
        IoOutputByte(0x05, 0);                 // clear count 2
        IoOutputByte(0x05, 0);
        IoOutputByte(0x06, 0);                 // clear base address 3
        IoOutputByte(0x06, 0);
        IoOutputByte(0x07, 0);                 // clear count 3
        IoOutputByte(0x07, 0);
        IoOutputByte(0x0B, 0x40);         	// set channel 0 to single mode, verify transfer
        IoOutputByte(0x0B, 0x41);         	// set channel 1 to single mode, verify transfer
        IoOutputByte(0x0B, 0x42);         	// set channel 2 to single mode, verify transfer
        IoOutputByte(0x0B, 0x43);         	// set channel 3 to single mode, verify transfer
        IoOutputByte(0x08, 0);                 // enable controller

        IoOutputByte(0xC0, 0);                 // clear base address 0
        IoOutputByte(0xC0, 0);
        IoOutputByte(0xC2, 0);                 // clear count 0
        IoOutputByte(0xC2, 0);
        IoOutputByte(0xC4, 0);                // clear base address 1
        IoOutputByte(0xC4, 0);
        IoOutputByte(0xC6, 0);                 // clear count 1
        IoOutputByte(0xC6, 0);
        IoOutputByte(0xC8, 0);                 // clear base address 2
        IoOutputByte(0xC8, 0);
        IoOutputByte(0xCA, 0);                 // clear count 2
        IoOutputByte(0xCA, 0);
        IoOutputByte(0xCC, 0);                 // clear base address 3
        IoOutputByte(0xCC, 0);
        IoOutputByte(0xCE, 0);                 // clear count 3
        IoOutputByte(0xCE, 0);
        IoOutputByte(0xD6, 0xC0);         // set channel 0 to cascade mode
        IoOutputByte(0xD6, 0xC1);         // set channel 1 to single mode, verify transfer
        IoOutputByte(0xD6, 0xC2);         // set channel 2 to single mode, verify transfer
        IoOutputByte(0xD6, 0xC3);         // set channel 3 to single mode, verify transfer
        IoOutputByte(0xD0, 0);                 // enable controller
        
        IoOutputByte(0x0E, 0);                 // enable DMA0 channels
        IoOutputByte(0xD4, 0);                 // clear chain 4 mask


	/* Setup the real time clock */
	CMOS_WRITE(RTC_CONTROL_DEFAULT, RTC_CONTROL);
	/* Setup the frequency it operates at */
	CMOS_WRITE(RTC_FREQ_SELECT_DEFAULT, RTC_FREQ_SELECT);
	/* Make certain we have a valid checksum */
	rtc_set_checksum(PC_CKS_RANGE_START,
                        PC_CKS_RANGE_END,PC_CKS_LOC);
	/* Clear any pending interrupts */
	(void) CMOS_READ(RTC_INTR_FLAGS);




        
	// configure ACPI hardware to generate interrupt on PIC-chip pin6 action (via EXTSMI#)

	IoOutputByte(0x80ce, 0x08);  // from 2bl RI#
	IoOutputByte(0x80c0, 0x08);  // from 2bl SMBUSC
	IoOutputByte(0x8004, IoInputByte(0x8004)|1);  // KERN: SCI enable == SCI interrupt generated
	IoOutputWord(0x8022, IoInputByte(0x8022)|2);  // KERN: Interrupt enable register, b1 RESERVED in AMD docs
	IoOutputWord(0x8023, IoInputByte(0x8023)|2);  // KERN: Interrupt enable register, b1 RESERVED in AMD docs
	IoOutputByte(0x8002, IoInputByte(0x8002)|1);  // KERN: Enable SCI interrupt when timer status goes high
	IoOutputWord(0x8028, IoInputByte(0x8028)|1);  // KERN: setting readonly trap event???
 
	I2CTransmitWord(0x10, 0x0b00); // Allow audio
//	I2CTransmitWord(0x10, 0x0b01); // GAH!!!  Audio Mute!

// Bus 0, Device 1, Function 0 = nForce HUB Interface - ISA Bridge
//
	PciWriteDword(BUS_0, DEV_1, FUNC_0, 0x6c, 0x0e065491);
	PciWriteByte(BUS_0, DEV_1, FUNC_0, 0x6a, 0x0003); // kern ??? gets us an int3?  vsync req
	PciWriteDword(BUS_0, DEV_1, FUNC_0, 0x64, 0x00000b0c);
	PciWriteByte(BUS_0, DEV_1, FUNC_0, 0x81, PciReadByte(BUS_0, DEV_1, FUNC_0, 0x81)|8);

	PciWriteDword(BUS_0, DEV_1, FUNC_0, 0x4c, 0x000f0000); // RTC clocks enable?  2Hz INT8?


//
// Bus 0, Device 9, Function 0 = nForce ATA Controller
//

	PciWriteDword(BUS_0, DEV_9, FUNC_0, 0x20, 0x0000ff61);	// (BMIBA) Set Busmaster regs I/O base address 0xff60
	PciWriteDword(BUS_0, DEV_9, FUNC_0, 4, PciReadDword(BUS_0, DEV_9, FUNC_0, 4)|5); // 0x00b00005 );
	PciWriteDword(BUS_0, DEV_9, FUNC_0, 8, PciReadDword(BUS_0, DEV_9, FUNC_0, 8)&0xfffffeff); // 0x01018ab1 ); // was fffffaff

	PciWriteDword(BUS_0, DEV_9, FUNC_0, 0x58, 0x20202020); // kern1.1
	PciWriteDword(BUS_0, DEV_9, FUNC_0, 0x60, 0x00000000); // kern1.1
//  PciWriteDword(BUS_0, DEV_9, FUNC_0, 0x40, 0x000084bb); // new - removed by frankenregister comparison
	PciWriteDword(BUS_0, DEV_9, FUNC_0, 0x50, 0x00000002);  // without this there is no register footprint at IO 1F0

	PciWriteDword(BUS_0, DEV_9, FUNC_0, 0x2c, 0x00000000); // frankenregister from xbe boot
	PciWriteDword(BUS_0, DEV_9, FUNC_0, 0x40, 0x00000000); // frankenregister from xbe boot
		// below reinstated by frankenregister compare with xbe boot
	PciWriteDword(BUS_0, DEV_9, FUNC_0, 0x60, 0xC0C0C0C0); // kern1.1 <--- this was in kern1.1 but is FATAL for good HDD access

//
// Bus 0, Device 4, Function 0 = nForce MCP Networking Adapter - all verified with kern1.1
//
	PciWriteDword(BUS_0, DEV_4, FUNC_0, 4, PciReadDword(BUS_0, DEV_4, FUNC_0, 4) | 7 );
	PciWriteDword(BUS_0, DEV_4, FUNC_0, 0x10, 0xfef00000); // memory base address 0xfef00000
	PciWriteDword(BUS_0, DEV_4, FUNC_0, 0x14, 0x0000e001); // I/O base address 0xe000
	PciWriteDword(BUS_0, DEV_4, FUNC_0, 0x118-0xdc, (PciReadDword(BUS_0, DEV_4, FUNC_0, 0x3c) &0xffff0000) | 0x0004 );

//
// Bus 0, Device 2, Function 0 = nForce OHCI USB Controller - all verified with kern 1.1
//

	PciWriteDword(BUS_0, DEV_2, FUNC_0, 4, PciReadDword(BUS_0, DEV_2, FUNC_0, 4) | 7 );
	PciWriteDword(BUS_0, DEV_2, FUNC_0, 0x10, 0xfed00000);	// memory base address 0xfed00000
	PciWriteDword(BUS_0, DEV_2, FUNC_0, 0x3c, (PciReadDword(BUS_0, DEV_2, FUNC_0, 0x3c) &0xffff0000) | 0x0001 );
	PciWriteDword(BUS_0, DEV_2, FUNC_0, 0x50, 0x0000000f);

//
// Bus 0, Device 3, Function 0 = nForce OHCI USB Controller - verified with kern1.1
//

	PciWriteDword(BUS_0, DEV_3, FUNC_0, 4, PciReadDword(BUS_0, DEV_3, FUNC_0, 4) | 7 );
	PciWriteDword(BUS_0, DEV_3, FUNC_0, 0x10, 0xfed08000);	// memory base address 0xfed08000
	PciWriteDword(BUS_0, DEV_3, FUNC_0, 0x3c, (PciReadDword(BUS_0, DEV_3, FUNC_0, 0x3c) &0xffff0000) | 0x0009 );
	PciWriteDword(BUS_0, DEV_3, FUNC_0, 0x50, 0x00000030);  // actually BYTE?

//
// Bus 0, Device 6, Function 0 = nForce Audio Codec Interface - verified with kern1.1
//

	PciWriteDword(BUS_0, DEV_6, FUNC_0, 4, PciReadDword(BUS_0, DEV_6, FUNC_0, 4) | 7 );
	PciWriteDword(BUS_0, DEV_6, FUNC_0, 0x10, (PciReadDword(BUS_0, DEV_6, FUNC_0, 0x10) &0xffff0000) | 0xd001 );  // MIXER at IO 0xd000
	PciWriteDword(BUS_0, DEV_6, FUNC_0, 0x14, (PciReadDword(BUS_0, DEV_6, FUNC_0, 0x14) &0xffff0000) | 0xd201 );  // BusMaster at IO 0xD200
	PciWriteDword(BUS_0, DEV_6, FUNC_0, 0x18, 0xfec00000);	// memory base address 0xfec00000
//	PciWriteDword(BUS_0, DEV_6, FUNC_0, 0x4c, (PciReadDword(BUS_0, DEV_6, FUNC_0, 0x4c) ) | 0x0101 ); // was wrongly 0x01010000

		// frankenregister from working Linux driver

	PciWriteDword(BUS_0, DEV_6, FUNC_0, 8, 0x40100b1 );
	PciWriteDword(BUS_0, DEV_6, FUNC_0, 0xc, 0x800000 );
	PciWriteDword(BUS_0, DEV_6, FUNC_0, 0x3c, 0x05020106 );
	PciWriteDword(BUS_0, DEV_6, FUNC_0, 0x44, 0x20001 );
	PciWriteDword(BUS_0, DEV_6, FUNC_0, 0x4c, 0x107 );

//
// Bus 0, Device 5, Function 0 = nForce MCP APU
//
	PciWriteDword(BUS_0, DEV_5, FUNC_0, 4, PciReadDword(BUS_0, DEV_5, FUNC_0, 4) | 7 );
	PciWriteDword(BUS_0, DEV_5, FUNC_0, 0x3c, (PciReadDword(BUS_0, DEV_5, FUNC_0, 0x3c) &0xffff0000) | 0x0005 );
	PciWriteDword(BUS_0, DEV_5, FUNC_0, 0x10, 0xfe800000);	// memory base address 0xfe800000

//
// Bus 0, Device 1, Function 0 = nForce HUB Interface - ISA Bridge
//
//	PciWriteDword(BUS_0, DEV_1, FUNC_0, 0x8c, 0x40000000 ); // from after pic challenge originally
	PciWriteDword(BUS_0, DEV_1, FUNC_0, 0x8c, (PciReadDword(BUS_0, DEV_1, FUNC_0, 0x8c) &0xfbffffff) | 0x08000000 );

	bprintf("a\n");


//
// ACPI pin init
//
/*
	IoOutputByte(0x80c5, 0x05); // Set CPUSLEEP# pin to low
	IoOutputByte(0x80c6, 0x05); // Set CACHEZZ pin to low
	IoOutputByte(0x80c7, 0x05); // Set CACHEZZ pin to low
	IoOutputByte(0x80c8, 0x04); // Set CACHEZZ pin to low
	IoOutputDword(0x80b4, 0xffff);  // any interrupt resets ACPI system inactivity timer
*/

	IoOutputDword(0x80b4, 0xffff);  // any interrupt resets ACPI system inactivity timer
	IoOutputByte(0x80cc, 0x08); // Set EXTSMI# pin to be pin function
	IoOutputByte(0x80cd, 0x08); // Set PRDY pin on ACPI to be PRDY function
	IoOutputByte(0x80cf, 0x08); // Set C32KHZ pin to be pin function

	IoOutputWord(0x8020, IoInputWord(0x8020)|0x200); // ack any preceding ACPI int


	bprintf("b\n");

//
// Bus 0, Device 1e, Function 0 = nForce AGP Host to PCI Bridge
//
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 4, PciReadDword(BUS_0, DEV_1e, FUNC_0, 4) | 7 );
//	bprintf("Test Smb status3a =%x\n",IoInputWord(I2C_IO_BASE+0));
////	PciWriteWord(BUS_0, DEV_1e, FUNC_0, 0x118-0xf2, 0xf7f0); // was missing  0xf3f0 or 0xf7f0 depending on var
//	bprintf("Test Smb status3b =%x\n",IoInputWord(I2C_IO_BASE+0));
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x18, (PciReadDword(BUS_0, DEV_1e, FUNC_0, 0x18) &0xffffff00));
//	bprintf("Test Smb status3c =%x\n",IoInputWord(I2C_IO_BASE+0));
////	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x14, (PciReadDword(BUS_0, DEV_1e, FUNC_0, 0x14) &0x0000ffff) | 0x01010000); // was missing
//	bprintf("Test Smb status3d =%x\n",IoInputWord(I2C_IO_BASE+0));
//	bprintf("Test Smb status3e =%x\n",IoInputWord(I2C_IO_BASE+0));
//	PciWriteWord(BUS_0, DEV_1e, FUNC_0, 0x20, 0xfd00); // <-- copy of line above to maintain positioning!
//	PciWriteWord(BUS_0, DEV_1e, FUNC_0, 0x1e, 0xfe70); // kills SMB controller sometimes!!!
//	bprintf("Test Smb status3f =%x\n",IoInputWord(I2C_IO_BASE+0));
//	bprintf("Test Smb status3g =%x\n",IoInputWord(I2C_IO_BASE+0));
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x3c, 7);  // trying to get video irq

	// frankenregister xbe load correction to match cromwell load
	// controls requests for memory regions
   
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x0c, 0xff019ee7);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x10, 0xbcfaf7e7);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x14, 0x0101fafa);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x1c, 0x02a000f0);    // Coud this be the BASE Video Address ?
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x20, 0xfdf0fd00);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x24, 0xf7f0f000);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x28, 0x8e7ffcff);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x2c, 0xf8bfef87);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x30, 0xdf758fa3);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x38, 0xb785fccc);

      
 /*  
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x04, 0x02200007);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x08, 0x060400a1);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x0c, 0xff019ee7);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x10, 0xbcfaf7e7);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x14, 0x0101fafa);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x18, 0x00010100);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x1c, 0x02a000f0);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x20, 0xfdf0fd00);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x24, 0xf7f0f000);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x28, 0x8e7ffcff);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x2c, 0xf8bfef87);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x30, 0xdf758fa3);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x34, 0x00000000);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x38, 0xb785fccc);
	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x3c, 0x00000007);	// Video IRQ
  	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x40, 0x00000001);
    	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x44, 0x80000000);
      	PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x48, 0x80000014);
        PciWriteDword(BUS_0, DEV_1e, FUNC_0, 0x4c, 0x80000001);
*/   
//
// Bus 1, Device 0, Function 0 = NV2A GeForce3 Integrated GPU
//
	PciWriteDword(BUS_1, DEV_0, FUNC_0, 4, PciReadDword(BUS_1, DEV_0, FUNC_0, 4) | 7 );
	PciWriteDword(BUS_1, DEV_0, FUNC_0, 0x3c, (PciReadDword(BUS_1, DEV_0, FUNC_0, 0x3c) &0xffff0000) | 0x0103 );  // should get vid irq!!
	PciWriteDword(BUS_1, DEV_0, FUNC_0, 0x4c, 0x00000114);

// frankenregisters so Xromwell matches Cromwell

	PciWriteDword(BUS_1, DEV_0, FUNC_0, 0x0c, 0x0);
	PciWriteDword(BUS_1, DEV_0, FUNC_0, 0x18, 0x08);




}
