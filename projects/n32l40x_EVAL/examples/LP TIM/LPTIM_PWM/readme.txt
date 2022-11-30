1、功能说明
    1、LPTIM 输出PWM信号
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.26.2.0
    硬件环境：      基于N32L43XM-STB开发
3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=64M,AHB=64M,APB1=16M,APB2=32M,LPTIM CLK=LSI
        2、端口配置：
            PC1选择为LPTIM输出
        3、LPTIM：
            LPTIM 4分频LSI，输出PWM信号
    使用方法：
        1、编译后打开调试模式，可观察到PC1引脚PWM信号
4、注意事项
    无