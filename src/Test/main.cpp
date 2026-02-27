
// #include <import/FMICoSimulation.h>
// #include <import/FMIInterface.h>
#include <common/FMIPPConfig.h>
#include <common/fmi_v1.0/fmiModelTypes.h>
#include <import/base/include/FMUCoSimulation_v2.h>
#include <import/base/include/ModelDescription.h>
#include <import/base/include/ModelManager.h>
#include <iostream>

int testCSTwo()
{
    try
    {
        // 1. 加载 FMU
        std::string              FMUPath = "file://D:/work/FMUs/Rectifier/";
        std::string              FMUName = "Rectifier";
        fmi_2_0::FMUCoSimulation fmu( FMUPath, FMUName );

        // 2. 实例化
        if ( fmu.instantiate( FMUName, 0, fmippFalse, fmippFalse ) != fmippOK )
        {
            throw std::runtime_error( "Instantiation failed" );
        }
        double StopTime = 0.1;
        // 初始化
        if ( fmu.initialize( 0, fmippTrue, StopTime ) != fmippOK )
        {
            throw std::runtime_error( "Initialization failed" );
        }
        fmippTime time = 0;
        fmippTime step = 1e-5;
        while ( time < StopTime )
        {
            fmippStatus status = fmu.doStep( time, step, true );
            if ( status != fmippOK )
            {
                std::cerr << "doStep failed at " << status << std::endl;
                break;
            }

            // 更新时间
            time += step;

            // 获取输出 y
            double voltage;
            fmu.getValue( "outputs", voltage );  // 或 fmu.getValue(40, y);
            std::cout << "time: " << time << ", voltage : " << voltage << std::endl;
        }
    }
    catch ( const std::exception& e )
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

int main()
{
    std::string      modelName = "Rectifier";
    std::string      fmuPath   = "D:/work/FMUs/Rectifier/modelDescription.xml";
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
    int index = 10;
    while ( --index )
    {
        testCSTwo();
    }
    return 0;
}