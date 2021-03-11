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

class GuiFilter;
class GuiRecipe;


class GUI : public QWidget {
private:
	Service& srv;
	QTreeWidget* list = new QTreeWidget;
	
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

	
	void initGUI();
	void initConnect();
	void actualList();
	void listClear();
	void loadList();
	void loadListWith(vector<Medicament> meds);

public:
	friend class GuiFilter;
	friend class GuiRecipe;
	friend class GuiRecipeReadOnly;

	GUI(Service& srv) : srv{ srv } {
		this->setMinimumSize(1280, 600);
		initGUI();
		initConnect();
		loadList();
		//actualList();
	}
};


class GuiExceptionHandler {
	vector<string> errors;
public:
	GuiExceptionHandler() = default;
	GuiExceptionHandler(ValidatorException& ex) : errors{ ex.getErrors() } {}
	GuiExceptionHandler(RepositoryException& ex) : errors { ex.getErrors() } {}
	GuiExceptionHandler(std::ofstream::failure ex) { errors.push_back("File Error! Retry...\n"); }
	GuiExceptionHandler(std::exception) { errors.push_back("Error! Retry...\n"); }
	void showErrors();
};


class GuiFilter : public QWidget {
private:
	GUI& gui;
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
	GuiFilter(GUI& g) : gui{ g }, srv { gui.srv } {
		this->setMinimumSize(300, 400);
		initGuiFilter();
		initConnectFilter();
	}
};


class GuiRecipe : public QWidget, public Observer {
private:
	GUI& gui;
	Service& srv;
	Reteta& rec;
	QTreeWidget* recipe = new QTreeWidget;
	QPushButton* btnExit = new QPushButton{ "Close" };
	
	QPushButton* btnGenerateRecipe = new QPushButton{ "&Genereaza medicamente pe reteta" };
	QPushButton* btnClearRecipe = new QPushButton{ "&Goleste Reteta" };
	QPushButton* btnExportRecipe = new QPushButton{ "&Export reteta" };

	void initGuiRecipe();
	void initRecipeConnect();
	void loadRecipe();
	void update() override {
		loadRecipe();
	}
	
public:
	friend class GUI;
	GuiRecipe(GUI& g) : gui{ g }, srv{ gui.srv }, rec{ srv.getRecipeRef() }{
		recipe->setMinimumSize(600, 400);
		initGuiRecipe();
		initRecipeConnect();
		loadRecipe();
	}
};

class GuiRecipeReadOnly : public QWidget {
private:
	GUI& gui;
	Service& srv;
	Reteta& rec;

	void initGuiRecipeRO();
	void paintEvent(QPaintEvent* ev) override;
public:
	GuiRecipeReadOnly(GUI& g) : gui{ g }, srv{ gui.srv }, rec{ srv.getRecipeRef() }{
		initGuiRecipeRO();
	}
};
