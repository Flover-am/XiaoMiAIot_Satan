# XiaoMiAIot_Satan
小米AIot大赛repository

### 目录结构
- /Core和/Drivers是一些官方的驱动
- /OpenOCD 是一些OpenOCD的配置文件，svd文件可以在调试时候引入，看寄存器的值
- /MyLib 移植库原作者写的一些工具集
  - /USART 是串口输出库，
  - /delay 延迟库
- /UserApp 代码库
  - i2c.h/c 用于I2C通信的库
  - 
``` java
D:.
├─.idea
├─cmake-build-debug
│  ├─.cmake
│  │  └─api
│  │      └─v1
│  │          ├─query
│  │          └─reply
│  ├─CMakeFiles
│  │  ├─3.24.2
│  │  │  ├─CompilerIdASM
│  │  │  ├─CompilerIdC
│  │  │  │  └─tmp
│  │  │  └─CompilerIdCXX
│  │  │      └─tmp
│  │  ├─CLion_Test.elf.dir
│  │  │  ├─Core
│  │  │  │  └─Src
│  │  │  ├─Drivers
│  │  │  │  └─CMSIS
│  │  │  ├─MyLib
│  │  │  │  ├─delay
│  │  │  │  ├─LED
│  │  │  │  └─USART
│  │  │  ├─startup
│  │  │  └─UserApp
│  │  │      └─src
│  │  ├─CMakeTmp
│  │  └─pkgRedirects
│  └─Testing
│      └─Temporary
├─Core
│  ├─Inc
│  └─Src
├─Drivers
│  └─CMSIS
├─MyLib
│  ├─delay
│  ├─LED
│  └─USART
├─OpenOCD
├─startup
└─UserApp
    ├─inc
    └─src

```