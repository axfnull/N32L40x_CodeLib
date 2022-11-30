1、功能说明
    1、通过RTC闹钟来唤醒SLEEP。
    2、串口打印进入退出状态

2、使用环境

    软件开发环境：KEIL MDK-ARM V5.25

    硬件环境：
        1、基于评估板N32L40XRL-STB V1.0开发
        2、MCU：N32L406RBL7


3、使用说明
    
    系统配置；
        1、RTC时钟源：LSI
        2、低功耗模式：SLEEP
        3、串口配置：
                    - 串口为USART2（TX：PA9  RX：PA10）:
                    - 数据位：8
                    - 停止位：1
                    - 奇偶校验：无


    使用方法：
        在KEIL下编译后烧录到评估板，通电，过了一会串口打印start low power，说明进入低功耗模式了。
        当串口打印输出Exit low power，说明此时闹钟唤醒SLEEP了。


4、注意事项
    无