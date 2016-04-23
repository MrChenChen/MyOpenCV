#pragma once
#include <stdafx.h>
#include <PictureBox.h>

#define MSG(title,content) QMessageBox::about(0,QString("%1").arg(title),QString("%1").arg(content))

#define QSTRING(content)  QString("%1").arg(content);


class MainWindow : public QWidget
{
	Q_OBJECT

public:


	MainWindow();

	void SetUpUI();

	void InitConnection();

	~MainWindow();

	////////////////////////////////////////


	QGridLayout *maingrid;

	QGroupBox *groupbox;



	PictureBox *mainPicbox;

	Mat src;

	Mat src_copy;

	Mat roi;


	QTimer cameraTimer;

	VideoCapture cap;
	///////////右侧控件///////////

	QPushButton *btnOpenImage;

	QPushButton *btnToGray;

	QPushButton *btnToSrc;

	QPushButton *btnMatch;

	QPushButton *btnShowSrc;

	QPushButton *btnShowSrc_Copy;

	QPushButton *btnOCR = nullptr;

	QComboBox *comboxMatchTemplate;

	QMap<QString, int> m_map_MatchTemplate;

	QSlider *scaleSlider;

	QLabel *sliderLabel;

	/////////////// Threshold ///////////////////

	QCheckBox *checkboxThreshold;

	QSlider *sliderThreshold;

	QComboBox *comboxThreshold;

	QMap<QString, int> m_map_threshold;

	QCheckBox *checkboxfloodFill;

#pragma region 画框相关

	QGroupBox *roigroupbox;

	QLabel *roiLabel;

	QLabel *roiInfoLabe;

	QPushButton *roibtnStart;

	QPushButton *roibtnStop;

	QPushButton *roibtnReset;

	QCheckBox *checkboxShowRect;

	bool roiflag = false;

	bool IsMouseDown = false;

	QPoint roiMoueDownPoint;

	QRect roiRect;

	QPen pen;

#pragma endregion 画框相关


#pragma region 相机相关

	QPushButton *btnCamera;

	QGroupBox *groupboxCamera;

	QCheckBox *checkboxGray;

	QCheckBox *checkboxHorFlip;

	QCheckBox *checkboxVerFlip;

	QCheckBox *checkboxDetction;

	QComboBox *comboxDetctionFiles;

	QTextEdit *labelDetectInfo;

	QMap<QString, QString> m_map_cascade;

	QPushButton *btnTestFace;

#pragma endregion 相机相关


};

