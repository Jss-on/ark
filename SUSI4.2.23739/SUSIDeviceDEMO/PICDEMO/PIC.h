#ifndef _PIC_H_
#define _PIC_H_

#include "SusiDeviceAPI.h"
#include <string>

#define PICDEMO_PAGE_MAX 3

#define PIC_ID_BASE			0x00600000

#define PIC_ID_INFO_TYPE					0x00000
#define PIC_ID_INFO_BASE					(PIC_ID_BASE | PIC_ID_INFO_TYPE)
#define PIC_ID_INFO_AVAILABLE				((SusiId_t)(PIC_ID_INFO_BASE + 8))

//General Info
#define PIC_ID_FW_VER						((SusiId_t)(PIC_ID_INFO_BASE + 0))
#define PIC_ID_FW_CONFIG_MASK				((SusiId_t)(PIC_ID_INFO_BASE + 1))
#define PIC_ID_BOARD_ID						((SusiId_t)(PIC_ID_INFO_BASE + 2))
#define PIC_ID_BOARD_NAME_LEN				((SusiId_t)(PIC_ID_INFO_BASE + 3))
#define PIC_ID_BOARD_NAME1					((SusiId_t)(PIC_ID_INFO_BASE + 4))
#define PIC_ID_BOARD_NAME2					((SusiId_t)(PIC_ID_INFO_BASE + 5))
#define PIC_ID_BOARD_NAME3					((SusiId_t)(PIC_ID_INFO_BASE + 6))
#define PIC_ID_BOARD_NAME4					((SusiId_t)(PIC_ID_INFO_BASE + 7))

//Switch status
#define PIC_ID_SWITCH_TYPE					0x10000
#define PIC_ID_SWITCH_BASE					(PIC_ID_BASE | PIC_ID_SWITCH_TYPE)
#define PIC_ID_SWITCH_STATE					((SusiId_t)(PIC_ID_SWITCH_BASE + 0))
#define PIC_ID_SWITCH1_MODE					((SusiId_t)(PIC_ID_SWITCH_BASE + 1))
#define PIC_ID_SWITCH1_CFG_SELECT			((SusiId_t)(PIC_ID_SWITCH_BASE + 2))
#define PIC_ID_SWITCH2_PWR_SELECT			((SusiId_t)(PIC_ID_SWITCH_BASE + 3))

//FW info
#define PIC_ID_FW_TYPE						0x20000
#define PIC_ID_FW_BASE						(PIC_ID_BASE | PIC_ID_FW_TYPE)
#define PIC_ID_FW_STATE						((SusiId_t)(PIC_ID_FW_BASE + 0))
#define PIC_ID_FW_SYS_STATUS				((SusiId_t)(PIC_ID_FW_BASE + 1))
#define PIC_ID_FW_BAT_STATUS				((SusiId_t)(PIC_ID_FW_BASE + 2))
#define PIC_ID_FW_TMR_STATUS				((SusiId_t)(PIC_ID_FW_BASE + 3))
#define PIC_ID_FW_BAT_TYPE					((SusiId_t)(PIC_ID_FW_BASE + 4))
#define PIC_ID_FW_BAT_VOLT					((SusiId_t)(PIC_ID_FW_BASE + 5))
#define PIC_ID_FW_BAT_VOLT_STATUS			((SusiId_t)(PIC_ID_FW_BASE + 6))
#define PIC_ID_FW_BAT_ADC					((SusiId_t)(PIC_ID_FW_BASE + 7))
#define PIC_ID_FW_BAT_LOW_ADC				((SusiId_t)(PIC_ID_FW_BASE + 8))
#define PIC_ID_FW_SYSON_LEVEL				((SusiId_t)(PIC_ID_FW_BASE + 9))
#define PIC_ID_FW_IGN_LEVEL					((SusiId_t)(PIC_ID_FW_BASE + 10))
#define PIC_ID_FW_V12_STATUS				((SusiId_t)(PIC_ID_FW_BASE + 11))
#define PIC_ID_FW_V48_STATUS				((SusiId_t)(PIC_ID_FW_BASE + 12))
#define PIC_ID_FW_CHECK_SUM					((SusiId_t)(PIC_ID_FW_BASE + 15))

