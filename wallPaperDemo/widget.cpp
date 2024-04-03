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


	//ȥ�����ڱ߿�ʹ����͸��
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool |
		Qt::WindowType::FramelessWindowHint | Qt::WindowStaysOnTopHint); // ȥ��������,ȥ����������ʾ�������ö�

	setAttribute(Qt::WA_TranslucentBackground);

	//������Ч
	QSoundEffect* effect = new QSoundEffect;
	effect->setSource(QUrl::fromLocalFile(":/music/resouces/music/yiyaha.wav"));
	effect->setVolume(0.5f);
	effect->play();


	//��ʱ����֡
	QTimer *updateTimer = new QTimer(this);
	connect(updateTimer, &QTimer::timeout, this, &Widget::onTimerTimeout);
	updateTimer->start(500);

	roleLabel->resize(200, 300);


	//����������Ӱ
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

//ͨ��˽���ź��ж��Ƿ���Ҫ��ͣ
void Widget::cutButtonClicked() {
	clickCount++;
	if (clickCount % 2 == 0)
		isPaused = false;  // ����������Ϊż������isPausedΪfalse������Ϊtrue
	else
		isPaused = true;
}

//ʵʱ���½�ɫ����
void Widget::upateRoleAnimation()
{
	QString qss("background-repeat: no-repeat;");
		roleLabel->setStyleSheet(qss + QString("background-image: url(:/static/resouces/mainCharacter/usagi/%0.png);").arg(curFrame)\
			+ "background-position: center;");

		if (isPaused == false) roleMove();
		curFrame = (curFrame + 1) % 42; // ѭ��������һ֡
}

//��ɫ��ק���⶯��
void Widget::roleDrag()
{
	QString qss("background-repeat: no-repeat;");
	roleLabel->setStyleSheet(qss + QString("background-image: url(:/drag/resouces/mainCharacter/usagi_drag/%0.png);").arg(curFrame)\
		+ "background-position: center;");
	curFrame = (curFrame + 1) % 8; // ѭ��������һ֡
}


//�¼�������
bool Widget::eventFilter(QObject* watched, QEvent* ev)
{
	QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(ev);

	//�ж�����������
	static QPoint beginPosition; //�����Ǿ�̬�ģ������Զ���λ�����Ͻ�s
	if (ev->type() == QEvent::MouseButtonPress)
	{
		beginPosition = mouseEvent->globalPos() - this->pos();
	}
	
	//�ж��������϶�
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

	//�ж�����Ҽ�����
	if (ev->type() == QEvent::MouseButtonPress &&
		mouseEvent->button() == Qt::MouseButton::RightButton)
	{
		QSoundEffect* effect = new QSoundEffect;
		effect->setSource(QUrl::fromLocalFile(":/music/resouces/music/usagi.wav"));
		effect->setVolume(0.5f);
		effect->play();
	}

	// ����Ƿ���������ļ���
		// ����Ƿ��������ļ���
	if (ev->type() == QEvent::DragEnter)
	{
		QDragEnterEvent* dragEnterEvent = static_cast<QDragEnterEvent*>(ev);
		// ֱ������ isEat Ϊ true�����ټ���ļ����Ƿ�Ϊ��
	}

	return false;
}

//��ʼ����ť
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


	connect(closeButton, &QPushButton::pressed, this, &Widget::close); //�رմ����ź���
	connect(closeButton, &QPushButton::pressed, qApp, &QApplication::quit); //�رճ����ź���

	connect(cutButton, &QPushButton::pressed, this, &Widget::cutButtonClicked); //��ͣ�ܶ��ź���

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
		});//�л���ֽ�ź���

}

//��ʱ���ź�ѡ����
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

//��ɫ�ƶ�
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