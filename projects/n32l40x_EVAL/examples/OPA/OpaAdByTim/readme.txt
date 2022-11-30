1、功能说明
    1、TIM1 CH4触发ADC注入采样OPA1 OPA2的电压数据，并且TIM1输出受COMP1刹车控制
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.26.2.0
    硬件环境：      基于N32L43XM-STB开发
3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=64M,AHB=64M,APB1=16M,APB2=32M,ADC CLK=64M/10,ADC 1M CLK=HSE/8,OPA CLK=16M,COMP CLK=16M,TIM1 CLK=64M,TIM8 CLK=64M
        2、中断：
            ADC注入中断打开，抢断优先级0，子优先级0
            读取转换结果
        3、端口配置：
            PA4选择为模拟功能OPA1 VP
            PA7选择为模拟功能OPA2 VP
            PA2选择为模拟功能OPA1 OUT
            PA6选择为模拟功能OPA2 OUT
            PA0选择为模拟功能COMP1 INP
            PB5选择为模拟功能COMP1 INM
            PA11选择为模拟功能COMP1 OUT	
            PA8选择为TIM1 CH1输出
            PA9选择为TIM1 CH2输出
            PA10选择为TIM1 CH3输出
            PB13选择为TIM1 CH1N输出
            PB14选择为TIM1 CH2N输出
            PB15选择为TIM1 CH3N输出
            PB12选择为TIM1 Breakin输入
            PA0选择为ADC1_CH1通道输入
            PC0选择为ADC12_CH6通道输入
        4、OPA：
            OPA1 OPA2电压跟随器功能，VM无引脚
        5、ADC：
            ADC注入模式、扫描转换、TIM1 CC4触发、12位数据右对齐，注入转换OPA1和OPA2输出模拟电压数据
        6、COMP：
            COMP1 INP选择PA0，INM选择PB5，输出触发TIM1和TIM8刹车
        7、TIM：
            TIM1 6路互补打开，刹车使能且打开中断，CH4触发输出
    使用方法：
        1、编译后打开调试模式，用示波器或者逻辑分析仪观察TIM1的输出波形和ADC读取的采集数据
        2、改变COMP1 的输出状态可以刹车TIM1，改变OPA1 OPA2的输入电压可以改变ADC读取的数据
4、注意事项
    无