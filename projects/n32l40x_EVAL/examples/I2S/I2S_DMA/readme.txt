1、功能说明

	1、I2S 通过 DMA 收发数据


2、使用环境

	/* 硬件环境：工程对应的开发硬件平台 */
	开发板：N32L43XM-STB N32L40XM-STB V1.1

3、使用说明
	
	/* 描述相关模块配置方法；例如:时钟，I/O等 */
	1、SystemClock：64MHz
	2、GPIO：I2S1（MASTER SPI1 WS--PA4、SCK--PA5、MOSI--PA7），
			 I2S2（SLAVE  SPI2 WS--PB12、SCK--PB13、 MOSI--PB15）,
	3、DMA： I2S1 接收采用 DMA_CH2 通道，I2S2 发送采用 DMA_CH4 通道；


	/* 描述Demo的测试步骤和现象 */
        1.编译后下载程序复位运行；
        2.I2S1 通过 DMA_CH2 发送数据，I2S2 通过 DMA_CH4 接收数据 ，运行完成后，查看 TransferStatus 状态为 PASSED

4、注意事项
	无