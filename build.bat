@REM ============================================================
@REM A batch file to build TTXExtraMenus
@REM
@REM OUTPUT: 
@REM  - Release/ or Debug/ 
@REM  - also copied to out\teraterm-5.3\TTXExtraMenus.dll
@REM
@REM Note:
@REM  - out\teraterm\       for referring header files
@REM  - out\teraterm-5.3\   actual execution binaries
@REM ============================================================

set CONFIG=Release
@REM set CONFIG=Debug

@REM -------------------
@REM build TTXExtraMenus
@REM -------------------
if not exist "out\" (
   mkdir out
)
cd out
if not exist "teraterm\" (
   git clone -b v5.3 https://github.com/TeraTermProject/teraterm.git teraterm
)
if not exist "teraterm-5.3\" (
   curl -LO https://github.com/TeraTermProject/teraterm/releases/download/v5.3/teraterm-5.3.zip
   tar xf teraterm-5.3.zip
)
cd ..

MSBuild.exe TTXExtraMenus.v17.sln /p:Configuration=%CONFIG% /p:Platform="win32"

@echo.
@echo Type out\teraterm-5.3\ttermpro.exe to run
