/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: uart.c
 *
 * Description: Two UART mode configration and interrupt handler.
 *              Check your hardware connection while use this mode.
 *
 * Modification history:
 *     2014/3/12, v1.0 create this file.
*******************************************************************************/
#include "ets_sys.h"
#include "osapi.h"
#include "uart.h"
//#include "../binary-protocol-tool/platform_functions.h"
//#include "../binary-protocol-tool/sub_device_protocol_tool.h"
#include "user_interface.h"
//#include "pando/pando_channel.h"
//#include "user/device_config.h"

#define UART0   0
#define UART1   1

// UartDev is defined and initialized in rom code.
extern UartDevice UartDev;

LOCAL void uart0_rx_intr_handler(void *para);

/******************************************************************************
 * FunctionName : uart_config
 * Description  : Internal used function
 *                UART0 used for data TX/RX, RX buffer size is 0x100, interrupt enabled
 *                UART1 just used for debug output
 * Parameters   : uart_no, use UART0 or UART1 defined ahead
 * Returns      : NONE
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
uart_config(uint8 uart_no)
{
    if (uart_no == UART1)
    {
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_U1TXD_BK);
    }
    else
    {
        /* rcv_buff size if 0x100 */
        ETS_UART_INTR_ATTACH(uart0_rx_intr_handler,  &(UartDev.rcv_buff));
        PIN_PULLUP_DIS(PERIPHS_IO_MUX_U0TXD_U);
        PIN_FUNC_SELECT(PERIPHS_IO_MUX_U0TXD_U, FUNC_U0TXD);
    }

    uart_div_modify(uart_no, UART_CLK_FREQ / (UartDev.baut_rate));

    WRITE_PERI_REG(UART_CONF0(uart_no),    UartDev.exist_parity
                   | UartDev.parity
                   | (UartDev.stop_bits << UART_STOP_BIT_NUM_S)
                   | (UartDev.data_bits << UART_BIT_NUM_S));


    //clear rx and tx fifo,not ready
    SET_PERI_REG_MASK(UART_CONF0(uart_no), UART_RXFIFO_RST | UART_TXFIFO_RST);
    CLEAR_PERI_REG_MASK(UART_CONF0(uart_no), UART_RXFIFO_RST | UART_TXFIFO_RST);

    //set rx fifo trigger
    WRITE_PERI_REG(UART_CONF1(uart_no), (UartDev.rcv_buff.TrigLvl & UART_RXFIFO_FULL_THRHD) << UART_RXFIFO_FULL_THRHD_S);

    //clear all interrupt
    WRITE_PERI_REG(UART_INT_CLR(uart_no), 0xffff);
    //enable rx_interrupt
    SET_PERI_REG_MASK(UART_INT_ENA(uart_no), UART_RXFIFO_FULL_INT_ENA);
}

/******************************************************************************
 * FunctionName : uart0_tx_one_char
 * Description  : Internal used function
 *                Use uart0 interface to transfer one char
 * Parameters   : uint8 TxChar - character to tx
 * Returns      : OK
*******************************************************************************/
LOCAL STATUS
uart0_tx_one_char(uint8 TxChar)
{
    while (true)
    {
      uint32 fifo_cnt = READ_PERI_REG(UART_STATUS(UART0)) & (UART_TXFIFO_CNT<<UART_TXFIFO_CNT_S);
      if ((fifo_cnt >> UART_TXFIFO_CNT_S & UART_TXFIFO_CNT) < 126) {
        break;
      }
    }

    WRITE_PERI_REG(UART_FIFO(UART0) , TxChar);
    return OK;
}

/******************************************************************************
 * FunctionName : uart0_rx_one_char
 * Description  : Internal used function
 *                Use uart0 interface to read one char
 * Parameters   : uint8 TxChar - character to tx
 * Returns      : OK
*******************************************************************************/
LOCAL uint8
uart0_rx_one_char(uint8* RxChar)
{
	if(READ_PERI_REG(UART_STATUS(UART0)) & (UART_RXFIFO_CNT << UART_RXFIFO_CNT_S))
	{
		*RxChar = READ_PERI_REG(UART_FIFO(UART0));
		return 1;
	}
	else
	{
		return 0;
	}
}

