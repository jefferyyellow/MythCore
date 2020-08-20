require("gameserverconfig/script/daily_activity/daily_activity_type")
require("gameserverconfig/script/daily_activity/world_boss")

DailyActivity_GlobalStartFunc = {
	[EmDailyActType.WorldBoss] = WorldBoss_Start
}

DailyActivity_GlobalEndFunc = {
	[EmDailyActType.WorldBoss] = WorldBoss_End
}

DailyActivity_GlobalNoticeFunc = {
	[EmDailyActType.WorldBoss] = WorldBoss_Notice
}


function DailyActivity_ActivityStart(nActType, nActID)
	if nil == DailyActivity_GlobalStartFunc[nActType] then
		return
	end

	DailyActivity_GlobalStartFunc[nActType](nActID)
end

function DailyActivity_ActivityEnd(ActType, ActID)
	if nil == DailyActivity_GlobalEndFunc[nActType] then
		return
	end
	DailyActivity_GlobalEndFunc[nActType](nActID)
end

function DailyActivity_ActivityNotice(ActType, ActID)
	if nil == DailyActivity_GlobalNoticeFunc[nActType] then
		return
	end	
	DailyActivity_GlobalNoticeFunc[nActType](nActID)
end