@REM ============================================================
@REM Optional batch file to build TeraTerm v5.3 under out\teraterm\
@REM
@REM OUTPUT: 
@REM  - out/teraterm/teraterm/Release/ttermpro.exe
@REM ============================================================

set CONFIG=Release
@REM set CONFIG=Debug

@REM --------------
@REM build teraterm
@REM --------------
set TERATERM=%~dp0\out\teraterm
if not exist "out\" (
   mkdir out
)
if not exist %TERATERM% (
   git clone -b v5.3 https://github.com/TeraTermProject/teraterm.git %TERATERM%
)

cd %TERATERM%\libs
call :build_libs

cd %TERATERM%\buildtools\svnrev
call svnrev.bat

cd %TERATERM%\teraterm
MSBuild.exe ttermpro.v17.sln /p:Configuration=%CONFIG% /p:Platform="Win32"

cd %TERATERM%\..\..

exit /b


@REM ----------
@REM build_libs
@REM ----------
:build_libs
curl -LO https://github.com/kkos/oniguruma/releases/download/v6.9.10/onig-6.9.10.tar.gz
tar xf onig-6.9.10.tar.gz
move onig-6.9.10 oniguruma

curl -LO https://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/SFMT/SFMT-src-1.5.1.zip
tar xf SFMT-src-1.5.1.zip
move SFMT-src-1.5.1 SFMT

curl -LO https://ftp.openbsd.org/pub/OpenBSD/LibreSSL/libressl-3.9.2.tar.gz
tar xf libressl-3.9.2.tar.gz
move libressl-3.9.2 libressl

call buildoniguruma6.bat
call buildSFMT.bat
call buildlibressl.bat
exit /b
