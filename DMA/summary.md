## 1 Description
DMA is used to data transaction bewtween RAM and peripherials. 
There are 12 channels in total in support of "Memory to Memory", "P to P" and "P to M" data transmission in STM32F103Z.

DMA applies on the scenario that CPU needs to handle volume of data which is able to release the computation ability.


### 1.1 4 priority level are implemented.
- very high
- High
- medium
- Low

### 1.2 Register Setting
- DMA_CPARx   It is used to config the peripherial data source (设置数据源地址)
- DMA_CMARx   This is used to set the destination memory address(设置数据目的地址)
- DMA_CCRx    It is used to implemented the data trasmission direction.
- DMA_CNDTRx  
### 1.3 UART and DMA
