/*****************************************************************************
 * Copyright (c) 2019, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file main.c
 * @author Nations
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */
#include "n32l40x.h"
#include "n32l40x_i2c.h"
#include "main.h"
#include "log.h"

/** @addtogroup N32L40X_StdPeriph_Examples
 * @{
 */

/** @addtogroup I2C_10bit
 * @{
 */

#define TEST_BUFFER_SIZE  100
#define I2CT_FLAG_TIMEOUT ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))
#define I2C_MASTER_ADDR   0x230
#define I2C_SLAVE_ADDR    0x2A0
#define I2C_FRAME_HEAD_TX 0xF0
#define I2C_FRAME_HEAD_RX 0xF1

#define I2Cx I2C1
#define I2Cx_SCL_PIN GPIO_PIN_8
#define I2Cx_SDA_PIN GPIO_PIN_9
#define GPIOx        GPIOB

static __IO uint32_t I2CTimeout = I2CT_LONG_TIMEOUT;

static uint8_t master_tx_buf[TEST_BUFFER_SIZE] = {0};
static uint8_t master_rx_buf[TEST_BUFFER_SIZE] = {0};
static uint8_t slave_tx_buf[TEST_BUFFER_SIZE] = {0};
static uint8_t slave_rx_buf[TEST_BUFFER_SIZE] = {0};
static CommCtrl_t Comm_Flag = C_READY;
static uint8_t RCC_RESET_Flag = 0;


static uint8_t rx_num = 0;
static uint8_t tx_num = 0;
volatile Status test_status = FAILED;
static uint8_t flag_master_complete = 0;
static TR_STA flag_master_dir = TRANSMIT;
static uint8_t flag_slave_complete = 0;
static TR_STA master_TR = TRANSMIT;

Status Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void Memset(void* s, uint8_t c, uint32_t count);
void CommTimeOut_CallBack(ErrCode_t errcode);

void Delay(uint32_t nCount)
{
    uint32_t tcnt;
    
    while(nCount--)
    {
        tcnt = 64000 / 5;
        while (tcnt--){;}
    }
}

void Delay_us(uint32_t nCount)
{
    uint32_t tcnt;
    
    while (nCount--)
    {
        tcnt = 64 / 5;
        while (tcnt--){;}
    }
}

/**
 * @brief  i2c Interrupt configuration
 * @param ch I2C channel
 */
void NVIC_Configuration(uint8_t ch)
{
    NVIC_InitType NVIC_InitStructure;
    if (ch == 1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    }
    if (ch == 2)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
    }

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    if (ch == 1)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn; // test err
    }
    if (ch == 2)
    {
        NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn; // test err
    }
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

int i2c_master_init(void)
{
    I2C_InitType i2c1_master;
    GPIO_InitType i2c1_gpio;
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_I2C1, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    
    /*PB8 -- SCL; PB9 -- SDA*/
    GPIO_InitStruct(&i2c1_gpio);
    i2c1_gpio.Pin               = GPIO_PIN_8 | GPIO_PIN_9;
    i2c1_gpio.GPIO_Slew_Rate    = GPIO_Slew_Rate_High;
    i2c1_gpio.GPIO_Mode         = GPIO_Mode_AF_OD;
    i2c1_gpio.GPIO_Alternate    = GPIO_AF4_I2C1;
    i2c1_gpio.GPIO_Pull         = GPIO_Pull_Up;	  
    GPIO_InitPeripheral(GPIOB, &i2c1_gpio);

    I2C_DeInit(I2C1);
    i2c1_master.BusMode     = I2C_BUSMODE_I2C;
    i2c1_master.FmDutyCycle = I2C_FMDUTYCYCLE_2;
    i2c1_master.OwnAddr1    = I2C_MASTER_ADDR;
    i2c1_master.AckEnable   = I2C_ACKEN;
    i2c1_master.AddrMode    = I2C_ADDR_MODE_10BIT;
    i2c1_master.ClkSpeed    = 100000; //100K

    I2C_Init(I2C1, &i2c1_master);
    // int enable
    I2C_ConfigInt(I2C1, I2C_INT_EVENT | I2C_INT_BUF | I2C_INT_ERR, ENABLE);
    NVIC_Configuration(1);
    I2C_Enable(I2C1, ENABLE);
    return 0;
}

