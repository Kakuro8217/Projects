# 数字电压表
这是一个基于STM32F407ZGT6开发的数字电压表小项目。该项目使用了三重ADC对单通道进行交替采集电压信号。
我在测量电压功能的基础上增加了电压范围检测：低于1V或高于2.5V均会报警，即亮红灯并蜂鸣器响起；在范围内正常检测并亮绿灯。
同时为了便于读取电压，我增加了串口功能，可以在串口助手上看到电压值和文字提示。

This is a simple digital voltmeter mini-project based on the STM32F407ZGT6. The project uses triple ADCs to alternately acquire voltage signals from a single channel. 
On the basis of the voltage measurement function, I added voltage range detection: if the voltage is below 1V or above 2.5V, an alarm will sound, meaning the red light will turn on and the buzzer will ring; if the voltage is within the range, normal detection occurs and the green light turns on. 
Additionally, to make reading the voltage easier, I added a serial port feature that allows you to see the voltage value and text prompts on a serial port assistant.
