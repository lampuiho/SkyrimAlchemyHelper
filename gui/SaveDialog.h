#pragma once

#include <QDialog>

#include "GameSave.h"

class QWidget;
class QLineEdit;
class InventoryWidget;
class KnownIngredientsWidget;
class ContainersWidget;

class SaveDialog : public QDialog
{
	Q_OBJECT

public:
	SaveDialog(QWidget *parent = 0);

	QSize sizeHint() const;
	bool modified() const;

public slots:
	void loadSave();
	void copySave();
	void saveSelected(int index);
	void setLoadMostRecent(int state);

protected:
	void refreshInformation();

	QWidget* m_saveInfoContainer;
	InventoryWidget* m_inventoryWidget;
	KnownIngredientsWidget* m_knownIngredientsWidget;
	ContainersWidget* m_containersWidget;
	GameSave m_gameSave;
	QFileInfoList m_savesList;
	bool m_modified = false, m_loadMostRecent;
	QString m_selectedSavePath;
	QLineEdit *m_maxValidIngredientCountEdit, *m_minValidNbIngredientsEdit;
};
