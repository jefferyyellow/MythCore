:: �����Լ���·��
set TortoiseSVN=C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe
set ClientPath=E:\mxw2Client\AdventureKing\bin-debug
set ServerPath=E:\mxw2linuxserver

:: ����ͻ���ģ��
E:
cd %ClientPath%
call Package.bat

::"%TortoiseSVN%" /command:add /path:"%ClientPath%\Resource" /closeonend:2
"%TortoiseSVN%" /command:commit /path:"%ClientPath%\Resource*%ClientPath%\language*%ClientPath%\data" /logmsg:"�ƽ���:����ģ��" /closeonend:2
"%TortoiseSVN%" /command:commit /path:"%ServerPath%\server\gameserver\config\data\template" /logmsg:"�ƽ���:����ģ��" /closeonend:2

:: 4096 ���� ϵͳģʽ�����û���Ӧ��Ϣ��ǰ������Ӧ�ó��򶼱�����
:: mshta vbscript:msgbox("�ͻ��˷�����ģ�������Ѹ��²��ύ !")(window.close)
::mshta vbscript:msgbox("�ͻ��˷�����ģ�������Ѹ��²��ύ!",4096,"ģ�����")(window.close)
pause