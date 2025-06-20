#ifndef _SAB2000_H_
#define _SAB2000_H_

//=============================================================================
// ID Format:
//
//	31		24	20			12	8		0
//	+-------+---+-----------+---+-------+
//	|  N/A	| C	|   Type	| S | Index	|
//	+-------+---+-----------+---+-------+
//
//	Where:
//		C		SAB2000 = 0b1000
//		Type	ID Type code
//		S		ID Sub-Type code
//		Index	Device Unit
//=============================================================================
#define SAB2000_ID_BASE						0x00800000

#define SAB2000_ID_INFO_TYPE				0x00000
#define SAB2000_ID_INFO_BASE				(SAB2000_ID_BASE | SAB2000_ID_INFO_TYPE)
#define SAB2000_ID_DEVICE_AVAILABLE			((SusiId_t)(SAB2000_ID_INFO_BASE + 0))
#define SAB2000_ID_FW_VER					((SusiId_t)(SAB2000_ID_INFO_BASE + 1))			// [7:0]	(byte) Version 0
// [15:8]	(byte) Version 1
// [23:16]	(char)
// [31:24]	Reserved

#define SAB2000_ID_EC_TYPE					((SusiId_t)(SAB2000_ID_INFO_BASE + 2))			// [7:0]	(byte) TBD
// [15:8]	(byte) 0x12: 8512, 0x16: 8516, 0x18: 8518
// [23:16]	(char) I: ITE, N: ENE
// [31:24]	Reserved

#define SAB2000_ID_KERNEL_VER				((SusiId_t)(SAB2000_ID_INFO_BASE + 3))			// [7:0]	(byte) Minor
// [15:8]	(byte) Major
// [23:16]	(char) X or V
// [31:24]	Reserved

#define SAB2000_ID_CASEOPEN					((SusiId_t)(SAB2000_ID_INFO_BASE + 0x10))

#define SAB2000_ID_CTRL_TYPE				0x10000
#define SAB2000_ID_CTRL_BASE				(SAB2000_ID_BASE | SAB2000_ID_CTRL_TYPE)
#define SAB2000_ID_CTRL_ALERT				((SusiId_t)(SAB2000_ID_CTRL_BASE + 0))			// [0]	 0: OFF, 1: ON	(RW)


#define SAB2000_ID_HWM_TEMP_TYPE			0x20000
#define SAB2000_ID_HWM_TEMP_BASE			(SAB2000_ID_BASE | SAB2000_ID_HWM_TEMP_TYPE)
#define SAB2000_ID_HWM_TEMP_VTIN1			((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x000))	// Unit: 0.1 Kelvins
#define SAB2000_ID_HWM_TEMP_VTIN2			((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x001))
#define SAB2000_ID_HWM_TEMP_VTIN3			((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x002))
#define SAB2000_ID_HWM_TEMP_BT1				((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x003))
#define SAB2000_ID_HWM_TEMP_BT2				((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x004))
#define SAB2000_ID_HWM_TEMP_BT3				((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x005))
#define SAB2000_ID_HWM_TEMP_BT4				((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x006))
#define SAB2000_ID_HWM_TEMP_ALERT_VTIN1		((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x100))	// RW
#define SAB2000_ID_HWM_TEMP_ALERT_VTIN2		((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x101))	// RW
#define SAB2000_ID_HWM_TEMP_ALERT_BT1		((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x103))	// RW
#define SAB2000_ID_HWM_TEMP_ALERT_BT2		((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x104))	// RW
#define SAB2000_ID_HWM_TEMP_ALERT_BT3		((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x105))	// RW
#define SAB2000_ID_HWM_TEMP_ALERT_BT4		((SusiId_t)(SAB2000_ID_HWM_TEMP_BASE + 0x106))	// RW

