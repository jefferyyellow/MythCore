#! /bin/sh

RoleIDArr=(1 2 3 4 5)

#./parsepbtool 192.168.5.20 3306 root webgame zhangchx_DB<<BatchExecute
#exit
#BatchExecute

for(( i = 0 ; i < 5 ; ++i ))
do
./parsepbtool 192.168.2.9 3306 root webgame WebGame_3<<BatchExecute
parse
UMS_ROLE_INFO role_gem_package ${RoleIDArr[$i]}
exit
BatchExecute


done

exit 0