int i2c_slave_init(void)
{
    I2C_InitType i2c2_slave;
    GPIO_InitType i2c2_gpio;
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_I2C2, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);

    /*PB10 -- SCL; PB11 -- SDA*/
    GPIO_InitStruct(&i2c2_gpio);
    i2c2_gpio.Pin               = GPIO_PIN_10 | GPIO_PIN_11;
    i2c2_gpio.GPIO_Slew_Rate    = GPIO_Slew_Rate_High;
    i2c2_gpio.GPIO_Mode         = GPIO_Mode_AF_OD;
    i2c2_gpio.GPIO_Alternate    = GPIO_AF6_I2C2;
    i2c2_gpio.GPIO_Pull         = GPIO_Pull_Up;	  
    GPIO_InitPeripheral(GPIOB, &i2c2_gpio);
    
    I2C_DeInit(I2C2);
    i2c2_slave.BusMode     = I2C_BUSMODE_I2C;
    i2c2_slave.FmDutyCycle = I2C_FMDUTYCYCLE_2;
    i2c2_slave.OwnAddr1    = I2C_SLAVE_ADDR;
    i2c2_slave.AckEnable   = I2C_ACKEN;
    i2c2_slave.AddrMode    = I2C_ADDR_MODE_10BIT;
    i2c2_slave.ClkSpeed    = 100000; //100K

    I2C_Init(I2C2, &i2c2_slave);
    // int enable
    I2C_ConfigInt(I2C2, I2C_INT_EVENT | I2C_INT_BUF | I2C_INT_ERR, ENABLE);
    NVIC_Configuration(2);
    I2C_Enable(I2C2, ENABLE);
    return 0;
}

/**
 * @brief   Main program
 */
int main(void)
{
    uint16_t i = 0;

    log_init();
    log_info("-----this is a i2c(10bit) master/slave demo\r\n");
    /* Initialize the I2C driver ------------------------------------*/
    i2c_master_init();
    i2c_slave_init();
    
    /*----------I2C1 Master write and I2C2 Slave read data-----------*/
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    {
        master_tx_buf[i] = i;
    }
    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    /*Wait until no busy*/
    while (I2C_GetFlag(I2C1, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_BUSY);
        }
    }
    
    /* I2C1 Write data and I2C2 Read data */
    if (Comm_Flag == C_READY)
    {
        Comm_Flag = C_START_BIT;
        I2C_GenerateStart(I2C1, ENABLE);
    }

    I2CTimeout = I2CT_LONG_TIMEOUT * 1000;
    /* Wait write/Read data complete*/
    while((!flag_master_complete) || (!flag_slave_complete))
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_UNKNOW);
        }
    }

    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (I2C_GetFlag(I2C1, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_BUSY);
        }
    }
    Comm_Flag = C_READY;
    
    /* Check if the data written to the memory is read correctly */
    test_status = Buffercmp(master_tx_buf, slave_rx_buf, TEST_BUFFER_SIZE);
    if (test_status == PASSED)
    {
        log_info("i2c(10bit) master->slave test pass!\r\n");
    }
    else
    {
        log_info("i2c(10bit) master->slave test fail!\r\n");
    }
    
    Delay(10);
    /*----------I2C1 Master Read and I2C2 Slave Write data-----------*/
    for (i = 0; i < TEST_BUFFER_SIZE; i++)
    {
        slave_tx_buf[i] = (0x80 + i);
    }
    flag_master_complete = 0;
    flag_slave_complete = 0;
    master_TR = RECEIVE;
    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    /*Wait until no busy*/
    while (I2C_GetFlag(I2C1, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_BUSY);
        }
    }
    
    I2C_ConfigAck(I2C1, ENABLE);
    /* I2C1 Write data and I2C2 Read data */
    if (Comm_Flag == C_READY)
    {
        Comm_Flag = C_START_BIT;
        I2C_GenerateStart(I2C1, ENABLE);
    }

    I2CTimeout = I2CT_LONG_TIMEOUT * 1000;
    /* Wait write/Read data complete*/
    while((!flag_master_complete) || (!flag_slave_complete))
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_UNKNOW);
        }
    }

    I2CTimeout = I2CT_LONG_TIMEOUT;
    while (I2C_GetFlag(I2C1, I2C_FLAG_BUSY))
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(MASTER_BUSY);
        }
    }
    Comm_Flag = C_READY;
    
    /* Check if the data written to the memory is read correctly */
    test_status = Buffercmp(master_rx_buf, slave_tx_buf, TEST_BUFFER_SIZE);
    if (test_status == PASSED)
    {
        log_info("i2c(10bit) master<-slave test pass!\r\n");
    }
    else
    {
        log_info("i2c(10bit) master<-slave test fail!\r\n");
    }
    
    while (1)
    {
    }
}