/******************************************************************************
 * FunctionName : uart0_rx_buffer
 * Description  : use uart0 to receive buffer
 * Parameters   : uint8 *buf - point to send buffer
 *                uint16 len - buffer len
 * Returns      : the bytes readed
*******************************************************************************/
uint16 ICACHE_FLASH_ATTR
uart0_rx_buffer(uint8 *buf, uint16 len)
{
	 uint16_t left = len;
	 uint16_t readed = 0;
	 while (left)
	 {
		 readed += uart0_rx_one_char(buf +readed);
	     left = len - readed;
	 }
	 return readed;
}

/******************************************************************************
 * FunctionName : uart1_tx_one_char
 * Description  : Internal used function
 *                Use uart1 interface to transfer one char
 * Parameters   : uint8 TxChar - character to tx
 * Returns      : OK
*******************************************************************************/
LOCAL STATUS ICACHE_FLASH_ATTR
uart1_tx_one_char(uint8 TxChar)
{
    while (true)
	{
		uint32 fifo_cnt = READ_PERI_REG(UART_STATUS(UART1)) & (UART_TXFIFO_CNT<<UART_TXFIFO_CNT_S);
		if ((fifo_cnt >> UART_TXFIFO_CNT_S & UART_TXFIFO_CNT) < 126) {
			break;
		}
	}

	WRITE_PERI_REG(UART_FIFO(UART1) , TxChar);
	return OK;
}

/******************************************************************************
 * FunctionName : uart1_write_char
 * Description  : Internal used function
 *                Do some special deal while tx char is '\r' or '\n'
 * Parameters   : char c - character to tx
 * Returns      : NONE
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
uart0_write_char(char c)
{
    if (c == '\n') {
        uart0_tx_one_char('\r');
        uart0_tx_one_char('\n');
    } else if (c == '\r') {
    } else {
        uart0_tx_one_char(c);
    }
}

/******************************************************************************
 * FunctionName : uart1_write_char
 * Description  : Internal used function
 *                Do some special deal while tx char is '\r' or '\n'
 * Parameters   : char c - character to tx
 * Returns      : NONE
*******************************************************************************/
LOCAL void ICACHE_FLASH_ATTR
uart1_write_char(char c)
{
    if (c == '\n') {
        uart1_tx_one_char('\r');
        uart1_tx_one_char('\n');
    } else if (c == '\r') {
    } else {
        uart1_tx_one_char(c);
    }
}