//HW config table
#define PIC_ID_HW_TYPE						0x30000
#define PIC_ID_HW_BASE						(PIC_ID_BASE | PIC_ID_HW_TYPE)
#define PIC_ID_HW_TAB_IGN1					((SusiId_t)(PIC_ID_HW_BASE + 0))
#define PIC_ID_HW_TAB_IGN2					((SusiId_t)(PIC_ID_HW_BASE + 1))
#define PIC_ID_HW_TAB_IGN3					((SusiId_t)(PIC_ID_HW_BASE + 2))
#define PIC_ID_HW_TAB_IGN4					((SusiId_t)(PIC_ID_HW_BASE + 3))
#define PIC_ID_HW_TAB_IGN5					((SusiId_t)(PIC_ID_HW_BASE + 4))
#define PIC_ID_HW_TAB_IGN6					((SusiId_t)(PIC_ID_HW_BASE + 5))
#define PIC_ID_HW_TAB_IGN7					((SusiId_t)(PIC_ID_HW_BASE + 6))
#define PIC_ID_HW_TAB_IGN8					((SusiId_t)(PIC_ID_HW_BASE + 7))
#define PIC_ID_HW_TAB_DELAY_OFF1			((SusiId_t)(PIC_ID_HW_BASE + 8))
#define PIC_ID_HW_TAB_DELAY_OFF2			((SusiId_t)(PIC_ID_HW_BASE + 9))
#define PIC_ID_HW_TAB_DELAY_OFF3			((SusiId_t)(PIC_ID_HW_BASE + 10))
#define PIC_ID_HW_TAB_DELAY_OFF4			((SusiId_t)(PIC_ID_HW_BASE + 11))
#define PIC_ID_HW_TAB_DELAY_OFF5			((SusiId_t)(PIC_ID_HW_BASE + 12))
#define PIC_ID_HW_TAB_DELAY_OFF6			((SusiId_t)(PIC_ID_HW_BASE + 13))
#define PIC_ID_HW_TAB_DELAY_OFF7			((SusiId_t)(PIC_ID_HW_BASE + 14))
#define PIC_ID_HW_TAB_DELAY_OFF8			((SusiId_t)(PIC_ID_HW_BASE + 15))

//Setup SW settings
#define PIC_ID_SET_TYPE						0x80000
#define PIC_ID_SET_BASE						(PIC_ID_BASE | PIC_ID_SET_TYPE)
#define PIC_ID_SET_IGN_DELAY				((SusiId_t)(PIC_ID_SET_BASE + 0))
#define PIC_ID_SET_DELAY_OFF				((SusiId_t)(PIC_ID_SET_BASE + 1))
#define PIC_ID_SET_HARD_OFF					((SusiId_t)(PIC_ID_SET_BASE + 2))
#define PIC_ID_SET_PWR_RETRIES				((SusiId_t)(PIC_ID_SET_BASE + 3))
#define PIC_ID_SET_PWR_INTERVAL				((SusiId_t)(PIC_ID_SET_BASE + 4))
#define PIC_ID_SET_BL_12V					((SusiId_t)(PIC_ID_SET_BASE + 5))
#define PIC_ID_SET_BL_24V					((SusiId_t)(PIC_ID_SET_BASE + 6))
#define PIC_ID_SET_BL_DELAY_OFF				((SusiId_t)(PIC_ID_SET_BASE + 7))
#define PIC_ID_SET_BL_HARD_OFF				((SusiId_t)(PIC_ID_SET_BASE + 8))
#define PIC_ID_SET_BAT_LOW_SWITCH			((SusiId_t)(PIC_ID_SET_BASE + 9))
#define PIC_ID_SET_BAT_TYPE					((SusiId_t)(PIC_ID_SET_BASE + 10))

