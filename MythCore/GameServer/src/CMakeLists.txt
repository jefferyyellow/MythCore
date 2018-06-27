SET(TMP_SRC_LIST
    chatmodule.cpp
    curlhandle.cpp
    dailyactivity.cpp
    dailyactmodule.cpp
    dbjob.cpp
    dbmodule.cpp
    entity.cpp
    entityplayer.cpp
    equip.cpp
    gameserver.cpp
    gameserverconfig.cpp
    geometrytype.cpp
    gmcommandmanager.cpp
    gtest.cpp
    instance.cpp
    instancemodule.cpp
    internalmsg.cpp
    internalmsgpool.cpp
    itemcontainer.cpp
    itemmodule.cpp
    itemobject.cpp
    itemunit.cpp
    job.cpp
    locallogjob.cpp
    loginmodule.cpp
    loginplayer.cpp
    main.cpp
    mapconfigmanager.cpp
    mapmamager.cpp
    mapmodule.cpp
    mapregionsearch.cpp
    messagefactory.cpp
    objmanager.cpp
    objpool.cpp
    platjob.cpp
    platlog.cpp
    propertymodule.cpp
    propertyunit.cpp
    rankmodule.cpp
    redis.cpp
    scenejob.cpp
    serveractivity.cpp
    serveractivityunit.cpp
    serveractmodule.cpp
    serverutility.cpp
    skillmodule.cpp
    skillunit.cpp
    strutility.cpp
    taskconfig.cpp
    taskmodule.cpp
    taskunit.cpp
    teammodule.cpp
    template.cpp
)

#所有的文件名面前加上 src/(相对主CMakeLists的目录)
foreach(filename ${TMP_SRC_LIST})
	list(APPEND COMPLETE_SRC_LIST src/${filename})
endforeach(filename)

#下面这一句要这么写主CMakeLists才能得到对应的值
SET(SRC_LIST ${COMPLETE_SRC_LIST} CACHE INTERNAL ...)
#message(....${SRC_LIST})