/**
 * @brief  Compares two buffers.
 * @param  pBuffer, pBuffer1: buffers to be compared.
 * @param BufferLength buffer's length
 * @return PASSED: pBuffer identical to pBuffer1
 *         FAILED: pBuffer differs from pBuffer1
 */
Status Buffercmp(uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength)
{
    while (BufferLength--)
    {
        if (*pBuffer != *pBuffer1)
        {
            return FAILED;
        }

        pBuffer++;
        pBuffer1++;
    }

    return PASSED;
}

/**
 * @brief memery set a value
 * @param s source
 * @param c value
 * @param count number
 * @return pointer of the memery
 */
void Memset(void* s, uint8_t c, uint32_t count)
{
    char* xs = (char*)s;

    while (count--)
    {
        *xs++ = c;
    }

    return;
}

/**
 * @brief  i2c master Interrupt service function
 *
 */
void I2C1_EV_IRQHandler(void)
{
    uint32_t last_event;

    last_event = I2C_GetLastEvent(I2C1);
    if ((last_event & I2C_ROLE_MASTER) == I2C_ROLE_MASTER) // master mode
    {
        switch (last_event)
        {
        case I2C_EVT_MASTER_MODE_FLAG: // 0x00030001.EV5 start
            if (flag_master_dir == TRANSMIT)
            {
                Memset(slave_rx_buf, 0, TEST_BUFFER_SIZE);
                I2C_SendData(I2C1, (I2C_FRAME_HEAD_TX | ((I2C_SLAVE_ADDR & 0x300) >> 7)));
                tx_num = 0;
            }
            else if (flag_master_dir == RECEIVE)
            {
                Memset(master_rx_buf, 0, TEST_BUFFER_SIZE);
                I2C_SendData(I2C1, (I2C_FRAME_HEAD_RX | ((I2C_SLAVE_ADDR & 0x300) >> 7)));
                rx_num = 0;
                flag_master_dir = TRANSMIT;
            }
            break;
        case I2C_EVT_MASTER_MODE_ADDRESS10_FLAG:
            I2C_SendData(I2C1, (I2C_SLAVE_ADDR & 0xFF));
            break;
   
        case I2C_EVT_MASTER_TXMODE_FLAG: // 0x00070082.EV6
            Comm_Flag = C_READY;
            if (master_TR == RECEIVE)
            {
                I2C_GenerateStart(I2C1, ENABLE);
                master_TR = NONE;
                flag_master_dir = RECEIVE;
            }
            else
            {
                I2C_SendData(I2C1, master_tx_buf[tx_num++]);
            }
            break;
        case I2C_EVT_MASTER_DATA_SENDING: // 0x00070080. transmitting data
            if (tx_num < TEST_BUFFER_SIZE)
            {
                I2C_SendData(I2C1, master_tx_buf[tx_num++]);
            }
            break;
        case I2C_EVT_MASTER_DATA_SENDED: // 0x00070084.byte data send finish
                                         // bit2	BSF (Byte transfer finished)
            if (tx_num == TEST_BUFFER_SIZE)
            {
                if (Comm_Flag == C_READY)
                {
                    Comm_Flag = C_STOP_BIT;
                    I2C_GenerateStop(I2C1, ENABLE);
                    flag_master_complete = 1;
                }
            }
            break;
            // MasterReceiver
        case I2C_EVT_MASTER_RXMODE_FLAG: // 0x00030002.EV6
            Comm_Flag = C_READY;
            if (TEST_BUFFER_SIZE == 1)
            {
                I2C_ConfigAck(I2C1, DISABLE);
                if (Comm_Flag == C_READY)
                {
                    Comm_Flag = C_STOP_BIT;
                    I2C_GenerateStop(I2C1, ENABLE);
                }
            }
            else if (TEST_BUFFER_SIZE == 2)
            {
                I2C1->CTRL1 |= I2C_NACK_POS_NEXT; /// set ACKPOS
                I2C_ConfigAck(I2C1, DISABLE);
            }
            break;
        case I2C_EVT_MASTER_DATA_RECVD_FLAG: // one byte recved :EV7.//BUSY,MSMODE and RXDATNE flags.
        case I2C_EVT_MASTER_SFT_DATA_RECVD_FLAG: // BUSY, MSMODE(Master) and Data register not empty, BSF(Byte transfer finished)flags.
            master_rx_buf[rx_num++] = I2C_RecvData(I2C1);
            // before te last data,should config NACK and STOP
            if (rx_num == (TEST_BUFFER_SIZE - 1))
            {
                I2C_ConfigAck(I2C1, DISABLE);   // Disable I2C1 acknowledgement.
                if (Comm_Flag == C_READY)
                {
                    Comm_Flag = C_STOP_BIT;
                    I2C_GenerateStop(I2C1, ENABLE); // Send I2C1 STOP Condition.
                }
            }
            else if (rx_num == TEST_BUFFER_SIZE)
            {
                flag_master_complete = 1;
            }
            break;
        case 0x00030201: // Arbitration lost
        case 0x00030401: // Acknowledge failure
        case 0x00030501: // Acknowledge failure and Bus error
            I2C_Enable(I2C1, DISABLE);
            I2C_Enable(I2C1, ENABLE);
            break;
        default:
            log_info("I2C error status:0x%x\r\n", last_event);
            break;
        }
    }
}

