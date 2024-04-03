#include "widget.h"
#include<QTimer>
#include <QtGui>
#include<QGraphicsDropShadowEffect>
#include<QMouseEvent>
#include <qguiapplication.h>
#include <QSystemTrayIcon>
#include <QIcon>
#include<QApplication>
#include <QFileDialog>
#include <QUrl>
#include <QSoundEffect>



Widget::Widget(QWidget* parent)
	: QWidget(parent, Qt::Widget | Qt::WindowStaysOnTopHint)
	, roleLabel(new QLabel(this))
	, curFrame(0)
	, DesktopWidget(new desktopWidget)
	, clickCount(0)
{
	// Set the widget's window title
	setWindowTitle("Usagi for Desktop");
	// Set the widget's icon
	setWindowIcon(QIcon(":/icon/resouces/icon/icon.png"));
	// Create a system tray icon
	QSystemTrayIcon* trayIcon = new QSystemTrayIcon(QIcon(":/icon/resouces/icon/icon.png"), this);
	// Set the system tray icon's tooltip
	trayIcon->setToolTip("Usagi for Desktop");
	// Connect the system tray icon's signal to a slot
	connect(trayIcon, &QSystemTrayIcon::activated, this, &Widget::iconActivated);

	// Show the system tray icon
	trayIcon->setVisible(true);


	//去掉窗口边框并使背景透明
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool |
		Qt::WindowType::FramelessWindowHint | Qt::WindowStaysOnTopHint); // 去掉标题栏,去掉任务栏显示，窗口置顶

	setAttribute(Qt::WA_TranslucentBackground);

	//开机音效
	QSoundEffect* effect = new QSoundEffect;
	effect->setSource(QUrl::fromLocalFile(":/music/resouces/music/yiyaha.wav"));
	effect->setVolume(0.5f);
	effect->play();


	//定时更新帧
	QTimer *updateTimer = new QTimer(this);
	connect(updateTimer, &QTimer::timeout, this, &Widget::onTimerTimeout);
	updateTimer->start(500);

	roleLabel->resize(200, 300);


	//窗口设置阴影
	QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
	shadow->setColor(QColor(230, 231, 232, 220));

	shadow->setBlurRadius(10);
	this->setGraphicsEffect(shadow);

	this->installEventFilter(this);

	initButton();

	DesktopWidget->show();
}

Widget::~Widget()
{

}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	// Show the widget when the system tray icon is activated
	show();
	if (reason == QSystemTrayIcon::Trigger) {
		QCoreApplication::instance()->quit();
	}
}

//通过私有信号判断是否需要暂停
void Widget::cutButtonClicked() {
	clickCount++;
	if (clickCount % 2 == 0)
		isPaused = false;  // 如果点击次数为偶数，则isPaused为false，否则为true
	else
		isPaused = true;
}

//实时更新角色动画
void Widget::upateRoleAnimation()
{
	QString qss("background-repeat: no-repeat;");
		roleLabel->setStyleSheet(qss + QString("background-image: url(:/static/resouces/mainCharacter/usagi/%0.png);").arg(curFrame)\
			+ "background-position: center;");

		if (isPaused == false) roleMove();
		curFrame = (curFrame + 1) % 42; // 循环遍历下一帧
}

//角色拖拽特殊动画
void Widget::roleDrag()
{
	QString qss("background-repeat: no-repeat;");
	roleLabel->setStyleSheet(qss + QString("background-image: url(:/drag/resouces/mainCharacter/usagi_drag/%0.png);").arg(curFrame)\
		+ "background-position: center;");
	curFrame = (curFrame + 1) % 8; // 循环遍历下一帧
}


