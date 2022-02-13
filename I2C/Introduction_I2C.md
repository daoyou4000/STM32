## I2C

In this file, the basic knowledge of I2C is demonstrated.
### Background introduction

[参考链接](https://zhuanlan.zhihu.com/p/31086959)
- SCL is clock, SDA is data bus.
- Start condition: SCL high， SDA high-to-low
- End condition:  SCL high， SDA low-to-high
- 发送顺序: 起始 -> 设备地址(0-6th bits) -> 方向位 (7th bit 表示 读1 写0) -> 应答位(8th ACK)
- Master - Slave mode.
