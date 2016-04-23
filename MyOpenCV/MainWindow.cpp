#include <stdafx.h>
#include "MainWindow.h"
#include <vector>


using namespace cv;
using namespace std;

MainWindow::MainWindow()
{

	SetUpUI();

	InitConnection();
}


static CvHaarClassifierCascade* cascade = nullptr;


void MainWindow::SetUpUI()
{
	resize(1024, 768);

	setWindowState(Qt::WindowMaximized);
	setAutoFillBackground(true);
	setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));

	maingrid = new QGridLayout(this);

	maingrid->cellRect(0, 1);

	maingrid->setColumnStretch(0, 1000);

	maingrid->setColumnStretch(1, 1);

	maingrid->setColumnMinimumWidth(1, 420);

	/////////////////// 添加控件 /////////////////////

	mainPicbox = new PictureBox(this);

	maingrid->addWidget(mainPicbox, 0, 0);

	groupbox = new QGroupBox("Setting");
	groupbox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	groupbox->setBaseSize(420, 955);
	groupbox->setMinimumHeight(950);

	maingrid->addWidget(groupbox, 0, 1, Qt::AlignTop);

#pragma region 右侧控件

	btnOpenImage = new QPushButton("Load Image", groupbox);

	btnOpenImage->setGeometry(20, 50, 120, 35);


	btnToGray = new QPushButton("CvtGray", groupbox);

	btnToGray->setGeometry(150, 50, 120, 35);


	btnToSrc = new QPushButton("Back To Src", groupbox);

	btnToSrc->setGeometry(280, 50, 120, 35);


	btnMatch = new QPushButton("Match", groupbox);

	btnMatch->setGeometry(20, 100, 120, 35);

	btnShowSrc = new QPushButton("Src", groupbox);
	btnShowSrc->setGeometry(280, 100, 60, 35);

	btnOCR = new QPushButton("OCR", groupbox);
	btnOCR->setGeometry(160, 280, 90, 35);

	btnShowSrc_Copy = new QPushButton("Src_Copy", groupbox);
	btnShowSrc_Copy->setGeometry(340, 100, 60, 35);

	comboxMatchTemplate = new QComboBox(groupbox);
	comboxMatchTemplate->setGeometry(150, 100, 120, 35);

	comboxMatchTemplate->addItem("CV_TM_SQDIFF");
	comboxMatchTemplate->addItem("CV_TM_CCORR");
	comboxMatchTemplate->addItem("CV_TM_CCOEFF");
	comboxMatchTemplate->addItem("CV_TM_SQDIFF_NORMED");
	comboxMatchTemplate->addItem("CV_TM_CCORR_NORMED");
	comboxMatchTemplate->addItem("CV_TM_CCOEFF_NORMED");
	comboxMatchTemplate->setStyleSheet(QLatin1String("selection-background-color: rgb(73, 85, 255);\n"
		"selection-color: rgb(255, 255, 255);"));

	m_map_MatchTemplate.insert("CV_TM_SQDIFF", CV_TM_SQDIFF);
	m_map_MatchTemplate.insert("CV_TM_CCORR", CV_TM_CCORR);
	m_map_MatchTemplate.insert("CV_TM_CCOEFF", CV_TM_CCOEFF);
	m_map_MatchTemplate.insert("CV_TM_SQDIFF_NORMED", CV_TM_SQDIFF_NORMED);
	m_map_MatchTemplate.insert("CV_TM_CCORR_NORMED", CV_TM_CCORR_NORMED);
	m_map_MatchTemplate.insert("CV_TM_CCOEFF_NORMED", CV_TM_CCOEFF_NORMED);


	scaleSlider = new QSlider(Qt::Horizontal, groupbox);
	scaleSlider->setMaximum(50);
	scaleSlider->setGeometry(80, 160, 300, 30);
	scaleSlider->setValue(10);

	auto labelSider = new QLabel("Scale:", groupbox);
	labelSider->setGeometry(20, 160, 60, 30);

	sliderLabel = new QLabel(groupbox);
	sliderLabel->setGeometry(385, 160, 30, 30);
	sliderLabel->setText(QString("%1").arg(scaleSlider->value() / 10.0));

	checkboxfloodFill = new QCheckBox("Pick Point", groupbox);
	checkboxfloodFill->setGeometry(20, 280, 120, 35);

