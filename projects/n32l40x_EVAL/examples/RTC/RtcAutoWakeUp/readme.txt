1、功能说明
    1、通过设定唤醒时间触发中断。
        2、通过唤醒标志位来配置IO输出


2、使用环境

    软件开发环境：KEIL MDK-ARM V5.25

        硬件环境：
        1、基于评估板N32L40XRL-STB V1.0开发
        2、MCU：N32L406RBL7


3、使用说明
    
    系统配置；
        1、周期性唤醒时钟源：RTCCLK（1HZ）
        2、唤醒输出口：PC13
        3、串口配置：
        3、串口配置：
                    - 串口为USART2（TX：PA9  RX：PA10）:
                    - 数据位：8
                    - 停止位：1
                    - 奇偶校验：无
                    - 波特率： 115200 

    使用方法：
        1、在KEIL下编译后烧录到评估板，上电后，串口每隔5s会打印:Stop2 Wakeup From RTC
        2、设定#define  RTC_ALARM_TEST_TYPE  RTC_ALARM_TEST_TYPE_OUTPUT，编译后烧录到评估板，
           上电后，PC13每隔5S翻转一次(4s高电平，1s低电平)。
                


4、注意事项
    无