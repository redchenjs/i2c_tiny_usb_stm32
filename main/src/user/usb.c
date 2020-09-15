/*
 * usb.c
 *
 *  Created on: 2020-03-24 20:52
 *      Author: Jack Chen <redchenjs@live.com>
 */

#include "stm32f1xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"

#include "tusb.h"
#include "core/os.h"

#define TAG "usb"

#define _STRING(X)      #X
#define STRING(X)       _STRING(X)

#define SERIAL_NUM_LEN  12
#define SERIAL_NUM_FMT  "%0"STRING(SERIAL_NUM_LEN)"lu"

static tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,

    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD,

    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0x1c40,
    .idProduct          = 0x0534,
    .bcdDevice          = 0x0205,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01,
};

enum itf_num {
    ITF_NUM_VENDOR = 0,

    ITF_NUM_CDC,
    ITF_NUM_CDC_DATA,

    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_VENDOR_DESC_LEN + TUD_CDC_DESC_LEN)

static uint8_t const desc_configuration[] = {
    // Interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    // Interface number, string index, EP Out & IN address, EP size
    TUD_VENDOR_DESCRIPTOR(ITF_NUM_VENDOR, 4, 0x01, 0x81, CFG_TUD_ENDPOINT0_SIZE),
    // Interface number, string index, EP notification address and size, EP data address (out, in) and size.
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 5, 0x83, 8, 0x02, 0x82, CFG_TUD_ENDPOINT0_SIZE),
};

static char const *desc_string_arr[] = {
    (const char[]){0x09, 0x04},         // 0: Language, English (0x0409)
    "EZPrototypes",                     // 1: Manufacturer
    "Generic USB to I2C Adapter",       // 2: Product
    (char[SERIAL_NUM_LEN+1]){0x00},     // 3: Serials, should use chip ID
    "USB to I2C Interface",             // 4: Interface 0
    "USB to UART Interface",            // 5: Interface 1
};

static uint16_t desc_string[32] = {0};

/* Invoked when received GET DEVICE DESCRIPTOR
 * Application return pointer to descriptor
 */
uint8_t const *tud_descriptor_device_cb(void)
{
    return (uint8_t const *)&desc_device;
}

/* Invoked when received GET CONFIGURATION DESCRIPTOR
 * Application return pointer to descriptor
 * Descriptor contents must exist long enough for transfer to complete
 */
uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index;
    return desc_configuration;
}

/* Invoked when received GET STRING DESCRIPTOR request
 * Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
 */
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void)langid;
    uint8_t chr_count = 0;

    if (index == 0) {
        memcpy(&desc_string[1], desc_string_arr[0], 2);

        chr_count = 1;
    } else {
        // convert ASCII string into UTF-16
        if (!(index < sizeof(desc_string_arr)/sizeof(desc_string_arr[0]))) {
            return NULL;
        }

        const char *str = desc_string_arr[index];

        // cap at max char
        chr_count = strlen(str);
        if (chr_count > 31) {
            chr_count = 31;
        }

        for (uint8_t i=0; i<chr_count; i++) {
            desc_string[1+i] = str[i];
        }
    }

    // first byte is length (including header), second byte is string type
    desc_string[0] = (TUSB_DESC_STRING << 8 ) | (2 * chr_count + 2);

    return desc_string;
}

void usb_task(void *pvParameter)
{
    (void)pvParameter;

    // unique device ID
    snprintf((char *)desc_string_arr[3], SERIAL_NUM_LEN + 1, SERIAL_NUM_FMT, *(uint32_t *)0x1FFFF7E8);

    tusb_init();

    OS_LOGI(TAG, "started.");

    while (1) {
        tud_task();
    }
}

void usb_init(void)
{
    xTaskCreate(usb_task, "usbT", 256, NULL, configMAX_PRIORITIES - 3, NULL);
}
