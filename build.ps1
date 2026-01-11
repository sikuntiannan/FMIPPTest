# build.ps1
# 用于构建和安装CMake项目的PowerShell脚本

# 显示执行的命令（关闭调试输出）
# Set-PSDebug -Off
# 显示执行的命令（启用调试输出）
# Set-PSDebug -Trace 1

# 检查build目录是否存在
if (-not (Test-Path -Path "build" -PathType Container)) {
    Write-Error "Error: build directory does not exist. Please run CMake configure step first." -ForegroundColor Red
    exit 1
}

# 进入build目录
Set-Location -Path "build"

# 执行构建和安装
$cpuCount = (Get-CimInstance -ClassName Win32_ComputerSystem).NumberOfLogicalProcessors
Write-Host "Building and installing project with $cpuCount parallel jobs..." -ForegroundColor Green
# 执行构建和安装
cmake --build . --target install --parallel

# 返回到原始目录
Set-Location -Path ".."

# 检查构建是否成功
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}

Write-Host "Build and install completed successfully!" -ForegroundColor Green