#pragma region Threshold

	checkboxThreshold = new QCheckBox("Threshold", groupbox);

	checkboxThreshold->setGeometry(20, 200, 120, 30);

	sliderThreshold = new QSlider(groupbox);

	sliderThreshold->setGeometry(150, 200, 240, 30);
	sliderThreshold->setOrientation(Qt::Horizontal);
	sliderThreshold->setMaximum(255);

	sliderThreshold->setMinimum(0);

	comboxThreshold = new QComboBox(groupbox);
	comboxThreshold->setGeometry(20, 240, 200, 30);
	comboxThreshold->addItem("CV_THRESH_BINARY");
	comboxThreshold->addItem("CV_THRESH_BINARY_INV");
	comboxThreshold->addItem("CV_THRESH_TRUNC");
	comboxThreshold->addItem("CV_THRESH_TOZERO");
	comboxThreshold->addItem("CV_THRESH_TOZERO_INV");
	comboxThreshold->setStyleSheet(QLatin1String("selection-background-color: rgb(73, 85, 255);\n"
		"selection-color: rgb(255, 255, 255);"));

	m_map_threshold.insert("CV_THRESH_BINARY", CV_THRESH_BINARY);
	m_map_threshold.insert("CV_THRESH_BINARY_INV", CV_THRESH_BINARY_INV);
	m_map_threshold.insert("CV_THRESH_TRUNC", CV_THRESH_TRUNC);
	m_map_threshold.insert("CV_THRESH_TOZERO", CV_THRESH_TOZERO);
	m_map_threshold.insert("CV_THRESH_TOZERO_INV", CV_THRESH_TOZERO_INV);


#pragma endregion Threshold


#pragma region 相机取像

	groupboxCamera = new QGroupBox("Camera", groupbox);

	groupboxCamera->setGeometry(10, 420, 400, 210);

	btnCamera = new QPushButton("Start Camera", groupboxCamera);

	btnCamera->setGeometry(20, 40, 120, 35);

	checkboxGray = new QCheckBox("Gray", groupboxCamera);

	checkboxGray->setGeometry(20, 170, 120, 30);

	checkboxHorFlip = new QCheckBox("Hor Filp", groupboxCamera);

	checkboxHorFlip->setGeometry(100, 170, 120, 30);

	checkboxVerFlip = new QCheckBox("Ver Filp", groupboxCamera);

	checkboxVerFlip->setGeometry(210, 170, 120, 30);

	checkboxDetction = new QCheckBox("Detection", groupboxCamera);

	checkboxDetction->setGeometry(20, 90, 100, 30);

	comboxDetctionFiles = new QComboBox(groupboxCamera);
	comboxDetctionFiles->setGeometry(20, 130, 230, 30);
	comboxDetctionFiles->setStyleSheet(QLatin1String("selection-background-color: rgb(73, 85, 255);\n"
		"selection-color: rgb(255, 255, 255);"));


	for each (auto var in GetFileList("D:\\Program Files\\opencv\\sources\\data"))
	{
		if (var.fileName().toLower().contains(".xml"))
		{
			m_map_cascade.insert(var.fileName(), var.absoluteFilePath());
			comboxDetctionFiles->addItem(var.fileName());
		}

	}

	labelDetectInfo = new QTextEdit(groupboxCamera);
	labelDetectInfo->setGeometry(160, 40, 200, 60);
	labelDetectInfo->setText("Nothing Here");


	btnTestFace = new QPushButton("Detect", groupboxCamera);
	btnTestFace->setGeometry(260, 130, 120, 30);

#pragma endregion 相机取像



#pragma region ROI

	pen.setColor(QColor(0, 255, 0));
	pen.setWidth(2);

	roigroupbox = new QGroupBox("ROI Setting", groupbox);

	roigroupbox->setGeometry(10, 640, 400, 300);

	roiLabel = new QLabel(roigroupbox);

	roiLabel->setGeometry(10, 37, 250, 250);

	roiLabel->setPixmap(QPixmap());

	roiLabel->setScaledContents(true);

	roiInfoLabe = new QLabel(roigroupbox);

	roiInfoLabe->setGeometry(280, 37, 110, 30);

	roiInfoLabe->setText("Nothing Here");


	roibtnStart = new QPushButton("Start ROI", roigroupbox);
	roibtnStart->setGeometry(285, 80, 100, 30);

	roibtnStop = new QPushButton("Stop ROI", roigroupbox);
	roibtnStop->setEnabled(false);
	roibtnStop->setGeometry(285, 130, 100, 30);

	roibtnReset = new QPushButton("Reset ROI", roigroupbox);
	roibtnReset->setGeometry(285, 180, 100, 30);

	checkboxShowRect = new QCheckBox("Show Rect", roigroupbox);
	checkboxShowRect->setGeometry(280, 230, 110, 30);
	checkboxShowRect->setChecked(true);

	maingrid->addWidget(groupbox, 0, 1, Qt::AlignBottom);

#pragma endregion ROI


#pragma endregion 右侧控件

}



