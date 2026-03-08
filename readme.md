以CS2.0为例子，目前可以进行简单的输入输出和驱动。
后续尝试初始化参数，复杂的输入输出参数设置和获取。以及其他CS的版本：1.0 

之后还有ME类型的模型，ME1.0,2.0,3.0。其中关于积分器部分还需要了解。
对FMI++库的错误处理继续增加了解




### 问题记录
OpenModelica的CS2.0不能正确处理string类型,后续相关测试都未进行
FMI++很久么更新，后续要使用就是自己维护。


### 依赖库
1. boost
2. FMI++
3. libzip
    1. zlib

### 编译问题
1. FMI++
```
FMI++很久没维护，所以不支持新版本的cmake、visual studio、boost，可以使用修改后的版本https://github.com/sikuntiannan/FMIPP.git
```
2. libzip

```
libzip依赖zlib，需要先编译zlib，然后再编译libzip
linux下可以直接使用apt-get install libzip-dev安装,但windows下需要手动编译安装，vcpkg安装zlib不适合内网环境
问题：编译脚本目前没有写和测试
```
### CS2.0 设置初始化参数、输入输出参数
```c++
 // 设置模型参数：起点、终点、速度、容差
 {
     fmu.setValue( "start_x", 0.0 );  // 起点 x 坐标
     fmu.setValue( "start_y", 0.0 );  // 起点 y 坐标
     fmu.setValue( "end_x", 100.0 );  // 终点 x 坐标
     fmu.setValue( "end_y", 90.0 );   // 终点 y 坐标
     fmu.setValue( "v", 1.0 );        // 速度大小
     fmu.setValue( "eps", 0.01 );     // 到达终点判断容差
 }
   // 设置输入参数：控制模型停止/运动
 bool stop = false;
{
    fmu.setValue( "stop", stop );
}
// 获取输出参数：当前位置坐标
double current_x, current_y;
{
    fmu.getValue( "x", current_x );
    fmu.getValue( "y", current_y );
}

### 编译方式
执行对应平台的rebuild脚本。