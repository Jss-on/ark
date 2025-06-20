#include "common.h"

enum funcRank {
	funcBuzzerOnOff,
	funcBuzzerFrequency,
	funcRTCS5Wake,
	NumFunc,
};

static int8_t functions[NumFunc + 1];	/* NumFunc + GoBack */

enum RTCRank {
	Day,
	Hour,
	Minute,
	Second,
	Save,
	NumRTC,
};

typedef struct _RTCConf {
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} RTCConf, *PRTCConf;

uint8_t set_init(void)
{
	SusiStatus_t status;
	uint32_t supFlag;
	status = SusiBoardGetValue(SUSI_ID_BOARD_SET_BASE, &supFlag);

	if (status == SUSI_STATUS_SUCCESS)
		return SUSIDEMO_DEVICE_AVAILALBE;
	else
		return SUSIDEMO_DEVICE_UNAVAILALBE;
}

static void set_title(void)
{
	printf("**********************************************\n");
	printf("**               SUSI4.0 demo               **\n");
	printf("**********************************************\n");
	printf("\nIntel Set Function\n\n");
}

static void RTC_title(void)
{
	printf("**********************************************\n");
	printf("**               SUSI4.0 demo               **\n");
	printf("**********************************************\n");
	printf("\nRTC S5 Wake\n\n");
}

static void menu()
{
	uint8_t i=1;
	SusiStatus_t status;
	uint32_t BuzzerStatus;
	functions[i-1] = SUSIDEMO_FUNCTIONS_GOBACK;
	printf("0) Back to Main menu\n");

	status = SusiBoardGetValue(SUSI_ID_BOARD_BUZZER_ONOFF_VAL, &BuzzerStatus);
	if (status == SUSI_STATUS_SUCCESS)
	{
		functions[i] = funcBuzzerOnOff;
		printf("%u) Buzzer : ", i);
		i++;
		if (BuzzerStatus == 0)
			printf("ON/[OFF]\n");
		else
			printf("[ON]/OFF\n");

		functions[i] = funcBuzzerFrequency;
		printf("%u) Set buzzer frequency\n", i);
		i++;
	}

	status = SusiBoardGetValue(SUSI_ID_BOARD_RTC_S5_WAKE_VAL, &BuzzerStatus);
	if (status == SUSI_STATUS_SUCCESS)
	{
		functions[i] = funcRTCS5Wake;
		printf("%u) Set RTC S5 wake \n", i);
	}

	printf("\nEnter your choice: ");
	fflush(stdout);
}

static uint32_t set_buzzer_onoff(uint32_t value)
{
	uint32_t status;
	status = SusiBoardSetValue(SUSI_ID_BOARD_BUZZER_ONOFF_VAL, value);
	return status;
}

static uint32_t set_buzzer_frequency(uint32_t value)
{
	uint32_t status;
	status = SusiBoardSetValue(SUSI_ID_BOARD_BUZZER_FREQUENCY_VAL, value);
	return status;
}

static uint32_t set_RTC_S5_WAKE(uint32_t value)
{
	uint32_t status;
	status = SusiBoardSetValue(SUSI_ID_BOARD_RTC_S5_WAKE_VAL, value);
	return status;
}

static void RTC_menu(RTCConf getconfig, RTCConf setconfig)
{
	RTC_title();
	printf("0) Back to Main menu\n");

	printf("1) Date of Month(0-31): %02d", getconfig.day);
	if (getconfig.day != setconfig.day)
		printf("->%02d \n", setconfig.day);
	else
		printf(" \n");

	printf("2) Resume Time(hh:0-23): %02d", getconfig.hour);
	if (getconfig.hour != setconfig.hour)
		printf("->%02d \n", setconfig.hour);
	else
		printf(" \n");

	printf("3) Resume Time(mm:0-59): %02d", getconfig.minute);
	if (getconfig.minute != setconfig.minute)
		printf("->%02d \n", setconfig.minute);
	else
		printf(" \n");

	printf("4) Resume Time(ss:0-59): %02d", getconfig.second);
	if (getconfig.second != setconfig.second)
		printf("->%02d \n", setconfig.second);
	else
		printf(" \n");

	printf("5) Save and apply setting \n");

	printf("\nEnter your choice: ");
	fflush(stdout);
}

