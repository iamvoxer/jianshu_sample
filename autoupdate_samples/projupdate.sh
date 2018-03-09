#!/bin/bash
#请务必确保最后部署的项目
#1. 目录结构如下：
#		-projectname
#        	-bin：    	存放.NetCore项目编译发布后的所有dll及相关文件
#        	-update   	存放当前projupdate.sh文件以及升级会自动生成相关目录和文件
#2. bin目录下的配置文件只包含appsetting.json以及自定义的ConfigFile目录
#3. bin目录下的所有日志豆都放在log目录下，这个目录是不需要备份的

#----------------------项目部署者需要修改的部分-----------------------#
#定义项目名称
proj_name="xxx项目的uat版本"

#定义项目源代码的git地址,包含用户名和密码，如果不考虑安全性，可直接写死密码在sh文件里
proj_src_git="http://tester:$1@xx.yy.zz:8080/r/tools/NetCoreProjectAutoupdate.git"
#定义项目源代码的git分支名，这个尽量不要用主分支
proj_src_git_branch="master"
#定义项目源代码的所在子目录,包含.csproj文件所在目录，前面包含/，后面不包含
proj_scr_git_path="/TestSample/src/MigrationsSample/MigrationsSample"

#定义项目配置的git地址,包含用户名和密码，如果不考虑安全性，可直接写死密码在sh文件里
proj_config_git="http://tester:$2@xx.yy.zz:8080/r/tools/NetCoreProjectAutoupdate.git"
#定义项目配置的所在子目录，前面包含/，后面不包含
proj_config_git_path="/TestSample/config/uat"
#---------------------------------------------------------------------#

#----------------------变量定义---------------------------------------#
#脚本的版本,格式为v.v.yyyymmdd
version="1.0.20180308"

#执行升级会记录日志文件，文件名为当日日期yyyy_mm_dd.log
logpath="./logs"
#转换为绝对路径
logname="`cd ${logpath};pwd`/`date +%Y_%m_%d`.log"

binpath="../bin"
backpath="./backup"
#---------------------------------------------------------------------#

#----------------------函数定义---------------------------------------#
#打印到命令行，并追加到日志文件
funecho(){
	echo $1
	if [ ! -d ${logpath} ];
	then
		mkdir ${logpath}
	fi
	echo $1 >>${logname}
}
gitclone(){
	# $1: git地址 $2: 分支  $3: 保存的目录
	#先删除历史clone留下的目录
	delfile $3
	funecho "git clone -b $2 $1 $3 2>&1|tee -a ${logname}"
	git clone -b $2 $1 $3 2>&1|tee -a ${logname}
	funecho " "
}

delfile(){
	if [ -d $1 ];
		then 
		funecho "删除文件或目录 $1"
		rm -rf $1 
	fi
}
copyfile(){
	funecho "拷贝文件或目录从$1到$2"
	cp -r $1 $2
}
migration(){
	cd ./src${proj_scr_git_path}
	delfile "./bin"
	delfile "./obj"

	funecho "dotnet build"
	dotnet build 2>&1| tee -a ${logname}
	
	funecho "创建mirgration"
	funecho "dotnet ef migrations add update`date +%Y_%m_%d_%H_%M_%S`"
	dotnet ef migrations add update`date +%Y_%m_%d_%H_%M_%S` 2>&1| tee -a ${logname}
	
	funecho "更新mirgration到数据库"
	funecho "dotnet ef database update"
	dotnet ef database update 2>&1| tee -a ${logname}
	cd -
	
}
push2configgit(){
	delfile "./config${proj_config_git_path}/Migrations"
	copyfile "./src${proj_scr_git_path}/Migrations" "./config${proj_config_git_path}/Migrations"
	cd ./config

	funecho "git add -A"
	git add -A 2>&1| tee -a ${logname}

	funecho "git commit -m \"自动提交\""
	git commit -m "自动提交" 2>&1| tee -a ${logname}

	funecho "git push"
	git push 2>&1| tee -a ${logname}
	cd - 
}
backup(){
	if [ ! -d ${backpath} ];
	then
		mkdir ${backpath}
	fi
	if [ ! -d ${binpath} ];
	then
		mkdir ${binpath}
	else
		funecho "tar -zcvf ${backpath}/`date +%Y_%m_%d_%H_%M_%S`.tar.gz --exclude=${binpath}/log ${binpath}"
		tar -zcvf ${backpath}/`date +%Y_%m_%d_%H_%M_%S`.tar.gz --exclude=${binpath}/log ${binpath}  2>&1| tee -a ${logname}
	fi
}
publish(){
	cd ./src${proj_scr_git_path}
	funecho "dotnet publish"
	dotnet publish 2>&1| tee -a ${logname}
	cd -
	
	funecho "cp -r -f ./src${proj_scr_git_path}/bin/Debug/netcoreapp2.0/publish/* ${binpath}"
	cp -r -f ./src${proj_scr_git_path}/bin/Debug/netcoreapp2.0/publish/* ${binpath} 2>&1| tee -a ${logname}

}
#---------------------------------------------------------------------#

#----------------------执行过程----------------------------------------#
funecho " "
funecho "`date` 开始执行升级......"

funecho "1. 下载项目源码文件"
gitclone ${proj_src_git} ${proj_src_git_branch} "./src"

funecho "2. 下载项目配置文件,只考虑主分支"
gitclone ${proj_config_git} "master" "./config"

funecho "3. 删除项目源码文件中的相关配置目录和文件以及开发者自己创建的Migrations"
delfile "./src${proj_scr_git_path}/appsettings.json"
delfile "./src${proj_scr_git_path}/ConfigFile"
delfile "./src${proj_scr_git_path}/Migrations"

funecho "4. 拷贝git上下载的项目配置文件和目录到src下"
copyfile "./config${proj_config_git_path}/appsettings.json" "./src${proj_scr_git_path}/appsettings.json"
copyfile "./config${proj_config_git_path}/ConfigFile" "./src${proj_scr_git_path}/ConfigFile"
copyfile "./config${proj_config_git_path}/Migrations" "./src${proj_scr_git_path}/Migrations"

funecho "5. 在src下执行数据库迁移"
migration

funecho "6. Migrations变化后，需拷贝并push回配置git"
push2configgit

funecho "7. 备份旧的发布文件，log目录不备份"
backup

funecho "8. 重新发布并更新到bin目录"
publish

funecho "`date` 执行升级完成！"
#---------------------------------------------------------------------#


read noexit