#define SAB2000_ID_HWM_VOLT_TYPE			0x21000
#define SAB2000_ID_HWM_VOLT_BASE			(SAB2000_ID_BASE | SAB2000_ID_HWM_VOLT_TYPE)
#define SAB2000_ID_HWM_VOLT_VCOREA			((SusiId_t)(SAB2000_ID_HWM_VOLT_BASE + 0))		// Unit: millivolts
#define SAB2000_ID_HWM_VOLT_VCOREB			((SusiId_t)(SAB2000_ID_HWM_VOLT_BASE + 1))
#define SAB2000_ID_HWM_VOLT_3V3				((SusiId_t)(SAB2000_ID_HWM_VOLT_BASE + 2))
#define SAB2000_ID_HWM_VOLT_5V				((SusiId_t)(SAB2000_ID_HWM_VOLT_BASE + 3))
#define SAB2000_ID_HWM_VOLT_12V				((SusiId_t)(SAB2000_ID_HWM_VOLT_BASE + 4))
#define SAB2000_ID_HWM_VOLT_12NV			((SusiId_t)(SAB2000_ID_HWM_VOLT_BASE + 5))
#define SAB2000_ID_HWM_VOLT_5VSB			((SusiId_t)(SAB2000_ID_HWM_VOLT_BASE + 6))
#define SAB2000_ID_HWM_VOLT_5NV				((SusiId_t)(SAB2000_ID_HWM_VOLT_BASE + 7))
#define SAB2000_ID_HWM_VOLT_VBAT			((SusiId_t)(SAB2000_ID_HWM_VOLT_BASE + 8))

#define SAB2000_ID_HWM_FAN_TYPE				0x22000
#define SAB2000_ID_HWM_FAN_BASE				(SAB2000_ID_BASE | SAB2000_ID_HWM_FAN_TYPE)
#define SAB2000_ID_HWM_FAN_0				((SusiId_t)(SAB2000_ID_HWM_FAN_BASE + 0))		// Unit: RPM
#define SAB2000_ID_HWM_FAN_1				((SusiId_t)(SAB2000_ID_HWM_FAN_BASE + 1))
#define SAB2000_ID_HWM_FAN_2				((SusiId_t)(SAB2000_ID_HWM_FAN_BASE + 2))
#define SAB2000_ID_HWM_FAN_OB1				((SusiId_t)(SAB2000_ID_HWM_FAN_BASE + 3))
#define SAB2000_ID_HWM_FAN_OB2				((SusiId_t)(SAB2000_ID_HWM_FAN_BASE + 4))
#define SAB2000_ID_HWM_FAN_OB3				((SusiId_t)(SAB2000_ID_HWM_FAN_BASE + 5))
#define SAB2000_ID_HWM_FAN_OB4				((SusiId_t)(SAB2000_ID_HWM_FAN_BASE + 6))
#define SAB2000_ID_HWM_FAN_OB5				((SusiId_t)(SAB2000_ID_HWM_FAN_BASE + 7))
#define SAB2000_ID_HWM_FAN_OB6				((SusiId_t)(SAB2000_ID_HWM_FAN_BASE + 8))
#define SAB2000_ID_HWM_FAN_OB7				((SusiId_t)(SAB2000_ID_HWM_FAN_BASE + 9))

#define SAB2000_ID_GSENSOR_TYPE				0x30000
#define SAB2000_ID_GSENSOR_BASE				(SAB2000_ID_BASE | SAB2000_ID_GSENSOR_TYPE)
#define SAB2000_ID_GSENSOR_AXIS_X			((SusiId_t)(SAB2000_ID_GSENSOR_BASE + 0))
#define SAB2000_ID_GSENSOR_AXIS_Y			((SusiId_t)(SAB2000_ID_GSENSOR_BASE + 1))
#define SAB2000_ID_GSENSOR_AXIS_Z			((SusiId_t)(SAB2000_ID_GSENSOR_BASE + 2))
#define SAB2000_ID_GSENSOR_FF_COUNT			((SusiId_t)(SAB2000_ID_GSENSOR_BASE + 3))
#define SAB2000_ID_GSENSOR_GVALUE			((SusiId_t)(SAB2000_ID_GSENSOR_BASE + 4))		// [1:0] 00		2g		(RW)
//		 01		4g
//		 10		8g
//		 11		16g

#define SAB2000_ID_LED_TYPE					0x31000
#define SAB2000_ID_LED_BASE					(SAB2000_ID_BASE | SAB2000_ID_LED_TYPE)
#define SAB2000_ID_LED_POWER				((SusiId_t)(SAB2000_ID_LED_BASE + 0))			// [2:0] 001	Green
//		 010	Red
//		 101	Green Blink
//		 110	Red Blink
//		 Other	N/A

#define SAB2000_ID_LED_TEMP					((SusiId_t)(SAB2000_ID_LED_BASE + 1))			// [2:0] 001	Green
//		 010	Red
//		 101	Green Blink
//		 110	Red Blink
//		 Other	N/A

#define SAB2000_ID_LED_FAN					((SusiId_t)(SAB2000_ID_LED_BASE + 2))			// [2:0] 001	Green
//		 010	Red
//		 101	Green Blink
//		 110	Red Blink
//		 Other	N/A

#endif /* _SAB2000_H_ */
