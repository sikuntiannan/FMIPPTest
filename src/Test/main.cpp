#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <cmath>
#include <common/FMIPPConfig.h>
#include <common/fmi_v1.0/fmiModelTypes.h>
#include <fstream>
#include <import/base/include/FMUCoSimulation_v2.h>
#include <import/base/include/ModelDescription.h>
#include <import/base/include/ModelManager.h>
#include <iostream>
#include <zip.h>

/**
 * @brief 使用libzip解压文件
 * @param zipFilePath 压缩包文件路径
 * @param extractPath 解压目标路径
 * @return int 0表示成功，非0表示失败（错误码）
 */
int extractZipFile( const std::string& zipFilePath, const std::string& extractPath )
{
    int    error   = 0;
    zip_t* archive = nullptr;

    // 1. 打开压缩包
    archive = zip_open( zipFilePath.c_str(), 0, &error );
    if ( !archive )
    {
        std::cerr << "Failed to open zip file: " << zipFilePath << std::endl;
        return -1;
    }

    // 2. 创建解压目录
    boost::filesystem::create_directories( extractPath );

    // 3. 获取文件数量
    zip_int64_t num_entries = zip_get_num_entries( archive, 0 );
    if ( num_entries < 0 )
    {
        std::cerr << "Failed to get number of entries in zip file" << std::endl;
        zip_close( archive );
        return -2;
    }

    // 4. 遍历并解压所有文件
    for ( zip_int64_t i = 0; i < num_entries; i++ )
    {
        // 获取文件名
        const char* name = zip_get_name( archive, i, 0 );
        if ( !name )
        {
            std::cerr << "Failed to get entry name at index " << i << std::endl;
            continue;
        }

        // 构建完整的目标路径
        boost::filesystem::path targetPath = boost::filesystem::path( extractPath ) / name;

        // 如果是目录，创建目录
        if ( name[ strlen( name ) - 1 ] == '/' )
        {
            boost::filesystem::create_directories( targetPath );
            continue;
        }

        // 确保父目录存在
        boost::filesystem::create_directories( targetPath.parent_path() );

        // 打开压缩文件
        zip_file_t* file = zip_fopen_index( archive, i, 0 );
        if ( !file )
        {
            std::cerr << "Failed to open file in zip: " << name << std::endl;
            continue;
        }

        // 创建目标文件
        std::ofstream outFile( targetPath.string(), std::ios::binary );
        if ( !outFile.is_open() )
        {
            std::cerr << "Failed to create output file: " << targetPath << std::endl;
            zip_fclose( file );
            continue;
        }

        // 读取并写入文件内容
        char        buffer[ 8192 ];
        zip_int64_t n;
        while ( ( n = zip_fread( file, buffer, sizeof( buffer ) ) ) > 0 )
        {
            outFile.write( buffer, n );
        }

        // 关闭文件
        zip_fclose( file );
        outFile.close();

        std::cout << "Extracted: " << name << " -> " << targetPath << std::endl;
    }

    // 5. 关闭压缩包
    zip_close( archive );

    std::cout << "Successfully extracted " << num_entries << " files to " << extractPath << std::endl;
    return 0;
}

/**
 * @brief 加载并驱动FMU的CS2.0模型
 * @param FMU模型地址
 * @param FMU模型名称
 * @return int 0表示成功，非0表示失败（错误码）
 */
int testCSTwo( std::string FMUPath, std::string FMUName )
{
    std::string ModelPath = ( boost::format( "file://%s" ) % FMUPath ).str();
    try
    {
        fmi_2_0::FMUCoSimulation fmu( ModelPath, FMUName );

        // 2. 实例化
        if ( fmu.instantiate( FMUName, 0, fmippFalse, fmippFalse ) != fmippOK )
        {
            throw std::runtime_error( "Instantiation failed" );
        }

        // 设置模型参数：起点、终点、速度、容差
        {
            fmu.setValue( "start_x", 0.0 );  // 起点 x 坐标
            fmu.setValue( "start_y", 0.0 );  // 起点 y 坐标
            fmu.setValue( "end_x", 100.0 );  // 终点 x 坐标
            fmu.setValue( "end_y", 90.0 );   // 终点 y 坐标
            fmu.setValue( "v", 1.0 );        // 速度大小
            fmu.setValue( "eps", 0.01 );     // 到达终点判断容差
        }

        // 计算最大仿真时间（距离/速度 * 1.5 安全系数）
        double    distance  = sqrt( pow( 100.0, 2 ) + pow( 90.0, 2 ) );
        fmippTime StopTime  = ( distance / 1.0 ) * 1.5;  // 速度=1;
        fmippTime StartTime = 0;
        fmippTime step      = 1e-2;
        // 初始化
        if ( fmu.initialize( 0, fmippTrue, StopTime ) != fmippOK )
        {
            throw std::runtime_error( "Initialization failed" );
        }
        // 控制参数：是否停止运动
        bool stop = false;
        while ( StartTime < StopTime )
        {
            // 设置输入参数：控制模型停止/运动
            {
                fmu.setValue( "stop", stop );
            }

            fmippStatus status = fmu.doStep( StartTime, step, true );
            if ( status != fmippOK )
            {
                std::cerr << "doStep failed at " << status << std::endl;
                break;
            }

            // 更新时间
            StartTime += step;

            // 获取输出参数：当前位置坐标
            double current_x, current_y;
            {
                fmu.getValue( "x", current_x );
                fmu.getValue( "y", current_y );
            }
            // 输出并检查当前状态
            {
                // 计算到终点的距离
                double distanceToEnd = sqrt( pow( 100.0 - current_x, 2 ) + pow( 90.0 - current_y, 2 ) );

                // 格式化输出结果
                std::cout << boost::format( "Now Pos: (%.1f, %.1f) | Distance : %.2f | State : %s" ) % current_x % current_y % distanceToEnd % ( stop ? "Stop" : "Runing" ) << std::endl;

                if ( stop )
                {
                    break;
                }

                // 检查是否到达终点（在容差范围内）
                if ( distanceToEnd <= 0.01 )
                {
                    stop = true;
                }
            }
        }
    }
    catch ( const std::exception& e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int main()
{
    std::string      FmuName      = "Move2D";
    std::string      FmuUnzipPath = "D:/work/FMUs/Move2D";
    std::string      FmuZipPath   = ( boost::format( "D:/work/FMUs/dengzheng Test/%s.fmu" ) % FmuName ).str();
    std::string      fmuPath      = ( boost::format( "%s/modelDescription.xml" ) % FmuUnzipPath ).str();
    ModelDescription ModelInfor( fmuPath );
    FMUType          type = ModelInfor.getFMUType();
    switch ( type )
    {
    case FMUType::invalid: {
        std::cout << "invalid" << std::endl;
        break;
    }
    case FMUType::fmi_1_0_cs: {
        std::cout << "fmi_1_0_cs" << std::endl;
        break;
    }
    case FMUType::fmi_1_0_me: {
        std::cout << "fmi_1_0_me" << std::endl;
        break;
    }
    case FMUType::fmi_2_0_me: {
        std::cout << "fmi_2_0_me" << std::endl;
        break;
    }
    case FMUType::fmi_2_0_cs: {
        std::cout << "fmi_2_0_cs" << std::endl;

        break;
    }
    case FMUType::fmi_2_0_me_and_cs: {
        std::cout << "fmi_2_0_me_and_cs" << std::endl;
        break;
    }
    default: {
        break;
    }
    }
    int index = 1;
    while ( index-- )
    {
        // 1. 加载 FMU
        testCSTwo( FmuUnzipPath, FmuName );
    }
    return 0;
}