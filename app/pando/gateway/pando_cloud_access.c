#include "pando_cloud_access.h"
#include "pando_storage_interface.h"
#include "user_interface.h"
#include "c_types.h"
#include "osapi.h"
#include "mem.h"
#include "espconn.h"
#include "pando_channel.h"
#include "pando_system_time.h"
#include "mqtt/mqtt.h"
#include "gateway_config.h"

#define IP_STR_LEN 16
#define PORT_STR_LEN 8
#define DEVICE_TOKEN_LEN 16

MQTT_Client mqtt_client;

extern uint64_t g_event_sequence;
extern uint64_t g_data_sequence;

static uint8 upgrade_flag = 0;

static access_error_callback error_callback = NULL;

static struct espconn * access_conn = NULL;

static struct pando_buffer * recv_buffer = NULL;

static uint64_t current_command_sequence = 0;

uint8 pando_device_token[DEVICE_TOKEN_LEN];

static bool file_download_flag = 0;

static struct download_file_info* download_info = NULL;

static struct sub_device_buffer *file_command_buffer = NULL;

static uint8 str_device_id_hex[17];

static void ICACHE_FLASH_ATTR
access_error_exit(PANDO_ACCESS_ERROR error)
{
	PRINTF("access exit.\n");

	PRINTF("after access: available heap size:%d\n", system_get_free_heap_size());
	if(1== upgrade_flag)
	{
		return;
	}

	if(error_callback != NULL)
	{
		error_callback(error);
	}

	PRINTF("after access: available heap size:%d\n", system_get_free_heap_size());
}



static int ICACHE_FLASH_ATTR
conv_addr_str(const char * ip_str, uint8 * str_ip_addr, int * port)
{
    ip_addr_t ip_addr;
    char * colon = NULL;
    if ((colon = strchr(ip_str, ':')) == NULL)
    {
        return -1;
    }
    // ip address
    size_t ip_len = colon - ip_str;
    os_strncpy(str_ip_addr, ip_str, ip_len);
    str_ip_addr[ip_len] = '\0';
    //ip->addr = ipaddr_addr(str_ip_addr);

    // port
    char str_port_addr[PORT_STR_LEN];
    os_strncpy(str_port_addr, colon+1, PORT_STR_LEN);
    *port = atoi(str_port_addr);

    PRINTF("pando cloud server ip %s, port %d\n", str_ip_addr, *port);

    return 0;
}

static void ICACHE_FLASH_ATTR
init_gateway_info()
{

}

static void ICACHE_FLASH_ATTR
pando_publish_data(uint8* buffer, uint16 length)
{

}




void ICACHE_FLASH_ATTR
decode_command(struct pando_buffer *buffer, uint8 p_type)
{

}


static void ICACHE_FLASH_ATTR
mqtt_data_cb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
    PRINTF("mqtt total data length: %d\n", data_len);
    PRINTF("mqtt topic length: %d\n", topic_len);

}


static void ICACHE_FLASH_ATTR
mqtt_published_cb(uint32_t *arg)
{
	PRINTF("MQTT: Published\r\n");
	MQTT_Client* client = (MQTT_Client*)arg;

}

static void ICACHE_FLASH_ATTR mqtt_connect_cb(uint32_t* arg)
{
	PRINTF("MQTT: Connected\r\n");
    MQTT_Client* client = (MQTT_Client*)arg;


}

static void ICACHE_FLASH_ATTR mqtt_disconnect_cb(uint32_t* arg)
{
	PRINTF("MQTT: Disconnected\r\n");
	MQTT_Client* client = (MQTT_Client*)arg;
}


/******************************************************************************
* FunctionName : pando_cloud_access
* Description  : pando cloud device access api.
* Parameters   : access_error_callback callback:access callback function
* Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR
pando_cloud_access(access_error_callback callback)
{
    PRINTF("PANDO begin access cloud...\n");

    if(error_callback != NULL)
    {
        error_callback = callback;
    }

    char* access_addr = pando_data_get(DATANAME_ACCESS_ADDR);
    if( NULL == access_addr )
    {
        PRINTF("no access server address found...\n");
        return access_error_exit(ERR_OTHER);
    }

    int port;
    uint8 ip_string[16];
    if(0 != (conv_addr_str(access_addr, ip_string, &port)))
    {
        PRINTF("wrong access server address...\n");
        return access_error_exit(ERR_OTHER);
    }

    char* str_device_id = pando_data_get(DATANAME_DEVICE_ID);
    int device_id = atol(str_device_id); // TODO: device id is 64bit, atol not support.
    os_sprintf(str_device_id_hex, "%x", str_device_id_hex);

    MQTT_InitConnection(&mqtt_client, ip_string, port, 0);
    MQTT_InitClient(&mqtt_client, str_device_id_hex, "", "", 30, 1);
    MQTT_OnConnected(&mqtt_client, mqtt_connect_cb);
    MQTT_OnDisconnected(&mqtt_client, mqtt_disconnect_cb);
    MQTT_OnPublished(&mqtt_client, mqtt_published_cb);
    MQTT_OnData(&mqtt_client, mqtt_data_cb);
    MQTT_Connect(&mqtt_client);
    // to consider: reconnect.
}
