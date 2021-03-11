#pragma once
#include "Service.h"
#include "Observer.h"
#include <fstream>
#include <QtWidgets/QApplication>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qgridlayout.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qtreewidget.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qtableview.h>
#include <QtWidgets/qtreeview.h>
#include <qpainter.h>
#include <qpicture.h>
#include <qvector.h>
#include <qmessagebox.h>
#include <qdebug.h>

class GuiModelFilter;
class GuiModelRecipe;

class MyListModel : public QAbstractTableModel {
	vector<Medicament> meds;
public:
	MyListModel(const vector<Medicament>& meds) : meds{ meds } { }
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	void setMeds(const vector<Medicament>& meds);
};


class GuiModel : public QWidget {
private:
	Service& srv;
	MyListModel* myModel = new MyListModel(srv.getAll());
	MyListModel* recipeModel = new MyListModel(srv.getRecipe());
	QTreeView* list = new QTreeView();


	//main buttons
	QPushButton* btnAdauga = new QPushButton{ "Adauga" };
	QPushButton* btnSterge = new QPushButton{ "Sterge" };
	QPushButton* btnModifica = new QPushButton{ "Modifica" };
	QPushButton* btnExit = new QPushButton{ "Exit" };

	//sort filter
	QCheckBox* boxSorteazaNume = new QCheckBox{ "Sorteaza dupa nume" };
	QCheckBox* boxSorteazaProd = new QCheckBox{ "Sorteaza dupa producator" };
	QCheckBox* boxSorteazaSubst = new QCheckBox{ "Sorteaza dupa substanta activa" };
	QPushButton* btnFiltreaza = new QPushButton{ "Filtreaza" };
	QPushButton* btnStergeFiltre = new QPushButton{ "Sterge Filtre" };

	//recipe stats undo
	QPushButton* btnRecipe = new QPushButton{ "Reteta" };
	QPushButton* btnRecipeReadOnly = new QPushButton{ "Reteta Read Only" };
	QPushButton* btnGenerateRecipe = new QPushButton{ "&Genereaza medicamente pe reteta" };
	QPushButton* btnClearRecipe = new QPushButton{ "&Goleste Reteta" };
	QPushButton* btnExportRecipe = new QPushButton{ "&Export reteta" };
	QPushButton* btnAddToRecipe = new QPushButton{ "Adauga medicament la reteta" };
	QPushButton* btnDeleteFromRecipe = new QPushButton{ "Sterge de pe reteta" };
	QPushButton* btnStats = new QPushButton{ "Statistica" };
	QPushButton* btnUndo = new QPushButton{ "Undo" };

	//form lines edit
	QLineEdit* txtNume = new QLineEdit;
	QLineEdit* txtProd = new QLineEdit;
	QLineEdit* txtSubst = new QLineEdit;
	QLineEdit* txtPret = new QLineEdit;

	std::vector<QPushButton*> btnList;
	QHBoxLayout* btnListLayout = new QHBoxLayout;

	string sortType;

	void initGUI();
	void connectSignals();
	void reloadList(vector<Medicament> meds);
	void actualSortType();
	
public:
	friend class GuiModelFilter;
	friend class GuiModelRecipe;
	friend class GuiModelRecipeReadOnly;
	friend class GuiModelStats;

	GuiModel(Service& srv) : srv{ srv } {
		this->setMinimumSize(1280, 600);
		initGUI();
		connectSignals();
		reloadList(srv.getAll());
		sortType = "0";
	}
};


class GuiModelExceptionHandler {
	vector<string> errors;
public:
	GuiModelExceptionHandler() = default;
	GuiModelExceptionHandler(ValidatorException& ex) : errors{ ex.getErrors() } {}
	GuiModelExceptionHandler(RepositoryException& ex) : errors{ ex.getErrors() } {}
	GuiModelExceptionHandler(std::ofstream::failure ex) { errors.push_back("File Error! Retry...\n"); }
	GuiModelExceptionHandler(std::exception) { errors.push_back("Error! Retry...\n"); }
	void showErrors();
};


class GuiModelFilter : public QWidget {
private:
	GuiModel& gui;
	Service& srv;

	QPushButton* btnFiltreazaNume = new QPushButton{ "Filtreaza dupa nume" };
	QPushButton* btnFiltreazaProd = new QPushButton{ "Filtreaza dupa producator" };
	QPushButton* btnFiltreazaSubst = new QPushButton{ "Filtreaza dupa substanta activa" };
	QPushButton* btnFiltreazaPret = new QPushButton{ "Filtreaza dupa pret" };

	QPushButton* btnClose = new QPushButton{ "Exit" };

	QWidget* readFilterWidget = new QWidget;
	QWidget* readFilterPrices = new QWidget;

	QLineEdit* txtFilter = new QLineEdit;
	QLineEdit* pretMin = new QLineEdit;
	QLineEdit* pretMax = new QLineEdit;
	QPushButton* btnStartFilterNames = new QPushButton{ "Filtreaza" };
	QPushButton* btnStartFilterPrice = new QPushButton{ "Filtreaza" };

	string type;

	void initGuiFilter();
	void initConnectFilter();
public:
	GuiModelFilter(GuiModel& g) : gui{ g }, srv{ gui.srv } {
		this->setMinimumSize(300, 400);
		initGuiFilter();
		initConnectFilter();
	}
};


class GuiModelRecipe : public QWidget, public Observer {
private:
	GuiModel& gui;
	Service& srv;
	Reteta& rec;
	MyListModel* recipeModel;
	QTreeView* recipe = new QTreeView();

	QPushButton* btnGenerateRecipe = new QPushButton{ "&Genereaza medicamente pe reteta" };
	QPushButton* btnClearRecipe = new QPushButton{ "&Goleste Reteta" };
	QPushButton* btnExportRecipe = new QPushButton{ "&Export reteta" };
	QPushButton* btnExit = new QPushButton{ "Close" };

	void initGuiRecipe();
	void initRecipeConnect();
	void loadRecipe();
	void update() {
		loadRecipe();
	}
	void updateSrv() {
		
	}

public:
	friend class GuiModel;
	GuiModelRecipe(GuiModel& g) : gui{ g }, srv{ gui.srv }, recipeModel{ gui.recipeModel }, rec{ srv.getRecipeRef() }{
		recipe->setMinimumSize(600, 400);
		initGuiRecipe();
		initRecipeConnect();
		loadRecipe();
	}
};


class GuiModelRecipeReadOnly : public QWidget {
private:
	GuiModel& gui;
	Service& srv;

	void initGuiRecipeRO();
	void paintEvent(QPaintEvent* ev) override;
public:
	GuiModelRecipeReadOnly(GuiModel& g) : gui{ g }, srv{ gui.srv } {
		initGuiRecipeRO();
	}
};

class GuiModelStats : public QWidget, public Observer {
private:
	GuiModel& gui;
	Service& srv;
	
	QTreeWidget* statsList = new QTreeWidget;
	
	void initStats();
	void connectStats();
	void loadStats();
	void update() {
		loadStats();
	}
public:
	GuiModelStats(GuiModel& g) : gui { g }, srv{ gui.srv } {
		initStats();
		connectStats();
		loadStats();
	}
};