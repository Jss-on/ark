#include "common.h"
#include "ADXL345.h"

#define SUSIDEV_GSENSOR_AXIS_X	0x00
#define SUSIDEV_GSENSOR_AXIS_Y	0x01
#define SUSIDEV_GSENSOR_AXIS_Z	0x02

static const struct Rank2SUSIValue rate_tbl[] = {
	//{SUSIDEV_GSENSOR_RATE_P098,	"0.098"},
	//{SUSIDEV_GSENSOR_RATE_P195,	"0.195"},
	//{SUSIDEV_GSENSOR_RATE_P390,	"0.390"},
	//{SUSIDEV_GSENSOR_RATE_P782,	"0.782"},
	//{SUSIDEV_GSENSOR_RATE_1P563,	"1.563"},
	//{SUSIDEV_GSENSOR_RATE_3P125,	"3.125"},
	//{SUSIDEV_GSENSOR_RATE_6P25,	"6.25"},
	{SUSIDEV_GSENSOR_RATE_12P5,		"12.5"},
	//{SUSIDEV_GSENSOR_RATE_25,		"25"},
	{SUSIDEV_GSENSOR_RATE_50,		"50"},
	{SUSIDEV_GSENSOR_RATE_100,		"100"},
	//{SUSIDEV_GSENSOR_RATE_200,	"200"},
	{SUSIDEV_GSENSOR_RATE_400,		"400"},
	//{SUSIDEV_GSENSOR_RATE_800,	"800"},
	{SUSIDEV_GSENSOR_RATE_1600,		"1600"},
	//{SUSIDEV_GSENSOR_RATE_3200,	"3200"},
};

static const struct Rank2SUSIValue grange_tbl[] = {
	{SUSIDEV_GSENSOR_GRANGE_2G,		"2g"},
	{SUSIDEV_GSENSOR_GRANGE_4G,		"4g"},
	{SUSIDEV_GSENSOR_GRANGE_8G,		"8g"},
	{SUSIDEV_GSENSOR_GRANGE_16G,	"16g"}
};

enum funcRank{
	funcData,
	funcOfsGet,
	funcOfsSetX,
	funcOfsSetY,
	funcOfsSetZ,	
	funcRate,
	funcRange,
	NumFunc,
};
static int8_t functions[NumFunc + 1];

struct GSensorInfo {
	uint32_t OfsX;
	uint32_t OfsY;
	uint32_t OfsZ;
	uint32_t Rate;
	uint32_t Range;
};

uint8_t gsnr_init()
{
	SusiStatus_t status;
	uint32_t tmp;

	// check device is exist
	status = SusiDeviceGetValue(ADXL345_ID_INFO_AVAILABLE, &tmp);
	if (status != SUSI_STATUS_SUCCESS)
		return SUSIDEMO_DEVICE_UNAVAILALBE;
	if (tmp == 0)
		return SUSIDEMO_DEVICE_UNAVAILALBE;

	// initial setting
	status = SusiDeviceSetValue(ADXL345_ID_MEASURE_CTRL, 1);
	if (status != SUSI_STATUS_SUCCESS)
		return SUSIDEMO_DEVICE_UNAVAILALBE;

	status = SusiDeviceSetValue(ADXL345_ID_POWER_LOWPOWER, 0);
	if (status != SUSI_STATUS_SUCCESS)
		return SUSIDEMO_DEVICE_UNAVAILALBE;

	status = SusiDeviceSetValue(ADXL345_ID_POWER_SLEEP, 0);
	if (status != SUSI_STATUS_SUCCESS)
		return SUSIDEMO_DEVICE_UNAVAILALBE;

	return SUSIDEMO_DEVICE_AVAILALBE;
}

static void title()
{
	printf(
		"**********************************************\n"
		"**            SUSI4.0 device demo           **\n"
		"**********************************************\n\n");

	printf("g-Sensor\n\n");
}

static void gsnr_set_config(struct GSensorInfo config)
{
	SusiStatus_t status;

	status = SusiDeviceSetValue(ADXL345_ID_OFFSET_X, config.OfsX);
	if (status != SUSI_STATUS_SUCCESS)
		printf("Get x-offset failed. (0x%08X)\n", status);

	status = SusiDeviceSetValue(ADXL345_ID_OFFSET_Y, config.OfsY);
	if (status != SUSI_STATUS_SUCCESS)
		printf("Get y-offset failed. (0x%08X)\n", status);

	status = SusiDeviceSetValue(ADXL345_ID_OFFSET_Z, config.OfsZ);
	if (status != SUSI_STATUS_SUCCESS)
		printf("Get z-offset failed. (0x%08X)\n", status);

	status = SusiDeviceSetValue(ADXL345_ID_DATARATE_NORMAL, config.Rate);
	if (status != SUSI_STATUS_SUCCESS)
		printf("Set rate failed. (0x%08X)\n", status);

	status = SusiDeviceSetValue(ADXL345_ID_MEASURE_RANGE, config.Range);
	if (status != SUSI_STATUS_SUCCESS)
		printf("Set g-range failed. (0x%08X)\n", status);
}

