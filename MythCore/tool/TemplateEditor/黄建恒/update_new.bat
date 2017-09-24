@echo off
:: 用你自己的路径
set TortoiseSVN=C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe
set ClientPath=E:\mxw2Client\AdventureKing\bin-debug\
set ServerPath=E:\mxw2linuxserver\
set DesignPath=E:\冒险王2策划文档\模板数据\

E:
cd %DesignPath%

"%TortoiseSVN%" /command:update /path:"%ClientPath%Resource*%ClientPath%language*%ClientPath%data*%ClientPath%Package.swf" /closeonend:2
"%TortoiseSVN%" /command:update /path:"%SERVER_PATH%\server\gameserver\config\data\template" /closeonend:2

:: 更新策划提交的模板文件
tskill TemplateBuilder
tskill TemplateEditor
"%TortoiseSVN%" /command:update /path:"%DesignPath%" /closeonend:2

:: 运行建恒写的命令行方式，编译模板
%DesignPath%TemplateEditor.exe create

:: 为防止出错，等待5s钟，可以紧急关闭批处理
ping 127.0.0.1 -n 5 > nul

echo ######更新服务器模板数据######
xcopy %DesignPath%template_server.dat %SERVER_PATH%\server\gameserver\config\data\template\template_server.dat /Y

:: 输出空行
echo=
echo ######更新客户端模板数据######
xcopy %DesignPath%lang_tpl.xml %ClientPath%language /Y
echo=
xcopy %DesignPath%client\*.* %ClientPath%Resource /s /h /Y

::echo ######请到客户端路径下执行提交至svn的批处理进行提交######

::提交到svn
call %DesignPath%update_template_new.bat
