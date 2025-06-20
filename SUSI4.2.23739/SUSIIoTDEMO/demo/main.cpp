#define _CRT_SECURE_NO_DEPRECATE
#include "common.h"

enum MENU_OPTIONS {
    OPT_EXIT                   = 0,
    OPT_CAPABILITY_OBJECT      = 1,
    OPT_CAPABILITY_STRING      = 2,
    OPT_GET_DATA_OBJECT        = 3,
    OPT_GET_DATA_STRING        = 4,
    OPT_GET_DATA_OBJECT_BY_URI = 5,
    OPT_GET_DATA_STRING_BY_URI = 6,
    OPT_GET_DATA_VALUE         = 7,
    OPT_SET_DATA_OBJECT        = 8,
    OPT_SET_DATA_STRING        = 9,
    OPT_SET_DATA_VALUE         = 10,
    OPT_ENABLE_LOGGER          = 11,
    OPT_DISABLE_LOGGER         = 12,
    OPT_GET_LOGGER_PATH        = 13,
    OPT_GET_LOGGER_STATUS      = 14,
    OPT_TEST_FUNCTION          = 99
};

enum JSON_OBJ_TYPES {
    TYPE_BOOLEAN = 0,
    TYPE_INTEGER = 1,
    TYPE_REAL    = 2,
    TYPE_STRING  = 3,
    TYPE_NULL    = 4
};

union JSON_OBJ_VALUE {
    int i;
    double f;
    char *s;
};

static uint8_t title(void)
{
    printf("**********************************************\n"
           "**               SUSIIoT demo               **\n"
           "**********************************************\n");

    printf("0.Exit\n"
           "1.Capability object\n"
           "2.Capability string\n"
           "3.Get data object\n"
           "4.Get data string\n"
           "5.Get data object by URI\n"
           "6.Get data string by URI\n"
           "7.Get data value\n"
           "8.Set Data object\n"
           "9.Set data string\n"
           "10.Set Data value\n"
           "11.Enable logger\n"
           "12.Disable logger\n"
           "13.Get logger path\n"
           "14.Get logger status\n"
           "Enter your choice:");

    return 0;
}

static json_t* getDataJson(uint32_t &id)
{
    int ret = 0;
    json_t *jsonObject = NULL;
    uint32_t type = 0;

    printf("Get Data ID: ");
    ret = SCANF("%d", &id);

    printf("\nType: 1. integer  2. real  3. string\nType: ");
    ret = SCANF("%d", &type);

    switch (type)
    {
        case TYPE_BOOLEAN:
        case TYPE_INTEGER:
            jsonObject = json_integer(0);
            break;
        case TYPE_REAL:
            jsonObject = json_real(0.0);
            break;
        case TYPE_STRING:
            jsonObject = json_string("");
            break;
        default:
            printf("no this type\n");
            break;
    }

    if (ret == 0) {}

    return jsonObject;
}

static json_t* setDataJson(uint32_t &id)
{
    int ret = 0;
    json_t *jsonObject = NULL;
    uint32_t type = 0;
    int setInt = 0;
    double setReal = 0.0;
    char setStr[65536];

    printf("Set Data Id: ");
    ret = SCANF("%d", &id);

    printf("\nType: 1. integer  2. real  3. string\nType: ");
    ret = SCANF("%d", &type);

    printf("\nValue: ");
    switch (type)
    {
        case TYPE_BOOLEAN:
        case TYPE_INTEGER:
            ret = SCANF("%d", &setInt);
            printf("setInt: %d\n", setInt);
            jsonObject = json_integer(setInt);
            break;
        case TYPE_REAL:
            ret = SCANF("%lf", &setReal);
            printf("setReal: %f\n", setReal);
            jsonObject = json_real(setReal);
            break;
        case TYPE_STRING:
            getchar();  // clear scanf input buffer
            ret = SCANF("%[^\n]", setStr);
            printf("setStr: %s\n", setStr);
            jsonObject = json_string((char*)setStr);
            break;
        default:
            printf("no this type\n");
            break;
    }

    if (ret == 0) {}

    return jsonObject;
}

