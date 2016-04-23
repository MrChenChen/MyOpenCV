#include <stdafx.h>
#include "GlobalFunction.h"

using namespace cv;
using namespace std;


QImage  cvMat2QImage(const cv::Mat & mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		qDebug() << "CV_8UC4";
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}


cv::Mat QImage2cvMat(QImage & image)
{
	cv::Mat mat;
	qDebug() << image.format();
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}


QFileInfoList GetFileList(QString path)
{
	QFileInfoList allFiles;
	QDir rootDir(path);
	allFiles.append(rootDir.entryInfoList(QDir::Files));

	for each (auto item in rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot))
	{

		allFiles.append(GetFileList(item.absoluteFilePath()));
	}

	return allFiles;
}


Mat DetectFace(Mat & src, CvHaarClassifierCascade* cascade, QTextEdit* label)
{
	Stopwatch sw;
	sw.Start();

	if (cascade)
	{
		CvMemStorage* storage = 0;
		storage = cvCreateMemStorage(0);
		cvClearMemStorage(storage);

		auto temp_mat = Mat(CvSize(src.rows, src.cols), 8, 1);

		if (src.channels() != 1)
		{
			cvtColor(src, temp_mat, CV_BGR2GRAY);
		}

		cvResize(&(IplImage)temp_mat, &(IplImage)temp_mat, CV_INTER_LINEAR);

		equalizeHist(temp_mat, temp_mat);

		if (cascade)
		{
			CvSeq* faces = cvHaarDetectObjects(&(IplImage)temp_mat, cascade, storage, 1.5, 3, 0
				//| CV_HAAR_FIND_BIGGEST_OBJECT
				| CV_HAAR_DO_ROUGH_SEARCH
				| CV_HAAR_SCALE_IMAGE
				,
				Size(30, 30));

			Rect maxrect;

			for (size_t i = 0; i < faces->total; i++)
			{
				Rect* maxrect = (Rect*)getSeqElem(faces, i);

				rectangle(src, CvPoint(maxrect->x, maxrect->y), CvPoint((maxrect->x + maxrect->width), (maxrect->y + maxrect->height)), CvScalar(0, 255, 0), 2);

			}

			label->setText(QString("Time : %1 \r\nCount : %2").arg(sw.Stop()).arg(faces->total));

		}

	}
	else
	{
		qDebug() << "cascade is null";
	}


	return src;
}

cv::Mat Reverse(Mat src)
{
	Mat dst = src < 100;

	return dst;
}

