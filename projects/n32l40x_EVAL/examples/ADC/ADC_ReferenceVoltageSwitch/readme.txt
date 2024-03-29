1、功能说明
    1、ADC1采样转换PC0 PC1引脚的模拟电压
    2、采用软件触发一次，采集一次的方式
	3、通过按键KEY1(PA4)，切换ADC参考电压为VREF+\VREFBUFF.
2、使用环境
    软件开发环境：  KEIL MDK-ARM V5.26.2.0
    硬件环境：      基于N32L40XML-STB V1.0开发
3、使用说明
    系统配置；
        1、时钟源：
            HSE=8M,PLL=64M,AHB=64M,APB1=16M,APB2=32M,ADC CLK=64M/16,ADC 1M CLK=HSE/8
        2、端口配置：
            PC0选择为模拟功能ADC1转换通道
            PC1选择为模拟功能ADC1转换通道
			PA4选择为按键输入功能，输入上拉模式，低电平有效
        3、ADC：
            ADC软件触发转换、12位数据右对齐，转换PC0 PC1的模拟电压数据
    使用方法：
        1、编译后打开调试模式，将变量ADCConvertedValue添加到watch窗口观察
        2、通过改变PC0 PC1引脚的电压，可以看到转换结果变量同步改变
		3、通过按键切换ADC参考电压，观察转换结果变量；
4、注意事项
    1、当系统采用HSE时钟时（一般HSI也是打开的），RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8)可以配置为HSE或者HSI
    2、当系统采样HSI时钟时（一般HSE是关闭的），RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8)只能配置为HSI
	3、当通过按键将ADC参考电压切换为VREFBUFF时，ADC能测量的电压范围为0 ~ 2.048V.



1. Function description
    1. ADC1 samples and converts the analog voltage of PC0 PC1 pin
    2. The software triggers once and collects once
	3. The ADC reference voltage is switched to VREF+\VREFBUFF by pressing KEY1 (PA4)
2. Use environment
    Software development environment: KEIL MDK-ARM V5.26.2.0
    Hardware environment: Developed based on the development board N32L40XML-STB V1.0
3. Instructions for use
    System Configuration;
        1. Clock source:
            HSE=8M,PLL=64M,AHB=64M,APB1=16M,APB2=32M,ADC CLK=64M/16,ADC 1M CLK=HSE/8
        2. Port configuration:
            PC0 is selected as the analog function, ADC1 conversion channel
            PC1 is selected as the analog function, ADC1 conversion channel
			PA4 selected for key input function, input pull-up mode, active low
        3. ADC:
            ADC software triggers conversion, 12-bit data is right-aligned, and the analog voltage data of PC0 PC1 is converted
    Instructions:
        1. After compiling, open the debug mode and add the variable ADCConvertedValue to the watch window to observe
        2. By changing the voltage of the PC0 PC1 pin, you can see that the conversion result variable changes synchronously
		3. Switch the ADC reference voltage by pressing the button and observe the conversion result variable
4. Matters needing attention
    1.When the system uses the HSE clock (HSI is generally enabled), ), RCC_ConfigAdc1mClk (RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8) can be configured as HSE or HSI
    2.When the system uses the HSI clock(HSE is generally disabled), RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV8) can only be configured as HSI
	3.When the ADC reference voltage is switched to VREFBUFF by pressing the key, the ADC can measure a voltage range of 0 ~ 2.048V.