static void testfunction()
{
    int ret = 0;
    const int count = 100000;
    uint32_t id;

    printf("Test SusiIoTGetPFCapability\r\n");
    json_t *susi_iot = json_object();
    for(int i = 0; i < count; i++)
    {
        printf("\r%.2f %%", i * (100 / (float)count));
        if(SusiIoTGetPFCapability(susi_iot) != 0)
        {
            //printf("\nSusiIoTGetPFCapability failed. \n");
        }
    }
    json_object_clear(susi_iot);
    json_decref(susi_iot);
    ret = PAUSE();

    printf("Test SusiIoTGetPFData\r\n");
    json_t *jtmp = json_object();
    for(int i = 0; i < count; i++)
    {
        printf("\r%.2f %%", i * (100 / (float)count));
        if(SusiIoTGetPFData(0, jtmp) != 0)
        {
            //printf("SusiIoTGetPFData failed. \n");
        }
    }
    json_object_clear(jtmp);
    json_decref(jtmp);
    ret = PAUSE();

    printf("Test SusiIoTSetPFData\r\n");
    jtmp = json_object();
    for(int i = 0; i < count; i++)
    {
        printf("\r%.2f %%", i * (100 / (float)count));
        if(SusiIoTGetPFData(0, jtmp) != 0)
        {
            //printf("SusiIoTGetPFData failed. \n");
        }
        if(SusiIoTSetPFData(jtmp) != 0)
        {
            //printf("SusiIoTSetPFData failed. \n");
        }
    }
    json_object_clear(jtmp);
    json_decref(jtmp);
    PAUSE();

    printf("Test SusiIoTGetValue\r\n");
    json_t *jsonGet = getDataJson(id);
    for(int i = 0; i < count; i++)
    {
        printf("\r%.2f %%", i * (100 / (float)count));
        if(SusiIoTGetValue(id, jsonGet) != 0)
        {
            //printf("SusiIoTGetValue failed. \n");
        }
    }
    json_object_clear(jsonGet);
    json_decref(jsonGet);
    ret = PAUSE();

    printf("Test SusiIoTSetValue\r\n");
    json_t *jsonSet = setDataJson(id);
    for(int i = 0; i < count; i++)
    {
        printf("\r%.2f %%", i * (100 / (float)count));
        if(SusiIoTSetValue(131072, jsonSet) != 0)
        {
            //printf("SusiIoTSetValue failed. \n");
        }
    }
    json_object_clear(jsonSet);
    json_decref(jsonSet);
    ret = PAUSE();
    if (ret == 0) {}
}

