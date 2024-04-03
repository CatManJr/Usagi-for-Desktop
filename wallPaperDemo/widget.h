#pragma once

#include <QWidget>
#include<qpixmap.h>
#include<qlabel.h>
#include<QPushButton>
#include <QSystemTrayIcon>
#include <QIcon>
#include "desktopwidget.h"

class Widget  : public QWidget
{
	Q_OBJECT

public:
	Widget(QWidget *parent = nullptr);
	~Widget();

	qint8 curFrame = 0; //当前帧
	//bool executedOnce = false; //是否是第一次启动
	bool startDrag = false; //是否开始拖拽

	void upateRoleAnimation(); //更新角色动画
	void roleDrag(); //角色拖拽
	bool eventFilter(QObject* watched, QEvent* ev) override; //事件过滤器q	widget
	void initButton(); //初始化按钮
	void onTimerTimeout(); //定时器信号选择器
	void roleMove(); //角色移动
	void iconActivated(QSystemTrayIcon::ActivationReason reason); //图标激活
	void cutButtonClicked(); //暂停

private:
	QLabel* roleLabel; //角色标签

	QPushButton* closeButton; //关闭按钮
	QPushButton* cutButton;
	QPushButton* openButton; //打开路径

	bool isDragging = false; //是否正在拖拽
	bool isPaused = false; //是否暂停

	int clickCount; //curButton点击次数

	desktopWidget* DesktopWidget; //桌面窗口
};
