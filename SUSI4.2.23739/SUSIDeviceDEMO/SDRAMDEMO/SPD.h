#ifndef _SPD_H_
#define _SPD_H_

//#ifndef SUSI_API
//#define SUSI_API __stdcall
//#endif
#include "SusiDeviceAPI.h"
#include <string>
//=============================================================================
// ID Format:
//
//	31		24	20			12	8		0
//	+-------+---+-----------+---+-------+
//	|  N/A	| C	|   Type	| S | Index	|
//	+-------+---+-----------+---+-------+
//
//	Where:
//		C		RAM = 0b1010
//		Type	ID Type code
//		S		ID Sub-Type code
//		Index	Device Unit
//=============================================================================

#define SPDDEMO_SOCKET_MAX				8

#define SPD_ID_BASE						0x00A00000

#define SPD_ID_INFO_TYPE				0x00000
#define SPD_ID_INFO_SUBTYPE				0xF00
#define SPD_ID_DRAM_BASE				(SPD_ID_BASE | SPD_ID_INFO_TYPE)
#define SPD_ID_DRAM_SOCKET(n)			(SPD_ID_INFO_SUBTYPE & (n<<8)) /* n=0-7 */

#define SUSI_ID_GET_SUBTYPE(Id)			(Id & 0x00000F00)
 
#define SPD_ID_DRAM_QTY					((SusiId_t)(SPD_ID_DRAM_BASE + 0x00))

/* Description			       | Return Values
// ----------------------------+----------------
//	"Reserved"					0x00
//	"Standard FPM DRAM"			0x01
//	"EDO"						0x02
//	"Pipelined Nibble"			0x03
//	"SDRAM"						0x04
//	"ROM"						0x05
//	"DDR SGRAM"					0x06
//	"DDR SDRAM"					0x07
//	"DDR2 SDRAM"				0x08
//	"DDR2 SDRAM FB-DIMM"		0x09
//	"DDR2 SDRAM FB-DIMM PROBE"	0x0A
//	"DDR3 SDRAM"				0x0B
//	"DDR4 SDRAM"				0x0C
*/
#define SPD_ID_DRAM_TYPE(n)				((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x01))

/*
// PARTNUMBER Format:
//	159			  128			96		   	  64			32			  0
//	+-------------+-------------+-------------+-------------+-------------+
//	| PARTNUMBER1 | PARTNUMBER2 | PARTNUMBER3 | PARTNUMBER4 | PARTNUMBER5 |
//	+-------------+-------------+-------------+-------------+-------------+
//	example:
//		PARTNUMBER1		SQR-
//		PARTNUMBER2		SD4I
//		PARTNUMBER3		16G2
//		PARTNUMBER4		K4SN
//		PARTNUMBER5		BB  
//		PARTNUMBER   =  SQR-SD4I16G2K4SNBB  
*/
#define SPD_ID_DRAM_PARTNUMBER1(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x11))
#define SPD_ID_DRAM_PARTNUMBER2(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x12))
#define SPD_ID_DRAM_PARTNUMBER3(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x13))
#define SPD_ID_DRAM_PARTNUMBER4(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x14))
#define SPD_ID_DRAM_PARTNUMBER5(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x15))

/*
// SPECIFICDATA Format:
//	223			  192			160     	 128			96			  64		   32			  0
//	+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
//	|SPECIFICDATA1|SPECIFICDATA2|SPECIFICDATA3|SPECIFICDATA4|SPECIFICDATA5|SPECIFICDATA6|SPECIFICDATA7|
//	+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
//	example:
//		SPECIFICDATA1		NCA1
//		SPECIFICDATA2		-191
//		SPECIFICDATA3		2200
//		SPECIFICDATA4		40
//		SPECIFICDATA5
//		SPECIFICDATA6
//		SPECIFICDATA7
//		SPECIFICDATA8
//		SPECIFICDATA    =  NCA1-191220040
*/
#define SPD_ID_DRAM_SPECIFICDATA1(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x21))
#define SPD_ID_DRAM_SPECIFICDATA2(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x22))
#define SPD_ID_DRAM_SPECIFICDATA3(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x23))
#define SPD_ID_DRAM_SPECIFICDATA4(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x24))
#define SPD_ID_DRAM_SPECIFICDATA5(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x25))
#define SPD_ID_DRAM_SPECIFICDATA6(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x26))
#define SPD_ID_DRAM_SPECIFICDATA7(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x27))
#define SPD_ID_DRAM_SPECIFICDATA8(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x28))