static uint8_t gsnr_get_data()
{
	SusiStatus_t status;
	uint32_t tmp_u32;

	printf("  ");

	status = SusiDeviceGetValue(ADXL345_ID_DATA_X, &tmp_u32);
	if (status == SUSI_STATUS_SUCCESS)
		printf("% 2.3f", (int)tmp_u32 / (float)10000);
	else
		printf(" (N/A)");

	printf("  ");

	status = SusiDeviceGetValue(ADXL345_ID_DATA_Y, &tmp_u32);
	if (status == SUSI_STATUS_SUCCESS)
		printf("% 2.3f", (int)tmp_u32 / (float)10000);
	else
		printf(" (N/A)");

	printf("  ");

	status = SusiDeviceGetValue(ADXL345_ID_DATA_Z, &tmp_u32);
	if (status == SUSI_STATUS_SUCCESS)
		printf("% 2.3f", (int)tmp_u32 / (float)10000);
	else
		printf(" (N/A)");

	printf("  ");

	return SUSIDEMO_PRINT_SUCCESS;
}

static uint8_t gsnr_scan(struct GSensorInfo config)
{
	int c = '\n';

	gsnr_set_config(config);

	printf("\nPress 'Enter' to continue. Enter 'q' to stop.\n\n");
	printf("Scan:\n       X       Y       Z\n");

	do {
		if (c == '\n' || c == EOF)
			gsnr_get_data();
	} while ((c = getchar()) != 'Q' && c != 'q');

	return SUSIDEMO_PRINT_NONE;
}

static uint8_t gsnr_get_offset(struct GSensorInfo *config)
{
	SusiStatus_t status;
	uint32_t tmp_u32[3];

	status = SusiDeviceGetValue(ADXL345_ID_OFFSET_X, &tmp_u32[0]);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("Get x-offset failed. (0x%08X)\n", status);
		return SUSIDEMO_PRINT_ERROR;
	}

	status = SusiDeviceGetValue(ADXL345_ID_OFFSET_Y, &tmp_u32[1]);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("Get y-offset failed. (0x%08X)\n", status);
		return SUSIDEMO_PRINT_ERROR;
	}

	status = SusiDeviceGetValue(ADXL345_ID_OFFSET_Z, &tmp_u32[2]);
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("Get z-offset failed. (0x%08X)\n", status);
		return SUSIDEMO_PRINT_ERROR;
	}

	config->OfsX = tmp_u32[0];
	config->OfsY = tmp_u32[1];
	config->OfsZ = tmp_u32[2];

	printf("\n");
	printf("X-offset: %.4f g\n", (int32_t)config->OfsX / (float)10000);
	printf("Y-offset: %.4f g\n", (int32_t)config->OfsY / (float)10000);
	printf("Z-offset: %.4f g\n", (int32_t)config->OfsZ / (float)10000);

	return SUSIDEMO_PRINT_SUCCESS;
}

static uint8_t gsnr_set_an_axis_offset(uint8_t axis)
{
	SusiStatus_t status;
	int32_t tmp_32;
	const char *label[] = {"x", "y", "z"};

	printf("%s-offset (-2000 to 2000 mg): ", label[axis]);
	if (input_int(&tmp_32, 2000, -2000))
	{
		printf("Invalid input\n");
		return SUSIDEMO_PRINT_ERROR;
	}

	switch (axis)
	{
	case SUSIDEV_GSENSOR_AXIS_X:
		status = SusiDeviceSetValue(ADXL345_ID_OFFSET_X, tmp_32 * 10);
		break;

	case SUSIDEV_GSENSOR_AXIS_Y:
		status = SusiDeviceSetValue(ADXL345_ID_OFFSET_Y, tmp_32 * 10);
		break;

	case SUSIDEV_GSENSOR_AXIS_Z:
		status = SusiDeviceSetValue(ADXL345_ID_OFFSET_Z, tmp_32 * 10);
		break;

	default:
		status = SUSI_STATUS_UNSUPPORTED;
	}
	
	if (status != SUSI_STATUS_SUCCESS)
	{
		printf("Set %s-offset failed. (0x%08X)\n", label[axis], status);
		return SUSIDEMO_PRINT_ERROR;
	}

	printf("Set %s-offset succeed.\n", label[axis]);
	return SUSIDEMO_PRINT_SUCCESS;
}

