1、功能说明
    1、LPTIM 定时产生EXTI24，唤醒CPU进行IO翻转
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.26.2.0
    硬件环境：      基于N32L43XM-STB开发
3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=64M,AHB=64M,APB1=16M,APB2=32M,LPTIM CLK=LSI
        2、中断：
            LPTIM周期触发EXTI24中断，唤醒CPU
        3、端口配置：
            PB7选择为IO输出
        4、LPTIM：
            LPTIM连续定时模式
    使用方法：
        1、编译后打开调试模式，可观察到PB7引脚出产生周期翻转，且CPU不断进入低功耗模式
4、注意事项
    无