static SusiIoTStatus_t exec_by_menu()
{
    const char *buffer = NULL;
    int32_t op = OPT_EXIT, id = 0;
    char uri[128];

    for (;;)
    {
        CLRSCR();
        title();
        fflush(stdin);

        SCANF(" %d", &op);

        if ((op < OPT_EXIT || op > OPT_GET_LOGGER_STATUS) && op != OPT_TEST_FUNCTION)
        {
            continue;
        }

        CLRSCR();

        if(op == OPT_EXIT)
        {
            return SUSIIOT_STATUS_SUCCESS;
        }

        if(op == OPT_CAPABILITY_OBJECT)
        {
            json_t *jsonObject = json_object();

            if(SusiIoTGetPFCapability(jsonObject) != 0)
            {
                printf("\nSusiIoTGetPFCapability failed. \n");
            }
            else
            {
                buffer = json_dumps(jsonObject, JSON_INDENT(4) | JSON_PRESERVE_ORDER | JSON_REAL_PRECISION(10));
                printf("%s\n", buffer);
            }

            json_object_clear(jsonObject);
            json_decref(jsonObject);
        }
        else if(op == OPT_CAPABILITY_STRING)
        {
            buffer = SusiIoTGetPFCapabilityString();
            printf("%s\n", buffer);
        }
        else if(op == OPT_GET_DATA_OBJECT)
        {
            json_t *jtmp = json_object();

            printf("id: ");
            SCANF("%d", &id);

            if(SusiIoTGetPFData(id, jtmp) != 0)
            {
                printf("SusiIoTGetPFData failed. \n");
            }
            else
            {
                buffer = json_dumps(jtmp, JSON_INDENT(4) | JSON_PRESERVE_ORDER | JSON_REAL_PRECISION(10));
                printf("%s\n", buffer);
            }

            json_object_clear(jtmp);
            json_decref(jtmp);
        }
        else if(op == OPT_GET_DATA_STRING)
        {
            printf("id: ");
            SCANF("%d", &id);
            buffer = SusiIoTGetPFDataString(id);
            printf("%s\n", buffer);
        }
        else if(op == OPT_GET_DATA_OBJECT_BY_URI)
        {
            json_t *jtmp = json_object();

            printf("uri: ");
            getchar();  // clear scanf input buffer
            SCANF("%[^\n]", uri);

            if(SusiIoTGetPFDataByUri(uri, jtmp) != 0)
            {
                printf("SusiIoTGetPFData failed. \n");
            }
            else
            {
                buffer = json_dumps(jtmp, JSON_INDENT(4) | JSON_PRESERVE_ORDER | JSON_REAL_PRECISION(10));
                printf("%s\n", buffer);
            }

            json_object_clear(jtmp);
            json_decref(jtmp);
        }
        else if(op == OPT_GET_DATA_STRING_BY_URI)
        {
            printf("uri: ");
            getchar();  // clear scanf input buffer
            scanf("%[^\n]", uri);
            buffer = SusiIoTGetPFDataStringByUri(uri);
            printf("%s\n", buffer);
        }
        else if(op == OPT_GET_DATA_VALUE)
        {
            uint32_t id = 0;
            json_t *jsonObject = getDataJson(id);

            if(SusiIoTGetValue(id, jsonObject) != 0)
            {
                printf("SusiIoTGetValue failed. \n");
                   json_object_clear(jsonObject);
                  json_decref(jsonObject);
            }
            else
            {
                json_t * tmp = json_pack("{s:i, s:o}", "Id", id, "v", jsonObject);
                buffer = json_dumps(tmp, JSON_INDENT(4) | JSON_PRESERVE_ORDER | JSON_REAL_PRECISION(10));
                printf("%s\n", buffer);
                json_object_clear(tmp);
                json_decref(tmp);
            }
        }
        else if(op == OPT_SET_DATA_OBJECT)
        {
            json_t *jtmp = json_object();
            printf("id: ");
            SCANF("%d", &id);
            if(SusiIoTGetPFData(id, jtmp) != 0)
            {
                printf("SusiIoTGetPFData failed. \n");
            }
            printf("SusiIoTGetPFData\n");
            PAUSE();
            if(SusiIoTSetPFData(jtmp) != 0)
            {
                printf("SusiIoTSetPFData failed. \n");
            }
            printf("SusiIoTSetPFData\n");
            json_object_clear(jtmp);
            json_decref(jtmp);
        }
        else if(op == OPT_SET_DATA_STRING)
        {
            json_t *jtmp = json_object();
            printf("id: ");
            SCANF("%d", &id);
            if(SusiIoTGetPFData(id, jtmp) != 0)
            {
                printf("SusiIoTGetPFData failed. \n");
            }
            printf("SusiIoTGetPFData\n");
            PAUSE();

            buffer = json_dumps(jtmp, 0);
            printf("%s\n", buffer);
            if(SusiIoTSetPFDataString(buffer) != 0)
            {
                printf("SusiIoTSetPFData failed. \n");
            }
            printf("SusiIoTSetPFData\n");
            json_object_clear(jtmp);
            json_decref(jtmp);
        }
        else if(op == OPT_SET_DATA_VALUE)
        {
            uint32_t id = 0;
            json_t *jsonObject = setDataJson(id);

            if(SusiIoTSetValue(id, jsonObject) != 0)
            {
                printf("SusiIoTSetValue Failed. \n");
            }
            else
            {
                printf("SusiIoTGetPFData Successfully \n");
            }
            json_object_clear(jsonObject);
            json_decref(jsonObject);
        }
        else if(op == OPT_ENABLE_LOGGER)
        {
            SusiIoTLogger(true);
            printf("SusiIoTEnablelogger\n");
        }
        else if(op == OPT_DISABLE_LOGGER)
        {
            SusiIoTLogger(false);
            printf("SusiIoTDisablelogger\n");
        }
        else if(op == OPT_GET_LOGGER_PATH)
        {
            buffer = SusiIoTGetLoggerPath();
            printf("SusiIoT Logger Path:%s\n", buffer);
        }
        else if(op == OPT_GET_LOGGER_STATUS)
        {
            bool logger = SusiIoTGetLoggerStatus();
            printf("SusiIoT Logger Status:%d\n", logger);
        }
        else if(op == OPT_TEST_FUNCTION)
        {
            testfunction();
        }

        if (buffer != NULL)
        {
            SusiIoTMemFree((void *)buffer);
            buffer = NULL;
        }

        PAUSE();
    }
}

