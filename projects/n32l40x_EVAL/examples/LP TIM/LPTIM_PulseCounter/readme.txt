1、功能说明
    1、LPTIM 计数IN1的上升沿个数
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.26.2.0
    硬件环境：      基于N32L43XM-STB开发
3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=64M,AHB=64M,APB1=16M,APB2=32M,LPTIM CLK=LSI
        2、端口配置：
            PC0选择为LPTIM IN1输入
            PB0选择为IO输出
        3、LPTIM：
            LPTIM外部计数模式，利用内部LSI时钟连续计数IN1上升沿个数
    使用方法：
        1、编译后打开调试模式，连接PB0和PC0，将变量tempCNT添加到watch窗口
        2、程序运行后，PB0输出10个脉冲周期，tempCNT等于10
4、注意事项
    无