//事件过滤器
bool Widget::eventFilter(QObject* watched, QEvent* ev)
{
	QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(ev);

	//判断鼠标左键按下
	static QPoint beginPosition; //必须是静态的，否则自动定位到左上角s
	if (ev->type() == QEvent::MouseButtonPress)
	{
		beginPosition = mouseEvent->globalPos() - this->pos();
	}
	
	//判断鼠标左键拖动
	else if (ev->type() == QEvent::MouseMove && 
		mouseEvent->buttons() & Qt::MouseButton::LeftButton)
	{
		isDragging = true;
		this->move(mouseEvent->globalPos() - beginPosition);
	}
	else if (ev->type() == QEvent::MouseButtonRelease)
	{
		isDragging = false;

	}

	//判断鼠标右键按下
	if (ev->type() == QEvent::MouseButtonPress &&
		mouseEvent->button() == Qt::MouseButton::RightButton)
	{
		QSoundEffect* effect = new QSoundEffect;
		effect->setSource(QUrl::fromLocalFile(":/music/resouces/music/usagi.wav"));
		effect->setVolume(0.5f);
		effect->play();
	}

	// 检查是否是拖入空文件夹
		// 检查是否是拖入文件夹
	if (ev->type() == QEvent::DragEnter)
	{
		QDragEnterEvent* dragEnterEvent = static_cast<QDragEnterEvent*>(ev);
		// 直接设置 isEat 为 true，不再检查文件夹是否为空
	}

	return false;
}

//初始化按钮
void Widget::initButton()
{
	closeButton = new QPushButton(this);
	closeButton->setGeometry(130, 50, 32, 32);
	closeButton->setObjectName("closeButton");
	closeButton->setStyleSheet("border-image: url(:/button/resouces/buttons/close.png);");

	cutButton = new QPushButton(this);
	cutButton->setGeometry(90, 50, 32, 32);
	cutButton->setObjectName("cutButton");
	cutButton->setStyleSheet("border-image: url(:/button/resouces/buttons/cut.png);");

	openButton = new QPushButton(this);
	openButton->setGeometry(50, 50, 32, 32);
	openButton->setObjectName("openButton");
	openButton->setStyleSheet("border-image: url(:/button/resouces/buttons/Switch.png);");

	this->setStyleSheet("QPushButton{background-color:rgb(239, 130, 160); border:none; border-radius:5px;}\
						 QPushButton#closeButton:hover{background-color:rgb(64, 173, 250);}\
						 QPushButton#openButton:hover{background-color:rgb(64, 173, 250);}\
						 QPushButton#cutButton:hover{background-color:rgb(64, 173, 250);}");


	connect(closeButton, &QPushButton::pressed, this, &Widget::close); //关闭窗口信号条
	connect(closeButton, &QPushButton::pressed, qApp, &QApplication::quit); //关闭程序信号条

	connect(cutButton, &QPushButton::pressed, this, &Widget::cutButtonClicked); //暂停跑动信号条

	connect(openButton, &QPushButton::pressed, this, [=]()
		{
			QString filename = QFileDialog::getOpenFileName(nullptr, "Select new wallpaper", "./", "Images (*.png *.jpg *.jpeg *.bmp)");
			if (filename.isEmpty())
			{
				qDebug() << "No Input";
				return ;
			}
			else
			{
				DesktopWidget->setPixmap(filename);
			}
		});//切换壁纸信号条

}

//定时器信号选择器
void Widget::onTimerTimeout()
{
	if (isDragging)
	{
		roleDrag();
	}
	else
	{
		upateRoleAnimation();
	}
}

//角色移动
void Widget::roleMove()
{
	if (curFrame >= 1 && curFrame <= 4 || curFrame > 30 && curFrame <= 32 || curFrame > 39 && curFrame <= 41) {
		if (this->x() >= 50) {
			this->move(this->x() - 50, this->y());
		}
	}
	else if (curFrame >= 9 && curFrame <= 13) {
		int newX = this->x() + 80;
		QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
		if (newX >= 0 && newX <= screenGeometry.width() - this->width())
			this->move(newX, this->y());
	}
	else if (curFrame >= 19 && curFrame <= 21 || curFrame >= 34 && curFrame <= 37) {
		if (this->x() >= 20)
		    this->move(this->x() - 20, this->y());
	}
}