void I2C1_ER_IRQHandler(void)
{
    uint32_t last_event;
    last_event = I2C_GetLastEvent(I2C1);
    if(last_event == I2C_EVT_SLAVE_ACK_MISS)   
    {	
        I2C_ClrFlag(I2C1, I2C_FLAG_ACKFAIL);
    }
}

/**
 * @brief  i2c slave Interrupt service function
 */
void I2C2_EV_IRQHandler(void)
{
    uint8_t timeout_flag = 0;
    uint32_t last_event = 0;

    last_event = I2C_GetLastEvent(I2C2);
    if ((last_event & I2C_ROLE_MASTER) != I2C_ROLE_MASTER) // MSMODE = 0:I2C slave mode
    {
        switch (last_event)
        {
        case I2C_EVT_SLAVE_RECV_ADDR_MATCHED:
            rx_num = 0;
            break;
        case I2C_EVT_SLAVE_SEND_ADDR_MATCHED:
            tx_num = 0;
            I2C2->DAT = slave_tx_buf[tx_num++];
            break;
        case I2C_EVT_SLAVE_DATA_SENDING:
            if (tx_num == TEST_BUFFER_SIZE)
            {
                flag_slave_complete = 1;
            }
            else
            {
                I2C2->DAT = slave_tx_buf[tx_num++];
            }
            break;
        case I2C_EVT_SLAVE_DATA_SENDED:
            I2C2->DAT = slave_tx_buf[tx_num++];
            break;
        case I2C_EVT_SLAVE_DATA_RECVD_NOBUSY:   //No busy when recv the last byte
        case I2C_EVT_SLAVE_DATA_RECVD:
            slave_rx_buf[rx_num++] = I2C2->DAT;
            break;
        case I2C_EVT_SLAVE_STOP_RECVD:
            I2C_Enable(I2C2, ENABLE);
            if((tx_num != 0) || (rx_num != 0))
            {
                flag_slave_complete = 1;
            }
            break;
        default:
            I2C_Enable(I2C2, ENABLE);
            timeout_flag = 1;
            break;
        }
    }
    if (timeout_flag)
    {
        if ((I2CTimeout--) == 0)
        {
            CommTimeOut_CallBack(SLAVE_UNKNOW);
        }
    }
    else
        I2CTimeout = I2CT_LONG_TIMEOUT;
}