static uint8_t gsnr_normal_rate(uint32_t *pvalue)
{
	uint32_t i = 0;

	for (; i < NELEMS(rate_tbl); i++)
	{
		if (rate_tbl[i].value == *pvalue)
			break;
	}
	
	if(i >= NELEMS(rate_tbl))
	{
		printf("No correspond rate in this sample for value %u.\n", *pvalue);
		return SUSIDEMO_PRINT_ERROR;
	}

	if (i + 1 < NELEMS(rate_tbl))
		*pvalue =rate_tbl[i + 1].value;
	else
		*pvalue = rate_tbl[0].value;

	return SUSIDEMO_PRINT_NONE;
}

static uint8_t gsnr_measure_grange(uint32_t *pvalue)
{
	uint32_t i = 0;
	
	for (; i < NELEMS(grange_tbl); i++)
	{
		if (grange_tbl[i].value == *pvalue)
			break;
	}
	
	if (i >= NELEMS(grange_tbl))
	{
		printf("No correspond g-Range for value %u.\n", *pvalue);
		return SUSIDEMO_PRINT_ERROR;
	}

	if (i + 1 < NELEMS(grange_tbl))
		*pvalue =grange_tbl[i + 1].value;
	else
		*pvalue = grange_tbl[0].value;

	return SUSIDEMO_PRINT_NONE;
}

static void menu(struct GSensorInfo config)
{
	uint8_t fnc = 0, i;

	printf("%u) Terminate g-Sensor Demo\n", fnc);
	functions[fnc++] = SUSIDEMO_FUNCTIONS_GOBACK;

	printf("%u) Get Offsets\n", fnc);
	functions[fnc++] = funcOfsGet;

	printf("%u) X-offset: %.4f g\n", fnc, (int32_t)config.OfsX / (float)10000);
	functions[fnc++] = funcOfsSetX;

	printf("%u) Y-offset: %.4f g\n", fnc, (int32_t)config.OfsY / (float)10000);
	functions[fnc++] = funcOfsSetY;

	printf("%u) Z-offset: %.4f g\n", fnc, (int32_t)config.OfsZ / (float)10000);
	functions[fnc++] = funcOfsSetZ;

	printf("%u) Rate (Hz): ", fnc);
	for (i = 0; i < NELEMS(rate_tbl); i++)
	{
		if (i > 0)
			printf("/");

		if (rate_tbl[i].value == config.Rate)
			printf("[%s]", rate_tbl[i].label);
		else
			printf(" %s ", rate_tbl[i].label);
	}
	printf("\n");
	functions[fnc++] = funcRate;

	printf("%u) Measure Range: ", fnc);
	for (i = 0; i < NELEMS(grange_tbl); i++)
	{
		if (i > 0)
			printf("/");

		if (grange_tbl[i].value == config.Range)
			printf("[%s]", grange_tbl[i].label);
		else
			printf(" %s ", grange_tbl[i].label);
	}
	printf("\n");
	functions[fnc++] = funcRange;

	printf("%u) Start to Scan\n", fnc);
	functions[fnc++] = funcData;
	
	printf("\nEnter your choice: ");
}

void gsnr_main(void)
{
	uint32_t op;
	struct GSensorInfo config;

	gsnr_get_offset(&config);
	config.Rate = SUSIDEV_GSENSOR_RATE_100;
	config.Range = SUSIDEV_GSENSOR_GRANGE_2G;
	
	while (1)
	{
		clr_screen();
		title();
		menu(config);

		if (input_uint(&op, 10, NELEMS(functions) - 1, 0))
			goto unknown;

		switch (functions[op])
		{
		case SUSIDEMO_FUNCTIONS_GOBACK:
			return;

		case funcData:
			if (gsnr_scan(config))
				goto pause;
			continue;

		case funcOfsSetX:
			if (gsnr_set_an_axis_offset(SUSIDEV_GSENSOR_AXIS_X))
				goto pause;
			continue;

		case funcOfsSetY:
			if (gsnr_set_an_axis_offset(SUSIDEV_GSENSOR_AXIS_Y))
				goto pause;
			continue;

		case funcOfsSetZ:
			if (gsnr_set_an_axis_offset(SUSIDEV_GSENSOR_AXIS_Z))
				goto pause;
			continue;

		case funcOfsGet:
			if (gsnr_get_offset(&config))
				goto pause;
			continue;

		case funcRate:
			if (gsnr_normal_rate(&config.Rate))
				goto pause;
			continue;

		case funcRange:
			if (gsnr_measure_grange(&config.Range))
				goto pause;
			continue;
		}
unknown:
		printf("Unknown choice!\n");
pause:
		printf("\nPress ENTER to continue. ");
		wait_enter();
	}
}

int main(void)
{
	int ret = 0;

	if (gsnr_init() == SUSIDEMO_DEVICE_AVAILALBE)
	{
		gsnr_main();
	}
	else
	{
		printf("g-Sensor device is not exist or initialize failed.\n");
		ret = -1;
	}

	printf("Exit the program...\n");

	return ret;
}
