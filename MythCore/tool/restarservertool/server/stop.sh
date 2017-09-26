#!/bin/sh

tool_pid_arr=`ps aux | grep $USER | grep restarttool | grep -v grep | awk '{print $2}'`
tool_pid=${tool_pid_arr[0]}


if [ "$tool_pid"  ] 
then
	echo killed process tool_pid :
	echo $tool_pid
	kill -9 ${tool_pid_arr[0]}
fi
