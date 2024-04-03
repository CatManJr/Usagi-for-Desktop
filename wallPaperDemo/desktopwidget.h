#pragma once

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QDesktopServices>

class desktopWidget  : public QWidget
{
	Q_OBJECT

public:
	desktopWidget(QWidget *parent = nullptr);
	~desktopWidget();
	void setWallpaper();
	void setPixmap(const QString &name);
private:
		QLabel* backLabel; //wallpaper
		QPixmap wallpaper; //wallpaper±¾Ìå
};