static void set_RTC_S5_wake()
{
	SusiStatus_t status;
	uint32_t value;
	uint32_t op;
	uint32_t tmp_u32;
	RTCConf getconfig;
	RTCConf setconfig;

	status = SusiBoardGetValue(SUSI_ID_BOARD_RTC_S5_WAKE_VAL, &value);
	if (status == SUSI_STATUS_SUCCESS)
	{
		setconfig.day = (value >> 24) & 0xFF;
		setconfig.hour = (value >> 16) & 0xFF;
		setconfig.minute = (value >> 8) & 0xFF;
		setconfig.second = value & 0xFF;
	}

	while (1)
	{
		clr_screen();
		
		status = SusiBoardGetValue(SUSI_ID_BOARD_RTC_S5_WAKE_VAL, &value);
		if (status == SUSI_STATUS_SUCCESS)
		{
			getconfig.day = (value >> 24) & 0xFF;
			getconfig.hour = (value >> 16) & 0xFF;
			getconfig.minute = (value >> 8) & 0xFF;
			getconfig.second = value & 0xFF;
		}
		else
		{
			getconfig.day = 0;
			getconfig.hour = 0;
			getconfig.minute = 0;
			getconfig.second = 0;
		}
		RTC_menu(getconfig, setconfig);
		if (input_uint(&op, 10, NumRTC, 0))
			goto unknown;

		switch (op - 1)
		{
			case SUSIDEMO_FUNCTIONS_GOBACK:
				return;

			case Day:
				printf("Input Day(0-31): ");
				if (input_uint(&tmp_u32, 10, 31, 0))
					goto unknown;
				setconfig.day = (uint8_t)tmp_u32;
				break;

			case Hour:
				printf("Input Hour(0-23): ");
				if (input_uint(&tmp_u32, 10, 23, 0))
					goto unknown;
				setconfig.hour = (uint8_t)tmp_u32;
				break;

			case Minute:
				printf("Input Minute(0-59): ");
				if (input_uint(&tmp_u32, 10, 59, 0))
					goto unknown;
				setconfig.minute = (uint8_t)tmp_u32;
				break;

			case Second:
				printf("Input Second(0-59): ");
				if (input_uint(&tmp_u32, 10, 59, 0))
					goto unknown;
				setconfig.second = (uint8_t)tmp_u32;
				break;

			case Save:
				set_RTC_S5_WAKE(setconfig.day << 24 | setconfig.hour << 16 | setconfig.minute << 8 | setconfig.second);
				printf("Save success\n");
				break;
		}
	unknown:
		printf("Unknown choice!\n");
	}
}

void set_main(void)
{
	uint32_t op;
	uint32_t tmp_u32;

	while (1)
	{
		clr_screen();
		set_title();
		menu();

		if (input_uint(&op, 10, NELEMS(functions), 0))
			goto unknown;

		switch (functions[op])
		{
			case SUSIDEMO_FUNCTIONS_GOBACK:
				return;
		
			case funcBuzzerOnOff:
				printf("Input 0/1 (1:ON , 0:OFF): ");
				if (input_uint(&tmp_u32, 16, 1, 0))
				{
					printf("Invalid input\n");
					goto pause;
				}
				set_buzzer_onoff(tmp_u32);
				continue;

			case funcBuzzerFrequency:
				printf("Input Frequency(0-65535): ");
				if (input_uint(&tmp_u32, 10, 0xFFFF, 0))
				{
					printf("Invalid input\n");
					goto pause;
				}
				set_buzzer_frequency(tmp_u32);
				continue;

			case funcRTCS5Wake:
				set_RTC_S5_wake();
				continue;
		}
	unknown:
		printf("Unknown choice!\n");
	pause:
		printf("\nPress ENTER to continue. ");
		wait_enter();
	}
}