/* Description			  | Return Values     | DRAM TYPE
// -----------------------+-------------------+-----------
//	"Undefined"				0x00				DDR3
//	"RDIMM"					0x01				DDR3
//	"UDIMM"					0x02				DDR3
//	"SO-DIMM"				0x03				DDR3
//	"Micro-DIMM"			0x04				DDR3
//	"Mini-RDIMM"			0x05				DDR3
//	"Mini-UDIMM"			0x06				DDR3
//	"Mini-CDIMM"			0x07				DDR3
//	"72b-SO-UDIMM"			0x08				DDR3
//	"72b-SO-RDIMM"			0x09				DDR3
//	"72b-SO-CDIMM"			0x0A				DDR3
//	"LRDIMM"				0x0B				DDR3
//
//	"Undefined"				0x00				DDR4
//	"RDIMM"					0x01				DDR4
//	"UDIMM"					0x02				DDR4
//	"SO-DIMM"				0x03				DDR4
//	"LRDIMM"				0x04				DDR4
//	"Mini-RDIMM"			0x05				DDR4
//	"Mini-UDIMM"			0x06				DDR4
//	"Reserved"				0x07				DDR4
//	"72b-SO-RDIMM"			0x08				DDR4
//	"72b-SO-UDIMM"			0x09				DDR4
//	"Reserved"				0x0A				DDR4
//	"Reserved"				0x0B				DDR4
//	"16b-SO-DIMM"			0x0C				DDR4
//	"32b-SO-DIMM"			0x0D				DDR4
//	"Reserved"				0x0E				DDR4
//	"Reserved"				0x0F				DDR4
*/
#define SPD_ID_DRAM_MODULETYPE(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x02))
#define SPD_ID_DRAM_SIZE(n)				((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x03)) //GB
#define SPD_ID_DRAM_SPEED(n)			((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x04)) //MHz
#define SPD_ID_DRAM_RANK(n)				((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x05))
#define SPD_ID_DRAM_VOLTAGE(n)			((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x06)) //mV
#define SPD_ID_DRAM_BANK(n)				((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x07))

/*
// WEEKYEAR Format:
//	31		16		8		0
//	+-------+-------+-------+
//	|  N/A	|  WEEK |  YEAR |
//	+-------+-------+-------+
*/
#define SPD_ID_DRAM_WEEKYEAR(n)			((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x08))

// (Return Values/100+273.15) ¢XC
#define SPD_ID_DRAM_TEMPERATURE(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x09)) 

/* Description | Return Values
// ------------+----------------
//	Enable		57 50
//	Disable		00 00
//	N/A			others
*/
#define SPD_ID_DRAM_WRITEPROTECTION(n)	((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x0A))

/* Description		   | Return Values
// --------------------+----------------
//	Advantech			8A C8
//	N/A					others
*/
#define SPD_ID_DRAM_MANUFACTURE(n)		((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x0B))

/* Description		   | Return Values
// --------------------+----------------
//	Samsung				80 CE
//	HYNIX				80 AD
//	Micron				80 2C
//	N/A					others
*/
#define SPD_ID_DRAM_DRAMIC(n)			((SusiId_t)(SPD_ID_DRAM_BASE + SPD_ID_DRAM_SOCKET(n) + 0x0C))

class SPD{
private:
	

public:
	static std::string MemoryType(uint8_t dev);
	static std::string DDR3ModuleType(uint8_t dev);
	static std::string DDR4ModuleType(uint8_t dev);
	static std::string DDR5ModuleType(uint8_t dev);
	static std::string GetManufacture(uint32_t dev);
	static bool isAvailable();
	static void GetDRAMQTY(uint32_t *qty);
	static void DRAM_Info(uint8_t socket);
	static void wait_enter_key();
	
};


#endif /* _SPD_H_ */
