# adi　相关文档链接

[adi 通用 spi 文档链接](https://wiki.analog.com/_media/resources/technical-guides/adispi_rev_1p0_customer.pdf?doc=AD9542.pdf)

[adi adrv9009](https://www.analog.com/cn/products/adrv9009.html#product-documentation)

[adi 9009 wiki](https://wiki.analog.com/resources/tools-software/linux-drivers/iio-transceiver/adrv9009)

[adi 9009 app Talise-API](https://www.analog.com/en/design-center/landing-pages/001/transceiver-evaluation-software.html)

[talise-api release](https://www.analog.com/media/en/evaluation-boards-kits/evaluation-software/Talise_Release_6-0_Customer_%20Release_%20Notes_3-6-0-6.pdf)

```
Linux Kernel Configuration
	Device Drivers  --->
	<*>     Industrial I/O support --->
	    --- Industrial I/O support
	    -*-   Enable ring buffer support within IIO
	    -*-     Industrial I/O lock free software ring
	    -*-   Enable triggered sampling support

	          *** Analog to digital converters ***
	    [--snip--]

		-*- Analog Devices High-Speed AXI ADC driver core
		< > Analog Devices AD9361, AD9364 RF Agile Transceiver driver
		< > Analog Devices AD9371 RF Transceiver driver
		<*> Analog Devices ADRV9009/ADRV9008 RF Transceiver driver
		< > Analog Devices AD6676 Wideband IF Receiver driver
		< > Analog Devices AD9467, AD9680, etc. high speed ADCs
		< > Analog Devices Motor Control (AD-FMCMOTCON) drivers

	    [--snip--]
	    

	Frequency Synthesizers DDS/PLL  --->
    		Direct Digital Synthesis  --->
	 		<*> Analog Devices CoreFPGA AXI DDS driver
		Clock Generator/Distribution  --->	
			< > Analog Devices AD9508 Clock Fanout Buffer                 
			< > Analog Devices AD9523 Low Jitter Clock Generator          
			<*> Analog Devices AD9528 Low Jitter Clock Generator          
			< > Analog Devices AD9548 Network Clock Generator/Synchronizer
			< > Analog Devices AD9517 12-Output Clock Generator  	

	<*>   JESD204 High-Speed Serial Interface Support  --->
		--- JESD204 High-Speed Serial Interface Support  
		< >   Altera Arria10 JESD204 PHY Support         
		<*>   Analog Devices AXI ADXCVR PHY Support      
		< >   Generic AXI JESD204B configuration driver  
		<*>   Analog Devices AXI JESD204B TX Support     
		<*>   Analog Devices AXI JESD204B RX Support  
```
