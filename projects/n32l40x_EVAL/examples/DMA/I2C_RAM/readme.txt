1、功能说明

    此例程提供了一种DMA用法，用于在外设与RAM之间传输数据。

2、使用环境

	软件开发环境：
        IDE工具：KEIL MDK-ARM 5.26
    
    硬件环境：
        开发板 N32L43XM-STB

3、使用说明
	
    1、时钟源：HSE+PLL
    2、主时钟：64MHz
    3、DMA通道：DMA_CH5，DMA_CH6
    4、I2C1 配置：
            SCL   -->  PB8
            SDA   -->  PB9
            ADDR：0x30(7bit)
            CLOCK：100K
    
    5、I2C2 配置：
            SCL   -->  PB10
            SDA   -->  PB11
            ADDR：0xA0(7bit)
            CLOCK：100K
    
    6、USART1配置：
            TX  -->  PA9
            波特率：115200
            数据位：8bit
            停止位：1bit
            无校验
            
    7、测试步骤与现象
	a，连接PB8到PB10，连接PB9到PB11
        b，编译下载代码复位运行
        c，从串口看打印信息，验证结果

4、注意事项
    在SDA，SCL上需要加上拉电阻