void I2C2_ER_IRQHandler(void)
{
    uint32_t last_event;
    last_event = I2C_GetLastEvent(I2C2);
    if(last_event == I2C_EVT_SLAVE_ACK_MISS)   
    {	
        I2C_ClrFlag(I2C2, I2C_FLAG_ACKFAIL);
        if(tx_num != 1)  //slave send the last data and recv NACK 
        {
            flag_slave_complete = 1;
        }
        else //not the last data recv nack, send fail
        {
        }
    }
}

void IIC_RestoreSlaveByClock(void)
{
    uint8_t i;
    GPIO_InitType i2cx_gpio;
    
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    GPIO_AFIOInitDefault();
    GPIO_DeInit(GPIOx);
    
    GPIO_InitStruct(&i2cx_gpio);
    i2cx_gpio.Pin               = I2Cx_SCL_PIN;
    i2cx_gpio.GPIO_Slew_Rate    = GPIO_Slew_Rate_High;
    i2cx_gpio.GPIO_Mode         = GPIO_Mode_Out_PP;
    i2cx_gpio.GPIO_Alternate    = GPIO_NO_AF;
    i2cx_gpio.GPIO_Pull         = GPIO_No_Pull;	  
    GPIO_InitPeripheral(GPIOx, &i2cx_gpio);
    
    for (i = 0; i < 9; i++)
    {
        GPIO_SetBits(GPIOx, I2Cx_SCL_PIN);
        Delay_us(5);
        GPIO_ResetBits(GPIOx, I2Cx_SCL_PIN);
        Delay_us(5);
    }
}
    
void SystemNVICReset(void)
{
    __set_FAULTMASK((uint32_t)1);
    log_info("***** NVIC system reset! *****\r\n");
    __NVIC_SystemReset();
}

void IIC_RCCReset(void)
{
    if (RCC_RESET_Flag >= 3)
    {
        SystemNVICReset();
    }
    else
    {
        RCC_RESET_Flag++;
        
        RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_I2C1, ENABLE);
        RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_I2C1, DISABLE);
        
        RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_I2C1, DISABLE );
        GPIOB->PMODE |= 0x0000000F;
        RCC_EnableAPB2PeriphClk( RCC_APB2_PERIPH_AFIO, DISABLE);
        RCC_EnableAPB2PeriphClk (RCC_APB2_PERIPH_GPIOB, DISABLE );
        
        RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_I2C1, ENABLE);
        RCC_EnableAPB1PeriphReset(RCC_APB1_PERIPH_I2C1, DISABLE);
        
        IIC_RestoreSlaveByClock();
        
        log_info("***** IIC module by RCC reset! *****\r\n");
        i2c_master_init();
    }
}

void IIC_SWReset(void)
{
    GPIO_InitType i2cx_gpio;
    
    GPIO_InitStruct(&i2cx_gpio);
    i2cx_gpio.Pin               = I2Cx_SCL_PIN | I2Cx_SDA_PIN;
    i2cx_gpio.GPIO_Slew_Rate    = GPIO_Slew_Rate_High;
    i2cx_gpio.GPIO_Mode         = GPIO_Mode_Input;
    i2cx_gpio.GPIO_Alternate    = GPIO_NO_AF;
    i2cx_gpio.GPIO_Pull         = GPIO_No_Pull;	  
    GPIO_InitPeripheral(GPIOx, &i2cx_gpio);
    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    for (;;)
    {
        if ((I2Cx_SCL_PIN | I2Cx_SDA_PIN) == (GPIOx->PID & (I2Cx_SCL_PIN | I2Cx_SDA_PIN)))
        {
            I2Cx->CTRL1 |= 0x8000;
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            I2Cx->CTRL1 &= ~0x8000;
            
            log_info("***** IIC module self reset! *****\r\n");
            break;
        }
        else
        {
            if ((I2CTimeout--) == 0)
            {
                IIC_RCCReset();
            }
        }
    }
}

void CommTimeOut_CallBack(ErrCode_t errcode)
{
    log_info("...ErrCode:%d\r\n", errcode);
    
#if (COMM_RECOVER_MODE == MODULE_SELF_RESET)
    IIC_SWReset();
#elif (COMM_RECOVER_MODE == MODULE_RCC_RESET)
    IIC_RCCReset();
#elif (COMM_RECOVER_MODE == SYSTEM_NVIC_RESET)
    SystemNVICReset();
#endif
}

/**
 * @}
 */
