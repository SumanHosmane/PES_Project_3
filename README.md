# PES_Project_3
##KL25Z ADC and Uart

--- Implementing a system to send continuos stream of data to the ADC (channel 0) of the KL25Z processor.
--- Data will be sampled at 12MHz with a 16-bit resolution. Sampled data will be stored in a double buffer through the DMA so that processor is not overwhelmed by the rate of data that is flowing in.
--- Captured data in the buffer will be echoed to a terminal through the UART.
