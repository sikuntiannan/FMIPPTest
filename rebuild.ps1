param(
    [ValidateSet("Debug", "Release")]
    [string]$BuildType = "Debug",
    
    [string]$Generator = "Visual Studio 17 2022",
    [string]$Toolset = "v143",
    [string]$Architecture = "x64"
)
# build.ps1
# 用于构建和安装CMake项目的PowerShell脚本

# 显示执行的命令（关闭调试输出）
# Set-PSDebug -Off
# 显示执行的命令（启用调试输出）
# Set-PSDebug -Trace 1

# 检查build目录是否存在，存在则删除重建，不存在则创建
if (Test-Path -Path "build" -PathType Container) {
    Write-Host "Removing existing build directory..." -ForegroundColor Yellow
    Remove-Item -Path "build" -Recurse -Force
}
# 创建build目录
Write-Host "Creating build directory..." -ForegroundColor Yellow
New-Item -Path "build" -ItemType Directory -Force

# 进入build目录
Set-Location -Path "build"

# 执行构建和安装
$cpuCount = (Get-CimInstance -ClassName Win32_ComputerSystem).NumberOfLogicalProcessors
Write-Host "Building and installing project with $cpuCount parallel jobs..." -ForegroundColor Green

# 生成编译配置
$cmakeArgs = @(
    "-DCMAKE_BUILD_TYPE=$BuildType",
    "-G", $Generator,
    "-T", $Toolset,
    "-A", $Architecture,
    ".."
)
cmake @cmakeArgs
# 执行构建和安装
$buildArgs = @(
    "--build", ".",
    "--target", "install",
    "--target", "DeployConfig",
    "--parallel", $cpuCount
)
cmake @buildArgs

# 返回到原始目录
Set-Location -Path ".."

# 检查构建是否成功
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}

Write-Host "Build and install completed successfully!" -ForegroundColor Green