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

	qint8 curFrame = 0; //��ǰ֡
	//bool executedOnce = false; //�Ƿ��ǵ�һ������
	bool startDrag = false; //�Ƿ�ʼ��ק

	void upateRoleAnimation(); //���½�ɫ����
	void roleDrag(); //��ɫ��ק
	bool eventFilter(QObject* watched, QEvent* ev) override; //�¼�������q	widget
	void initButton(); //��ʼ����ť
	void onTimerTimeout(); //��ʱ���ź�ѡ����
	void roleMove(); //��ɫ�ƶ�
	void iconActivated(QSystemTrayIcon::ActivationReason reason); //ͼ�꼤��
	void cutButtonClicked(); //��ͣ

private:
	QLabel* roleLabel; //��ɫ��ǩ

	QPushButton* closeButton; //�رհ�ť
	QPushButton* cutButton;
	QPushButton* openButton; //��·��

	bool isDragging = false; //�Ƿ�������ק
	bool isPaused = false; //�Ƿ���ͣ

	int clickCount; //curButton�������

	desktopWidget* DesktopWidget; //���洰��
};
