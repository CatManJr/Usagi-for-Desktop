#include "desktopwidget.h"
#include <QHBoxLayout>
#include <qt_windows.h>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>

desktopWidget::desktopWidget(QWidget *parent)
	: QWidget(parent)
	, backLabel(new QLabel)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowState(Qt::WindowMaximized);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(backLabel);

	wallpaper.load(":/wallpaper/resouces/wallpapers/1.jpg");
	QRect screenRect = QGuiApplication::primaryScreen()->geometry();
	wallpaper = wallpaper.scaled(screenRect.width(), screenRect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	backLabel->setPixmap(wallpaper);
	setWallpaper();



	HWND hwndDesktop = FindWindow(L"Progman", NULL);

	SetParent((HWND)this->winId(), hwndDesktop);
	
	setWallpaper();

}

desktopWidget::~desktopWidget()
{}

void desktopWidget::setWallpaper()
{

	PDWORD_PTR result = nullptr;
	HWND hWorkerW = nullptr;
	HWND hDefView = nullptr;

	SendMessageTimeout(FindWindow(L"Progman", NULL), 0x52c, 0, 0, SMTO_NORMAL, 1000, result);

	hWorkerW = FindWindowEx(NULL, NULL, L"WorkerW", NULL);

	while ((!hDefView) && hWorkerW)
	{
		hDefView = FindWindowEx(hWorkerW, NULL, L"SHELLDLL_DefView", NULL);
		hWorkerW = FindWindowEx(NULL, hWorkerW, L"WorkerW", NULL);
	}

	ShowWindow(hWorkerW, 0); //����WorkerW����Ҫ�����쳣�׳�

	//�ҵ�����ľ��
	HWND desktopHwd = FindWindowW(L"Progman",NULL);
	if (!desktopHwd)
	{
		qDebug() << "Can't find desktop window" << desktopHwd;
		return;
	}
	//std::cout << "desktopHwd: " << desktopHwd << std::endl;
	//��this�ľ������Ϊ������Ӵ���
	SetParent((HWND)this->winId(), desktopHwd);
	// ���ô��ڵ�Z����Ϊ�ײ�
	SetWindowPos((HWND)this->winId(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void desktopWidget::setPixmap(const QString &filename)
{
	if (QPixmap(filename).isNull())
	{
		qDebug() << "Can't load image";
		return;
	}
	else {
		this->hide();
		QRect screenRect = QGuiApplication::primaryScreen()->geometry();
		wallpaper.load(filename);
		wallpaper = wallpaper.scaled(screenRect.width(), screenRect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		backLabel->setPixmap(wallpaper);
		this->showFullScreen();
	}
}
