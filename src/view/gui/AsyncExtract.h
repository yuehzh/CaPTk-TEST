

#include <QObject>
#include <QThread>
#include <QFile>

#include "CaPTkUtils.h"
#include "CaPTkGUIUtils.h"
#include "QZipReader.h"
#include "ApplicationPreferences.h"

class ASyncExtract : public QThread
{
	Q_OBJECT
	void run() override {
		ApplicationPreferences::GetInstance()->SetLibraExtractionStartedStatus(QVariant("true").toString());
		ApplicationPreferences::GetInstance()->SerializePreferences();
		ApplicationPreferences::GetInstance()->DisplayPreferences();

		if (QFile::exists(this->fullPath))
		{
			QZipReader zr(this->fullPath);
			bool ret = zr.extractAll(this->extractPath);

			if(ret)
			{
				//after extraction remove the zip
				bool successfullyremoved = QFile::remove(this->fullPath.toStdString().c_str());
			}
		}

		qDebug() << "Extraction done in background" << this->fullPath << endl;

		//serialize only once
		ApplicationPreferences::GetInstance()->SerializePreferences();

		emit resultReady(this->appName);
	}	

public:
	ASyncExtract() = default;
	~ASyncExtract() = default;

	void setFullPath(QString fullPath) {
		this->fullPath = fullPath;
	}

	void setExtractPath(QString extractPath) {
		this->extractPath = extractPath;
	}

	void setAppName(QString appName) {
		this->appName = appName;
	}

private:
	Q_DISABLE_COPY(ASyncExtract)

	QString fullPath;
	QString extractPath;
	QString appName;

signals:
    void resultReady(QString appName);
};

