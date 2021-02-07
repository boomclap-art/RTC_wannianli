# 介绍
stm32f103 RTC万年历工程.

## 烧录介绍

推荐使用野火的stm32板子(指南者或者霸道进行烧录).

为了使汉字能在OLED正常显示,需要烧录新的字库文件GB2312_H1616.FON.该字库文件适用于OLED页写入的方式(与野火自带的字库文件有所不同,取模方式不一样).需将工程文件下的srcdata文件夹放到sd卡的根目录,再使用野火的官方例程  ```刷外部FLASH程序（如何恢复出厂内容）``` 进行烧录. 

## 按键操作介绍

为了满足时钟的小巧设计,使用4个按键进行控制.分别为next, ok, back 和串口校准触发按键.

 next用于选择下一个功能项, ok键表示确定选择, back返回上一页.

## 功能介绍

* RTC实时时钟
* 年月日时分秒可以按键设置

* 秒表计时功能

* 定时闹钟功能
* 显示年月日时分秒星期及农历（农历根据日期自动更新）

* 图形显示时钟

* 使用温湿度传感器，测量显示环境温湿度

* 可使用计算机串口进行时间同步

# 引脚对应关系
`key `使用矩阵键盘,使用一行和四列进行中断触发

行1 R1  -->  PC10

列1 C1  -->  PC6

列2 C2  -->  PC7

列3 C3  -->  PC8

列4 C4  -->  PC9

`DHT11 温湿度传感器`

VCC     -->  3.3V

DATA    -->  PE6

GND     -->  GND

`IIC oled 屏幕`

使用IIC1

GND 	-->		GND

VCC	  -->		3.3V

SCL		-->		PB6

SDA		-->		PB7

