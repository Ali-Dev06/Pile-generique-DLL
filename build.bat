@echo off
REM ===================================================================
REM  Construction de la bibliotheque dynamique et du programme client
REM ===================================================================

echo [1/3] Compilation de la bibliotheque (mode EXPORT)...
g++ -c -DEXPRESSIONLIB_EXPORTS ExpressionLib.cpp -o ExpressionLib.o
if errorlevel 1 goto erreur

echo [2/3] Creation de ExpressionLib.dll...
g++ -shared -o ExpressionLib.dll ExpressionLib.o -Wl,--out-implib,libExpressionLib.a
if errorlevel 1 goto erreur

echo [3/3] Compilation du client et liaison avec la DLL...
g++ main.cpp -L. -lExpressionLib -o mainDLL.exe
if errorlevel 1 goto erreur

echo.
echo Construction terminee. Lancez mainDLL.exe
goto fin

:erreur
echo.
echo ERREUR pendant la construction.

:fin
pause
