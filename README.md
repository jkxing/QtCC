# Qt Chinese Chess

2018-2019 summer term second week project

## 项目简介

可进行网络对战/残局加载的中国象棋游戏平台

## 工作流程

客户端与服务端为同一程序

作为服务端，可以在工具栏中点击listen，此时可以监听连接，监听时可点击dislisten取消监听，连接成功会在界面右侧显示对方ip

作为客户端，可以发起connect，输入对方ip地址和端口号，进行连接，连接成功会在界面右侧显示对方ip，且ready按钮变为可点击

客户端可加载残局，需要在点击ready之前加载

服务端在客户端点击ready后，点击start可开始游戏

开始游戏后双方随时可认输

界面左侧的lcdnumber显示的是剩余时间，如果轮到本方，计时会开始，否则计时器不会变。

超时判负。

点击棋子可查看棋子可走的位置，若轮到本方，可点击一个可走位置进行移动。

将军时会有提示音

对局结束后，重复ready-start即可开始对局，若上局有加载的残局文件，会取消之

客户端点击disconnect可取消连接

## 通信协议

第一个字节为表头

若表头为1，接下来接收4个字符，为对方棋子的移动

若表头为2，下一个字节为传输的残局文件的大小，接下来传残局

若表头为3，对方认输

若表头为4，客户端准备

若表头为5，服务端开始

若表头为6，对方发起了save

## 项目组成

MainWindow类：

​	ui：主界面

​	connectToHost：客户端连接到服务器

​	connectSucceed：连接成功后的槽函数

​	disConnect：客户端取消连接

​	gameStart：通知Game类实例游戏开始

​	reinit：重新初始化，准备下一局游戏

​	readData：从tcpSocket中读取数据

​	sendData：向tcpSocket写入数据

​	startTimeLimit/endTimeLimie：计时开始/结束

​	onTimeOut：和Qtimer关联，处理计时

​	win/lose：显示输赢对话框

BasicPieces类及其派生类Soldier,Horse,Pao,Car,Ele,Man,Boss：

​	均继承自QGraphicsObject，实现paint，boundingRect等函数

​	canMove(x,y,mp):虚函数，检测能否在地图为mp的情况下到达目标点(x,y)

​	changePosition(x,y):改变坐标到x,y

​	at(x,y):判断是否在x,y

Game类：

​	负责游戏的控制，包括对棋子的移动等等

​	成员变量有BasicPieces* pieces\[2\]\[20\]，存储己方和地方的所有棋子

​	receivedData处理接收的数据

​	sendData发送数据

​	PossiblePosition显示点击棋子后可移动到的位置以及点击可移动位置后的移动

​	CanecelPosition：取消选中的位置

​	win/lose:发送赢/输信号

​	loadFromFile:从文件中加载局面

​	save：保存残局到文件

​	start：开始计时	

Position类：

​	存储点与坐标的对应

ConnectDialog类：

​	显示客户端输入对方ip和port的对话框。

## 项目进展

​	day 1：实现了基本的界面构成，包括棋盘，棋子和UI

​	day2：

​		上午：实现了棋子的行走规则

​		下午：实现了网络通信

​		晚上：实现了对战流程

​	day3：

​		添加文档，补充细节，debugs