void MainWindow::InitConnection()
{

	connect(btnOpenImage, &QPushButton::clicked, this, [&]
	{
		QFileDialog file(this, "Open Image File", "D:\\", "*.jpg;*.png;*.bmp;");

		file.exec();

		if (file.selectedFiles().count() == 0) return;

		if (file.selectedFiles().count() == 1)
		{
			if (QFile::exists(file.selectedFiles()[0]))
			{
				src = imread(cv::String(file.selectedFiles()[0].toStdString()));
				src_copy = src.clone();
				mainPicbox->SetImage(cvMat2QImage(src));
				return;
			}
		}
		MSG("Error", "Open Image File Failed");
	});


	connect(btnToGray, &QPushButton::clicked, this, [&]
	{
		if (src.empty())
		{
			MSG("Error", "Source Image is Empty!");
		}
		else
		{
			if (1 == src.channels())
			{

			}
			else if (3 == src.channels())
			{

				cvtColor(src_copy, src_copy, CV_BGR2GRAY);
				src = src_copy.clone();
				mainPicbox->SetImage(src_copy);
			}
			else
			{
				auto temp = "src.channels is " + QSTRING(src.channels());
				MSG("Warning!", temp);
			}


		}
	});


	connect(btnToSrc, &QPushButton::clicked, this, [&]
	{
		src = src_copy;
		mainPicbox->SetImage(src_copy);
	});


	connect(btnCamera, &QPushButton::clicked, this, [&]
	{
		if (cameraTimer.isActive() || cap.isOpened())
		{
			cap.release();

			cameraTimer.stop();

			btnCamera->setText("Start Camera");

			src_copy = src.clone();
		}
		else
		{
			cameraTimer.setInterval(200);

			cap = VideoCapture(0);

			btnCamera->setText("Stop Camera");

			cascade = (CvHaarClassifierCascade*)cvLoad(m_map_cascade.take(comboxDetctionFiles->currentText()).toStdString().data());

			cameraTimer.start();

		}

	});


	connect(scaleSlider, &QSlider::valueChanged, this, [&](int value)
	{
		double d = value / 10.0;
		mainPicbox->SetScale(d);
		QToolTip::showText(QPoint(QCursor::pos().x() - 16, scaleSlider->y() - 5), QString("%1").arg(d));
		sliderLabel->setText(QString("%1").arg(d));
	});


	//////////////////////////////////////////////////////////////////////////

	connect(roibtnStart, &QPushButton::clicked, this, [&]
	{
		roiflag = true;
		roibtnStart->setEnabled(!roiflag);
		roibtnStop->setEnabled(roiflag);
	});


	connect(roibtnStop, &QPushButton::clicked, this, [&]
	{
		roiflag = false;
		roibtnStart->setEnabled(!roiflag);
		roibtnStop->setEnabled(roiflag);
	});


	connect(mainPicbox, &PictureBox::PictureBox_MousePress, this, [&](QMouseEvent *ev)
	{
		if (roiflag)
		{
			roiMoueDownPoint = ev->pos();

		}
		IsMouseDown = true;
	});


	connect(mainPicbox, &PictureBox::PictureBox_MouseMove, this, [&](QMouseEvent *ev)
	{
		if (roiflag && IsMouseDown)
		{
			auto s = 1 / mainPicbox->m_Scale;

			auto p1 = roiMoueDownPoint;
			auto p2 = ev->pos();
			auto r1 = QPoint(qMin<int>(p1.x(), p2.x()), qMin<int>(p1.y(), p2.y()));
			auto r2 = QPoint(qMax<int>(p1.x(), p2.x()), qMax<int>(p1.y(), p2.y()));
			roiRect = QRect(QPoint(r1.x()*s, r1.y()*s), QPoint(r2.x()*s, r2.y()*s));

			roiInfoLabe->setText(QString("%1 x %2").arg(roiRect.width()).arg(roiRect.height()));
			mainPicbox->update();
		}
	});


	connect(mainPicbox, &PictureBox::PictureBox_MouseRelease, this, [&](QMouseEvent *ev)
	{
		IsMouseDown = false;

		if (roiflag)
		{

			cv::Rect rect(roiRect.left(), roiRect.top(), roiRect.width(), roiRect.height());
			if (rect.x > 0 && rect.y > 0 && rect.width < src.cols && rect.height < src.rows)
			{
				roi = src(rect);

				QImage temp_image = cvMat2QImage(roi);
				roiLabel->setPixmap(QPixmap::fromImage(temp_image));

				roiInfoLabe->setText(QString("%1 x %2").arg(roi.cols).arg(roi.rows));

			}

		}

		if (checkboxfloodFill->isChecked())
		{
			Rect comm;

			floodFill(src, Point(qRound(ev->x()*mainPicbox->m_Scale), qRound(ev->y()*mainPicbox->m_Scale)), Scalar(0, 0, 255), &comm, Scalar(0, 0, 0), Scalar(20, 20, 20));

			mainPicbox->SetImage(src);
		}

	});


	connect(mainPicbox, &PictureBox::PictureBox_Paint, this, [&](QPaintEvent *ev)
	{
		if (roiflag && checkboxShowRect->isChecked())
		{
			QPainter p(mainPicbox);

			p.setPen(pen);

			p.drawRect(roiRect.x()*mainPicbox->m_Scale,
				roiRect.y()*mainPicbox->m_Scale,
				roiRect.width()*mainPicbox->m_Scale,
				roiRect.height()*mainPicbox->m_Scale);

		}

	});


	connect(checkboxShowRect, &QCheckBox::clicked, this, [&]
	{
		mainPicbox->update();
	});


	connect(roibtnReset, &QPushButton::clicked, this, [&]
	{
		roi = Mat();
		roiInfoLabe->setText("Nothing Here");
		roiRect = QRect();
		roiMoueDownPoint = QPoint();
		IsMouseDown = false;
		roiflag = false;
		roibtnStart->setEnabled(true);
		roiLabel->setPixmap(QPixmap());
		roibtnStop->setEnabled(false);
		mainPicbox->update();
	});


	//相机
	connect(&cameraTimer, &QTimer::timeout, this, [&]
	{
		if (!cap.isOpened())
		{
			MSG("Error", "Camera Open Failed");

			btnCamera->setText("Start Camera");

			cameraTimer.stop();
		}

		Mat frame;

		cap >> frame;



		if (checkboxGray->isChecked())
		{
			if (3 == frame.channels())
			{
				cvtColor(frame, frame, CV_BGR2GRAY);
			}
		}

		if (checkboxHorFlip->isChecked() && checkboxVerFlip->isChecked())
		{
			flip(frame, frame, 0);
		}
		else if (checkboxHorFlip->isChecked())
		{
			flip(frame, frame, 1);
		}
		else if (checkboxVerFlip->isChecked())
		{
			flip(frame, frame, -1);
		}

		if (checkboxDetction->isChecked())
		{
			DetectFace(frame, cascade, labelDetectInfo);
		}

		mainPicbox->SetImage(frame);

		src = frame;

	});


	connect(btnMatch, &QPushButton::clicked, this, [&]
	{
		mainPicbox->ClearRect();

		if (src.empty() || roi.empty())
		{
			MSG("Error", "Src or Roi is Empty");
			return;
		}

		if (src.channels() != roi.channels())
		{
			MSG("Error", "Src.channels is not equal to Roi.channels");
			return;
		}

		Stopwatch sw;

		sw.Start();

		Mat result;

		auto wid = roi.cols;

		auto hei = roi.rows;

		matchTemplate(src, roi, result, m_map_MatchTemplate.take(comboxMatchTemplate->currentText()));

		Point minLoc(0, 0);
		Point maxLoc(0, 0);
		double minVal = 0;
		double maxVal = 0;

		minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

		mainPicbox->list_Rect << QRect(minLoc.x, minLoc.y, wid, hei) << QRect(maxLoc.x, maxLoc.y, wid, hei);

		mainPicbox->update();

		qDebug() << sw.Stop();

		waitKey();

	});


	connect(sliderThreshold, (void (QSlider::*)(int))&QSlider::valueChanged, this, [&](int  value)
	{
		if (checkboxThreshold->isChecked())
		{
			qDebug() << value;
			threshold(src_copy, src, value, 255, m_map_threshold.take(comboxThreshold->currentText()));
			mainPicbox->m_image = cvMat2QImage(src);
			mainPicbox->update();
		}

	});


	connect(btnShowSrc, &QPushButton::clicked, this, [&]
	{
		imshow("Src", src);
	});


	connect(btnShowSrc_Copy, &QPushButton::clicked, this, [&]
	{
		imshow("Src_copy", src_copy);
	});


	connect(btnTestFace, &QPushButton::clicked, this, [&]
	{

		cascade = (CvHaarClassifierCascade*)cvLoad(m_map_cascade.take(comboxDetctionFiles->currentText()).toStdString().data());
		mainPicbox->SetImage(DetectFace(src, cascade, labelDetectInfo));

	});


	connect(btnOCR, &QPushButton::clicked, this, [&]
	{
		Mat src_1 = src.clone();

		if (src_1.channels() != 1)
		{
			cvtColor(src_1, src_1, CV_RGB2GRAY);
		}

		threshold(src_1, src_1, 128, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);


		vector<vector<Point>> contours;

		findContours(src_1, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		//drawContours(src, contours, -1, Scalar(0, 255, 0), 1);


		for each (auto item in contours)
		{
			InputArray p(item);
			RotatedRect rect = minAreaRect(p);
			rectangle(src, rect.boundingRect(), CvScalar(0, 255, 0));
		}

		mainPicbox->SetImage(src);
	});
}




MainWindow::~MainWindow()
{
	cameraTimer.stop();
}
