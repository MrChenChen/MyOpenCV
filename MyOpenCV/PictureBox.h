#pragma once
#include <stdafx.h>
#include <opencv.hpp>
#include <GlobalFunction.h>

using namespace cv;

class PictureBox : public QWidget
{
	Q_OBJECT

public:
	PictureBox(QWidget *parent) :QWidget(parent)
	{
		m_pen.setColor(QColor(255, 0, 0));
		m_pen.setWidth(2);
	}


	~PictureBox() {}



	void ClearRect()
	{
		list_Rect.clear();
	}


	void SetImage(QImage & image)
	{
		if (image.isNull())
		{
			QMessageBox::about(0, "QImage is Null", "Error");
		}
		m_image = image;
		update();
	}


	void SetImage(Mat & mat)
	{
		SetImage(cvMat2QImage(mat));
	}

	void SetScale(double d)
	{
		m_Scale = d;
		update();
	}

	virtual void mousePressEvent(QMouseEvent *ev) override
	{
		emit PictureBox_MousePress(ev);
	}

	virtual void mouseReleaseEvent(QMouseEvent *ev) override
	{
		emit PictureBox_MouseRelease(ev);
	}

	virtual void mouseMoveEvent(QMouseEvent *ev) override
	{
		emit PictureBox_MouseMove(ev);
	}

	virtual void paintEvent(QPaintEvent *ev) override
	{
		QPainter p(this);

		p.scale(m_Scale, m_Scale);

		p.drawImage(QPoint(0, 0), m_image);

		p.setPen(m_pen);

		p.drawRects(list_Rect);

		emit PictureBox_Paint(ev);
	}

	QImage m_image;

	double m_Scale = 1.0;

	QVector<QRect> list_Rect;

	QPen m_pen;

signals:

	void PictureBox_MousePress(QMouseEvent *);

	void PictureBox_MouseRelease(QMouseEvent *);

	void PictureBox_MouseMove(QMouseEvent *);

	void PictureBox_Paint(QPaintEvent *);
};

