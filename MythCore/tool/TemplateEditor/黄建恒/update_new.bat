@echo off
:: �����Լ���·��
set TortoiseSVN=C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe
set ClientPath=E:\mxw2Client\AdventureKing\bin-debug\
set ServerPath=E:\mxw2linuxserver\
set DesignPath=E:\ð����2�߻��ĵ�\ģ������\

E:
cd %DesignPath%

"%TortoiseSVN%" /command:update /path:"%ClientPath%Resource*%ClientPath%language*%ClientPath%data*%ClientPath%Package.swf" /closeonend:2
"%TortoiseSVN%" /command:update /path:"%SERVER_PATH%\server\gameserver\config\data\template" /closeonend:2

:: ���²߻��ύ��ģ���ļ�
tskill TemplateBuilder
tskill TemplateEditor
"%TortoiseSVN%" /command:update /path:"%DesignPath%" /closeonend:2

:: ���н���д�������з�ʽ������ģ��
%DesignPath%TemplateEditor.exe create

:: Ϊ��ֹ�����ȴ�5s�ӣ����Խ����ر�������
ping 127.0.0.1 -n 5 > nul

echo ######���·�����ģ������######
xcopy %DesignPath%template_server.dat %SERVER_PATH%\server\gameserver\config\data\template\template_server.dat /Y

:: �������
echo=
echo ######���¿ͻ���ģ������######
xcopy %DesignPath%lang_tpl.xml %ClientPath%language /Y
echo=
xcopy %DesignPath%client\*.* %ClientPath%Resource /s /h /Y

::echo ######�뵽�ͻ���·����ִ���ύ��svn������������ύ######

::�ύ��svn
call %DesignPath%update_template_new.bat