//Get current settings		
#define PIC_ID_GET_TYPE						0x90000
#define PIC_ID_GET_BASE						(PIC_ID_BASE | PIC_ID_GET_TYPE)
#define PIC_ID_GET_IGN_DELAY				((SusiId_t)(PIC_ID_GET_BASE + 0))
#define PIC_ID_GET_DELAY_OFF				((SusiId_t)(PIC_ID_GET_BASE + 1))
#define PIC_ID_GET_HARD_OFF					((SusiId_t)(PIC_ID_GET_BASE + 2))
#define PIC_ID_GET_PWR_RETRIES				((SusiId_t)(PIC_ID_GET_BASE + 3))
#define PIC_ID_GET_PWR_INTERVAL				((SusiId_t)(PIC_ID_GET_BASE + 4))
#define PIC_ID_GET_BL_12V					((SusiId_t)(PIC_ID_GET_BASE + 5))
#define PIC_ID_GET_BL_24V					((SusiId_t)(PIC_ID_GET_BASE + 6))
#define PIC_ID_GET_BL_DELAY_OFF				((SusiId_t)(PIC_ID_GET_BASE + 7))
#define PIC_ID_GET_BL_HARD_OFF				((SusiId_t)(PIC_ID_GET_BASE + 8))
#define PIC_ID_GET_BAT_LOW_SWITCH			((SusiId_t)(PIC_ID_GET_BASE + 9))
#define PIC_ID_GET_BAT_TYPE					((SusiId_t)(PIC_ID_GET_BASE + 10))


//Timer		
#define PIC_ID_TIMER_TYPE					0xA0000
#define PIC_ID_TIMER_BASE					(PIC_ID_BASE | PIC_ID_TIMER_TYPE)
#define PIC_ID_TIMER_TMR_IGN_ON				((SusiId_t)(PIC_ID_TIMER_BASE + 0))
#define PIC_ID_TIMER_PWR_ON_RETRIES			((SusiId_t)(PIC_ID_TIMER_BASE + 1))
#define PIC_ID_TIMER_PWR_ON_INTERVAL		((SusiId_t)(PIC_ID_TIMER_BASE + 2))
#define PIC_ID_TIMER_PWR_OFF_RETRIES		((SusiId_t)(PIC_ID_TIMER_BASE + 3))
#define PIC_ID_TIMER_PWR_OFF_INTERVAL		((SusiId_t)(PIC_ID_TIMER_BASE + 4))
#define PIC_ID_TIMER_TMR_DELAY_OFF			((SusiId_t)(PIC_ID_TIMER_BASE + 5))
#define PIC_ID_TIMER_TMR_HARD_OFF			((SusiId_t)(PIC_ID_TIMER_BASE + 6))
#define PIC_ID_TIMER_TMR_BL_DELAY_OFF		((SusiId_t)(PIC_ID_TIMER_BASE + 7))
#define PIC_ID_TIMER_PWR_12V48V_INTERVAL	((SusiId_t)(PIC_ID_TIMER_BASE + 8))
#define PIC_ID_TIMER_TMR_FW_UP_TIME			((SusiId_t)(PIC_ID_TIMER_BASE + 15))

//Min. settings		
#define PIC_ID_MIN_TYPE						0xB0000
#define PIC_ID_MIN_BASE						(PIC_ID_BASE | PIC_ID_MIN_TYPE)
#define PIC_ID_MIN_IGN_DELAY				((SusiId_t)(PIC_ID_MIN_BASE + 0))
#define PIC_ID_MIN_DELAY_OFF				((SusiId_t)(PIC_ID_MIN_BASE + 1))
#define PIC_ID_MIN_HARD_OFF					((SusiId_t)(PIC_ID_MIN_BASE + 2))
#define PIC_ID_MIN_PWR_RETRIES				((SusiId_t)(PIC_ID_MIN_BASE + 3))
#define PIC_ID_MIN_PWR_INTERVAL				((SusiId_t)(PIC_ID_MIN_BASE + 4))
#define PIC_ID_MIN_BL_12V					((SusiId_t)(PIC_ID_MIN_BASE + 5))
#define PIC_ID_MIN_BL_24V					((SusiId_t)(PIC_ID_MIN_BASE + 6))
#define PIC_ID_MIN_BL_DELAY_OFF				((SusiId_t)(PIC_ID_MIN_BASE + 7))
#define PIC_ID_MIN_BL_HARD_OFF				((SusiId_t)(PIC_ID_MIN_BASE + 8))
#define PIC_ID_MIN_BAT_LOW_SWITCH			((SusiId_t)(PIC_ID_MIN_BASE + 9))
#define PIC_ID_MIN_BAT_TYPE					((SusiId_t)(PIC_ID_MIN_BASE + 10))

