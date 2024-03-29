     # -Actian-Zen-C-
嵌入式数据库Actian Zen的C++使用
为了演示嵌入式环境下非常容易的使用Actian Zen，我将使用一个树莓派设备作为嵌入式系统，并且在它上面运行一个简单的实例；这个实例会告诉你在服务器桌面系统和嵌入式环境之间ZEn进行数据交换是如何的完美。
这篇文章提供给你一些产品和服务的开发的相关信息。希望能够帮助到大家。
越来越多的智能互联的物联网设备现在导致数据的指数级增长，这些数据来源于传感器，驱动单元，控制系统和智能相机采集的实时图像数据。所以如今显而易见的问题是：如何合理的管理这些数据呢？实际上，这些边缘化的设备被限制住了：CPU处理速度慢，较小的内存空间，和小容量的磁盘。
从开发者的角度讲，这些限制要求产生的代码空间占用要小，对磁盘的占用要尽量合理。目前嵌入式的应用程序已经能够做到将代码空间足够紧凑，读取数据的方式已经是最优的方法。
新型的嵌入式数据库提供给开发者他们一个非常有用并且易于实现的物联网方案。
Actian Zen
Actian Zen 是第一个企业级嵌入式数据库，它提供有效的数据管理和从数据中心载入和载出的数据分析的能力。
使用多个网管和边缘设备意味着在所有应用节点多个数据和数据管理解决方案交互。通常这要求客户的数据传输和ETL 的操作指令能够在设备和中心数据库之间移动，这使得部署和和维护系统成为一个难题。这不仅会拖累部署工作，也会导致没必要的复杂度和更大的管理代价。不符合奥卡姆剃刀原理。
Actian Zen提供通用的数据格式和文件格式，满足大多数平台。开发者不用过多关注自定义的ETL 操作。
选择树莓派的原因是它很便宜，是很多嵌入式系统开发的解决方案搭建。可以说是大众选择。我使用存入和获取数据的操作来演示。
安装步骤

树莓派系统的基本的配置可以参考树莓派的官网和各种csdn 的博客。我们需要做的是安装树莓派操作系统，参考文档。现在我假设你了解基本的数据库语句操作(本博客只使用到select 和 insert)和c++ 编程。
Zen 的下载地址：https://esd.actian.com/product ，我使用的树莓派的系统是：Raspbian ARM 32-bit，下载对应的即可。安装过程参考提供的readMe。
还需要确定下载了对应的数据库管理软件，实际上包含了c++的数据库操作API，解压PSQL-SDK可以找到btrieveCpp.h 等相关头文件。

时间序列Demo
万事具备，只欠demo，现在开始我用c++实现一个获取时间序列上的温度值数据，并将其保存进Zen 数据库。为了仿真真实的温度，我就直接读取片上系统的温度寄存器保存的值(sys/class/thermal/thermal_zone0/temp)，之后再对数据库里面的温度数据进行查询和读取。
demo代码地址.





