:: 用你自己的路径
set TortoiseSVN=C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe
set ClientPath=E:\mxw2Client\AdventureKing\bin-debug
set ServerPath=E:\mxw2linuxserver

:: 打包客户端模板
E:
cd %ClientPath%
call Package.bat

::"%TortoiseSVN%" /command:add /path:"%ClientPath%\Resource" /closeonend:2
"%TortoiseSVN%" /command:commit /path:"%ClientPath%\Resource*%ClientPath%\language*%ClientPath%\data" /logmsg:"黄建恒:更新模板" /closeonend:2
"%TortoiseSVN%" /command:commit /path:"%ServerPath%\server\gameserver\config\data\template" /logmsg:"黄建恒:更新模板" /closeonend:2

:: 4096 —— 系统模式：在用户响应消息框前，所有应用程序都被挂起
:: mshta vbscript:msgbox("客户端服务器模板数据已更新并提交 !")(window.close)
::mshta vbscript:msgbox("客户端服务器模板数据已更新并提交!",4096,"模板更新")(window.close)
pause