//Max. settings		
#define PIC_ID_MAX_TYPE						0xC0000
#define PIC_ID_MAX_BASE						(PIC_ID_BASE | PIC_ID_MAX_TYPE)
#define PIC_ID_MAX_IGN_DELAY				((SusiId_t)(PIC_ID_MAX_BASE + 0))
#define PIC_ID_MAX_DELAY_OFF				((SusiId_t)(PIC_ID_MAX_BASE + 1))
#define PIC_ID_MAX_HARD_OFF					((SusiId_t)(PIC_ID_MAX_BASE + 2))
#define PIC_ID_MAX_PWR_RETRIES				((SusiId_t)(PIC_ID_MAX_BASE + 3))
#define PIC_ID_MAX_PWR_INTERVAL				((SusiId_t)(PIC_ID_MAX_BASE + 4))
#define PIC_ID_MAX_BL_12V					((SusiId_t)(PIC_ID_MAX_BASE + 5))
#define PIC_ID_MAX_BL_24V					((SusiId_t)(PIC_ID_MAX_BASE + 6))
#define PIC_ID_MAX_BL_DELAY_OFF				((SusiId_t)(PIC_ID_MAX_BASE + 7))
#define PIC_ID_MAX_BL_HARD_OFF				((SusiId_t)(PIC_ID_MAX_BASE + 8))
#define PIC_ID_MAX_BAT_LOW_SWITCH			((SusiId_t)(PIC_ID_MAX_BASE + 9))
#define PIC_ID_MAX_BAT_TYPE					((SusiId_t)(PIC_ID_MAX_BASE + 10))

//Default settings	
#define PIC_ID_DEF_TYPE						0xD0000
#define PIC_ID_DEF_BASE						(PIC_ID_BASE | PIC_ID_DEF_TYPE)
#define PIC_ID_DEF_IGN_DELAY				((SusiId_t)(PIC_ID_DEF_BASE + 0))
#define PIC_ID_DEF_DELAY_OFF				((SusiId_t)(PIC_ID_DEF_BASE + 1))
#define PIC_ID_DEF_HARD_OFF					((SusiId_t)(PIC_ID_DEF_BASE + 2))
#define PIC_ID_DEF_PWR_RETRIES				((SusiId_t)(PIC_ID_DEF_BASE + 3))
#define PIC_ID_DEF_PWR_INTERVAL				((SusiId_t)(PIC_ID_DEF_BASE + 4))
#define PIC_ID_DEF_BL_12V					((SusiId_t)(PIC_ID_DEF_BASE + 5))
#define PIC_ID_DEF_BL_24V					((SusiId_t)(PIC_ID_DEF_BASE + 6))
#define PIC_ID_DEF_BL_DELAY_OFF				((SusiId_t)(PIC_ID_DEF_BASE + 7))
#define PIC_ID_DEF_BL_HARD_OFF				((SusiId_t)(PIC_ID_DEF_BASE + 8))
#define PIC_ID_DEF_BAT_LOW_SWITCH			((SusiId_t)(PIC_ID_DEF_BASE + 9))
#define PIC_ID_DEF_BAT_TYPE					((SusiId_t)(PIC_ID_DEF_BASE + 10))

