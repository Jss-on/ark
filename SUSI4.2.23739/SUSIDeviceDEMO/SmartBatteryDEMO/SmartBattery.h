#ifndef _SmartBattery_H_
#define _SmartBattery_H_

#include "SusiDeviceAPI.h"
//=============================================================================
// ID Format:
//
//	31		24	20			12	8		0
//	+-------+---+-----------+---+-------+
//	|  N/A	| C	|   Type	| S | Index	|
//	+-------+---+-----------+---+-------+
//
//	Where:
//		C		SBS = 0b1011
//		Type	ID Type code
//		S		ID Sub-Type code
//		Index	Device Unit
//=============================================================================
#define SBS_ID_BASE					0x00B00000

/*	Value		   | Unit
// ----------------+----------------
//	0				mA
//	1				10 mW
*/
#define SBS_ID_unit							((SusiId_t)(SBS_ID_BASE + 0x00))
/* Range 0-700 */
#define SBS_ID_RemainingCapacityAlarm		((SusiId_t)(SBS_ID_BASE + 0x01))
/* Range 0-30 */
#define SBS_ID_RemainingTimeAlarm			((SusiId_t)(SBS_ID_BASE + 0x02))
/* Range 0x0000-0xFFFF */
#define SBS_ID_BatteryMode					((SusiId_t)(SBS_ID_BASE + 0x03))
/* Range -32768-32767 */
#define SBS_ID_AtRate						((SusiId_t)(SBS_ID_BASE + 0x04))
/* Range 0-65535 */
#define SBS_ID_AtRateTimeToFull				((SusiId_t)(SBS_ID_BASE + 0x05))
/* Range 0-65535 */
#define SBS_ID_AtRateTimeToEmpty			((SusiId_t)(SBS_ID_BASE + 0x06))
/* Range 0-65535 */
#define SBS_ID_AtRateOK						((SusiId_t)(SBS_ID_BASE + 0x07))
/* Range 0-65535 */
#define SBS_ID_Temperature					((SusiId_t)(SBS_ID_BASE + 0x08))
/* Range 0-65535 */
#define SBS_ID_Voltage						((SusiId_t)(SBS_ID_BASE + 0x09))
/* Range -32767-32768 */
#define SBS_ID_Current						((SusiId_t)(SBS_ID_BASE + 0x0A))
/* Range -32767-32768 */
#define SBS_ID_AverageCurrent				((SusiId_t)(SBS_ID_BASE + 0x0B))
/* Range 0-100 */
#define SBS_ID_MaxError						((SusiId_t)(SBS_ID_BASE + 0x0C))
/* Range 0-100 */
#define SBS_ID_RelativeStateOfCharge		((SusiId_t)(SBS_ID_BASE + 0x0D))
/* Range 0-100 */
#define SBS_ID_AbsoluteStateOfCharge		((SusiId_t)(SBS_ID_BASE + 0x0E))
/* Range 0-65535 */
#define SBS_ID_RemainingCapacity			((SusiId_t)(SBS_ID_BASE + 0x0F))
/* Range 0-65535 */
#define SBS_ID_FullChargeCapacity			((SusiId_t)(SBS_ID_BASE + 0x10))
/* Range 0-65535 */
#define SBS_ID_RunTimeToEmpty				((SusiId_t)(SBS_ID_BASE + 0x11))
/* Range 0-65535 */
#define SBS_ID_AverageTimeToEmpty			((SusiId_t)(SBS_ID_BASE + 0x12))
/* Range 0-65535 */
#define SBS_ID_AverageTimeToFull			((SusiId_t)(SBS_ID_BASE + 0x13))
/* Range 0-65535 */
#define SBS_ID_ChargingCurrent				((SusiId_t)(SBS_ID_BASE + 0x14))
/* Range 0-65535 */
#define SBS_ID_ChargingVoltage				((SusiId_t)(SBS_ID_BASE + 0x15))
#define SBS_ID_BatteryStatus				((SusiId_t)(SBS_ID_BASE + 0x16))
/* Range 0-65535 */
#define SBS_ID_CycleCount					((SusiId_t)(SBS_ID_BASE + 0x17))
/* Range 0-65535 */
#define SBS_ID_DesignCapacity				((SusiId_t)(SBS_ID_BASE + 0x18))
/* Range 7000-18000 */
#define SBS_ID_DesignVoltage				((SusiId_t)(SBS_ID_BASE + 0x19))
#define SBS_ID_SpecificationInfo			((SusiId_t)(SBS_ID_BASE + 0x1A))
/* format: Day + Month*32 + (Year¡V1980)*256 */
#define SBS_ID_ManufacturerDate				((SusiId_t)(SBS_ID_BASE + 0x1B))
/* Range 0x0000-0xFFFF */
#define SBS_ID_SerialNumber					((SusiId_t)(SBS_ID_BASE + 0x1C))
/* Runtime FullChargeCapacity/Design Capacity */
#define SBS_ID_SoH							((SusiId_t)(SBS_ID_BASE + 0x4F))


#define SBS_ID_ManufacturerName_Len			((SusiId_t)(SBS_ID_BASE + 0x20))
#define SBS_ID_ManufacturerName1			((SusiId_t)(SBS_ID_BASE + 0x24))
#define SBS_ID_ManufacturerName2			((SusiId_t)(SBS_ID_BASE + 0x25))
#define SBS_ID_ManufacturerName3			((SusiId_t)(SBS_ID_BASE + 0x26))
#define SBS_ID_ManufacturerName4			((SusiId_t)(SBS_ID_BASE + 0x27))
#define SBS_ID_ManufacturerName5			((SusiId_t)(SBS_ID_BASE + 0x28))

class SmartBattery{
private:

public:
	static bool isAvailable();
	static void GetBatteryInfo();
	static void wait_enter_key();
};

#endif /* _SmartBattery_H_ */