// success: SUSIIOT_STATUS_SUCCESS
// error:   SUSIIOT_STATUS_ERROR
static SusiIoTStatus_t exec_by_args(const int argc, char **argv)
{
    const char *buffer = NULL;
    SusiIoTId_t id;
	SusiIoTStatus_t resp = SUSIIOT_STATUS_ERROR;
    uint8_t opt;

    sscanf(argv[0], "%hhu", &opt);

    if(opt == OPT_EXIT)
    {
        resp = SUSIIOT_STATUS_SUCCESS;
    }
    else if ( opt == OPT_CAPABILITY_OBJECT )
    {
        json_t *json_obj = json_object();
        resp = SusiIoTGetPFCapability(json_obj);

        if(resp != SUSIIOT_STATUS_SUCCESS)
        {
            printf("[fail] SusiIoTGetPFCapability()\n");
        }
        else
        {
            buffer = json_dumps(json_obj, JSON_PRESERVE_ORDER | JSON_REAL_PRECISION(10));

            printf("%s\n", buffer);

            SusiIoTMemFree((void *)buffer);
            buffer = NULL;
        }

        json_object_clear(json_obj);
        json_decref(json_obj);
    }
    else if ( opt == OPT_CAPABILITY_STRING )
    {
        buffer = SusiIoTGetPFCapabilityString();

        if ( buffer != NULL )
        {
            printf("%s\n", buffer);

			SusiIoTMemFree((void *)buffer);
            buffer = NULL;

			resp = SUSIIOT_STATUS_SUCCESS;
		}
		else
		{
			printf("[fail] SusiIoTGetPFCapabilityString()\n");
		}
    }
    else if ( opt == OPT_GET_DATA_STRING )
    {
        if ( argc < 2 )  // option, id
        {
            printf("[fail] OPT_GET_DATA_STRING: invalid argument count\n");
        }
        else
        {
            sscanf(argv[1], "%lu", &id);
		    buffer = SusiIoTGetPFDataString(id);

            if ( buffer != NULL )
            {
                printf("%s\n", buffer);

                SusiIoTMemFree((void *)buffer);
                buffer = NULL;

                resp = SUSIIOT_STATUS_SUCCESS;
            }
            else
            {
                printf("[fail] SusiIoTGetPFDataString(0x%x)\n", id);
            }
        }
    }
    else if ( opt == OPT_SET_DATA_VALUE )
    {
        uint8_t obj_type;
        union JSON_OBJ_VALUE obj_value;
        json_t *json_obj = NULL;

        if ( argc < 4 )  // option, id, obj_type, obj_value
        {
            printf("[fail] OPT_SET_DATA_VALUE: invalid argument count\n");
        }
        else
        {
            sscanf(argv[1], "%lu", &id);
            sscanf(argv[2], "%hhu", &obj_type);

            switch (obj_type)
            {
                case TYPE_BOOLEAN:
                case TYPE_INTEGER:
                    sscanf(argv[3], "%d", &obj_value.i);
                    json_obj = json_integer(obj_value.i);
                    break;

                case TYPE_REAL:
                    sscanf(argv[3], "%lf", &obj_value.f);
                    json_obj = json_real(obj_value.f);
                    break;

                case TYPE_STRING:
                    obj_value.s = argv[3];
                    json_obj = json_string(obj_value.s);
                    break;
            }

            if ( !json_obj )
            {
                printf("[fail] unsupport json object: type=%u, value=%s\n", obj_type, argv[3]);
            }
            else
            {
                resp = SusiIoTSetValue(id, json_obj);

                if ( resp != SUSIIOT_STATUS_SUCCESS )
                {
                    printf("[fail] SusiIoTSetValue(0x%x, %s)\n", id, argv[3]);
                }
                else
                {
                    json_object_clear(json_obj);
                    json_decref(json_obj);
                }
            }
        }
    }
    else
    {
        printf("[fail] unsupport menu option: %u\n", opt);
    }

	printf("ExitCode=%lu\n", resp);
	fflush(stdout);
    return resp;
}

void SUSI_IOT_API EventCallBack(SusiIoTId_t id, char *jsonstr)
{
    printf("EventCallBack\nId:0x%d\nData:%s\n", id, jsonstr);
}

int main(int argc, char **argv)
{
    SusiIoTStatus_t status;

    status = SusiIoTInitialize();

    if(status != SUSIIOT_STATUS_SUCCESS)
    {
        printf("SusiIoTInitialize() failed.");
        return status;
    }

    SusiIoTSetPFEventHandler(EventCallBack);

    if ( argc > 1 )
    {
        status = exec_by_args(argc - 1, argv + 1);
    }
    else
    {
        status = exec_by_menu();
    }

    SusiIoTUninitialize();

    return status;
}