//Read EEPROM data	
#define PIC_ID_EEPROM_TYPE					0xE0000
#define PIC_ID_EEPROM_BASE					(PIC_ID_BASE | PIC_ID_EEPROM_TYPE)
#define PIC_ID_EEPROM_DATA1					((SusiId_t)(PIC_ID_EEPROM_BASE + 0))
#define PIC_ID_EEPROM_DATA2					((SusiId_t)(PIC_ID_EEPROM_BASE + 1))
#define PIC_ID_EEPROM_DATA3					((SusiId_t)(PIC_ID_EEPROM_BASE + 2))
#define PIC_ID_EEPROM_DATA4					((SusiId_t)(PIC_ID_EEPROM_BASE + 3))
#define PIC_ID_EEPROM_DATA5					((SusiId_t)(PIC_ID_EEPROM_BASE + 4))
#define PIC_ID_EEPROM_DATA6					((SusiId_t)(PIC_ID_EEPROM_BASE + 5))
#define PIC_ID_EEPROM_DATA7					((SusiId_t)(PIC_ID_EEPROM_BASE + 6))
#define PIC_ID_EEPROM_DATA8					((SusiId_t)(PIC_ID_EEPROM_BASE + 7))
#define PIC_ID_EEPROM_DATA9					((SusiId_t)(PIC_ID_EEPROM_BASE + 8))
#define PIC_ID_EEPROM_DATA10				((SusiId_t)(PIC_ID_EEPROM_BASE + 9))
#define PIC_ID_EEPROM_DATA11				((SusiId_t)(PIC_ID_EEPROM_BASE + 10))
#define PIC_ID_EEPROM_DATA12				((SusiId_t)(PIC_ID_EEPROM_BASE + 11))
#define PIC_ID_EEPROM_DATA13				((SusiId_t)(PIC_ID_EEPROM_BASE + 12))
#define PIC_ID_EEPROM_DATA14				((SusiId_t)(PIC_ID_EEPROM_BASE + 13))
#define PIC_ID_EEPROM_DATA15				((SusiId_t)(PIC_ID_EEPROM_BASE + 14))

//System Command		
#define PIC_ID_SYSTEM_TYPE					0xF0000
#define PIC_ID_SYSTEM_BASE					(PIC_ID_BASE | PIC_ID_SYSTEM_TYPE)
#define PIC_ID_SYSTEM_GET_PIC_CHECKSUM		((SusiId_t)(PIC_ID_SYSTEM_BASE + 0))
#define PIC_ID_SYSTEM_GET_PIC_CONFIG1		((SusiId_t)(PIC_ID_SYSTEM_BASE + 1))
#define PIC_ID_SYSTEM_GET_PIC_CONFIG2		((SusiId_t)(PIC_ID_SYSTEM_BASE + 2))
#define PIC_ID_SYSTEM_GET_PIC_DEVICE_ID		((SusiId_t)(PIC_ID_SYSTEM_BASE + 3))
#define PIC_ID_SYSTEM_GET_PIC_USER_ID0		((SusiId_t)(PIC_ID_SYSTEM_BASE + 4))
#define PIC_ID_SYSTEM_GET_PIC_USER_ID1		((SusiId_t)(PIC_ID_SYSTEM_BASE + 5))
#define PIC_ID_SYSTEM_GET_PIC_USER_ID2		((SusiId_t)(PIC_ID_SYSTEM_BASE + 6))
#define PIC_ID_SYSTEM_GET_PIC_USER_ID3		((SusiId_t)(PIC_ID_SYSTEM_BASE + 7))
#define PIC_ID_SYSTEM_SET_DEFAULT			((SusiId_t)(PIC_ID_SYSTEM_BASE + 14))
#define PIC_ID_SYSTEM_SET_PIC_RESET			((SusiId_t)(PIC_ID_SYSTEM_BASE + 15))

class PIC {
public:
	static uint32_t _powerStatus;
	uint32_t _portNum;
	uint32_t _enableData;
	uint32_t _disableData;
	uint32_t _power;
public:
	PIC();

	static bool isAvailable();
	static void page_status();
	static void page_set_up();
	static void page_real_time_status();
	static void wait_enter_key(void);
};

#endif /* _PIC_H_ */