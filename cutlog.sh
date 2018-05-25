#!/bin/bash
#设置日志文件存放目录
logs_path="/data/logs/nginx/"
#设置pid文件
pid_path="/var/run/nginx.pid"

#重命名日志文件
mv ${logs_path}nginx-access.log ${logs_path}nginx-access-$(date -d "yesterday" +"%Y-%m-%d").log

#向nginx主进程发信号重新打开日志
kill -USR1 `cat ${pid_path}`