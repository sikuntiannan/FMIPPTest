
// #include <import/FMICoSimulation.h>
// #include <import/FMIInterface.h>
#include <common/FMIPPConfig.h>
#include <common/fmi_v1.0/fmiModelTypes.h>
#include <import/base/include/ModelManager.h>
#include <iostream>

int main()
{
    // 2. 创建Import对象并加载FMU
    std::string                 modelName = "Rectifier";
    std::string                 fmuPath   = "file://D:/work/FMUs/Rectifier/";
    ModelManager&               manager   = ModelManager::getModelManager();
    ModelManager::LoadFMUStatus status    = ModelManager::failed;
    FMUType                     type      = invalid;
    status                                = ModelManager::loadFMU( modelName, fmuPath, fmiTrue, type );
    if ( status != ModelManager::success )
    {
        std::cout << "Failed to load FMU" << std::endl;
    }
    if ( type == invalid )
    {
        std::cout << "Failed to determine FMU type" << std::endl;
    }

    auto bareFMU = manager.getInstance( modelName );  // (FMI ME/CS 2.0)
    if ( nullptr == bareFMU.get() )
    {
        std::cout << "Failed to get model" << std::endl;
    }
    return 0;
}