/******************************************************************************
 * FunctionName : uart0_rx_intr_handler
 * Description  : Internal used function
 *                UART0 interrupt handler, add self handle code inside
 * Parameters   : void *para - point to ETS_UART_INTR_ATTACH's arg
 * Returns      : NONE
*******************************************************************************/
LOCAL void
uart0_rx_intr_handler(void *para)
{
    /* uart0 and uart1 intr combine togther, when interrupt occur, see reg 0x3ff20020, bit2, bit0 represents
     * uart1 and uart0 respectively
     */
  /*  RcvMsgBuff *pRxBuff = (RcvMsgBuff *)para;
    uint8 RcvChar;

    if (UART_RXFIFO_FULL_INT_ST != (READ_PERI_REG(UART_INT_ST(UART0)) & UART_RXFIFO_FULL_INT_ST))
    {
        return;
    }

    WRITE_PERI_REG(UART_INT_CLR(UART0), UART_RXFIFO_FULL_INT_CLR);
    //34 11 00 03 00 10 CC DD 00 00 00 02 00 00 00 02 00 01 00 01 00 07 00 04 01 00 00 00
    while (uart0_rx_one_char(&RcvChar))
    {
    	if (RcvChar == MAGIC_HEAD_SUB_DEVICE)
        {
    		struct device_header head;
    		struct sub_device_buffer sub_device_buf;
    		int payload_length = 1;
    		uint16_t bytes_readed = 1;
    		head.magic = MAGIC_HEAD_SUB_DEVICE;
    		//读取包头剩余部分
    		bytes_readed += uart0_rx_buffer((char *)&head + sizeof(head.magic), sizeof(head) - sizeof(head.magic));
    		PRINTF("bytes_readed:%d\n", bytes_readed);
    		if(bytes_readed < sizeof(head))
    		{
    			PRINTF("not enough bytes for package head");
    		    return;
    		}

    		payload_length = net16_to_host(head.payload_len);
    		sub_device_buf.buffer_length = payload_length + sizeof(head);
    		sub_device_buf.buffer = (uint8*)pd_malloc(sub_device_buf.buffer_length);
    		pd_memcpy(sub_device_buf.buffer, &head, sizeof(head));
    	    bytes_readed += uart0_rx_buffer((char *)sub_device_buf.buffer +  sizeof(head), payload_length);
    		if(bytes_readed < sub_device_buf.buffer_length)
    		{
    			PRINTF("not enough bytes");
    		    return;
    		}

    		show_package(sub_device_buf.buffer, sub_device_buf.buffer_length);
    		channel_send_to_device(PANDO_CHANNEL_PORT_1, sub_device_buf.buffer, sub_device_buf.buffer_length);
    		pd_free(sub_device_buf.buffer);

        }
    }*/

   /* while (READ_PERI_REG(UART_STATUS(UART0)) & (UART_RXFIFO_CNT << UART_RXFIFO_CNT_S)) {
        RcvChar = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;

        /* you can add your handle code below.*/
      /*  struct sub_device_buffer* data_buffer;

            channel_send_to_device(PANDO_CHANNEL_PORT_1, data_buffer->buffer, data_buffer->buffer_length);
            show_package(data_buffer->buffer, data_buffer->buffer_length);

            delete_device_package(data_buffer);
        *(pRxBuff->pWritePos) = RcvChar;*/

        // insert here for get one command line from uart
       /* if (RcvChar == '\r') {
            pRxBuff->BuffState = WRITE_OVER
        }*/

       /* pRxBuff->pWritePos++;

        if (pRxBuff->pWritePos == (pRxBuff->pRcvMsgBuff + RX_BUFF_SIZE)) {
            // overflow ...we may need more error handle here.
            pRxBuff->pWritePos = pRxBuff->pRcvMsgBuff ;
        }
    }*/

}

/******************************************************************************
 * FunctionName : uart0_tx_buffer
 * Description  : use uart0 to transfer buffer
 * Parameters   : uint8 *buf - point to send buffer
 *                uint16 len - buffer len
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR
uart0_tx_buffer(uint8 *buf, uint16 len)
{
	uint16 i;

    for (i = 0; i < len; i++) {
        uart0_tx_one_char(buf[i]);
    }
}



/******************************************************************************
 * FunctionName : uart_init
 * Description  : user interface for init uart
 * Parameters   : UartBautRate uart0_br - uart0 bautrate
 *                UartBautRate uart1_br - uart1 bautrate
 * Returns      : NONE
*******************************************************************************/
void ICACHE_FLASH_ATTR
uart_init(UartBautRate uart0_br, UartBautRate uart1_br)
{
    // rom use 74880 baut_rate, here reinitialize
    UartDev.baut_rate = uart0_br;
    uart_config(UART0);
    UartDev.baut_rate = uart1_br;
    uart_config(UART1);
    ETS_UART_INTR_ENABLE();

    // install uart1 putc callback
#ifdef SUBDEVICE_INNER
    // esp8266 driver the object, uart0 used to print log.
    os_install_putc1((void *)uart0_write_char);
#else
    // esp8266 used as only gateway, uart0 used to send message to other mcu, and uart1 used to print log.
    os_install_putc1((void *)uart1_write_char);
#endif
    
}

