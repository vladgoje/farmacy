#include "GUI.h"
#include <sstream>
#include <qpaintengine.h>

void GUI::initGUI() {
	this->setWindowTitle("Application");
	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->setMargin(30);
	setLayout(mainLayout);

	//top left layout
	auto topLayout = new QHBoxLayout(nullptr);
	mainLayout->addLayout(topLayout);
	//form
	auto topLeftLayout = new QVBoxLayout;
	auto formLayout = new QFormLayout;
	formLayout->addRow("Nume", txtNume);
	formLayout->addRow("Producator", txtProd);
	formLayout->addRow("Substanta activa", txtSubst);
	formLayout->addRow("Pret", txtPret);
	formLayout->setMargin(20);
	topLeftLayout->addLayout(formLayout);
	//top left buttons
	auto topLeftBtnsLayout = new QHBoxLayout;
	topLeftBtnsLayout->addWidget(btnAdauga);
	topLeftBtnsLayout->addWidget(btnModifica);
	topLeftBtnsLayout->addWidget(btnSterge);
	topLeftLayout->addLayout(topLeftBtnsLayout);
	
	//list
	list->setColumnCount(4);
	const QStringList headerList = { "Nume", "Producator", "Substanta activa", "Pret" };
	list->setHeaderLabels(headerList);
	topLeftLayout->addWidget(list);


	topLayout->addLayout(topLeftLayout);

	//top right layout
	auto topRightLayout = new QVBoxLayout;
	auto topRightOptions = new QGridLayout;
	topRightOptions->addWidget(new QLabel("Farmacie:"), 0, 0);
	topRightOptions->addWidget(new QLabel("Reteta:"), 0, 1);
	topRightOptions->addWidget(boxSorteazaNume, 1, 0);
	topRightOptions->addWidget(boxSorteazaProd, 2, 0);
	topRightOptions->addWidget(boxSorteazaSubst, 3, 0);
	topRightOptions->addWidget(btnFiltreaza, 4, 0);
	topRightOptions->addWidget(btnStergeFiltre, 5, 0);
	topRightOptions->addWidget(btnStats, 6, 0);
	topRightOptions->addWidget(btnUndo, 7, 0);
	topRightOptions->addWidget(btnRecipe, 1, 1);
	topRightOptions->addWidget(btnRecipeReadOnly, 2, 1);
	topRightOptions->addWidget(btnAddToRecipe, 3, 1);
	topRightOptions->addWidget(btnDeleteFromRecipe, 4, 1);
	topRightOptions->addWidget(btnGenerateRecipe, 5, 1);
	topRightOptions->addWidget(btnClearRecipe, 6, 1);
	topRightOptions->addWidget(btnExportRecipe, 7, 1);

	topRightOptions->setMargin(10);
	
	auto topRightBottom = new QHBoxLayout;
	topRightBottom->addWidget(btnExit);

	topRightLayout->addLayout(topRightOptions);
	topRightLayout->addLayout(topRightBottom);
	topLayout->addLayout(topRightLayout);

	btnListLayout->setMargin(30);
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(btnListLayout);
}


void GUI::actualList() {
	listClear();
	QTreeWidgetItemIterator it(list);
	while (*it) {
		auto btn = new QPushButton((*it)->text(0));
		btnList.push_back(btn);
		auto name = (*it)->text(0);
		QObject::connect(btn, &QPushButton::clicked, [&, name]() {
			srv.sterge(name.toStdString());
			loadList();
			});
		
		++it;
	}
	for (auto btn : btnList) {
		btnListLayout->addWidget(btn);
	}
}


void GUI::listClear() {
	QLayoutItem* item;
	while ((item = btnListLayout->takeAt(0)) != NULL)
	{
		delete item->widget();
		delete item;
	}
	btnList.clear();
}



void GUI::loadList() {
	list->clear();
	vector<Medicament> meds = srv.getAll();
	for (const auto& med : meds){
		//string details = med.getName() + " " + med.getProd() + " " + med.getSubst() + " " + std::to_string(med.getPrice());
		QTreeWidgetItem* item = new QTreeWidgetItem(list);
		item->setText(0, QString::fromStdString(med.getName()));
		item->setText(1, QString::fromStdString(med.getProd()));
		item->setText(2, QString::fromStdString(med.getSubst()));
		item->setText(3, QString::fromStdString(std::to_string(med.getPrice())));
		list->addTopLevelItem(item);
	}
	//actualList(); // LISTA BUTOANE DINAMICE
}

void GUI::loadListWith(vector<Medicament> meds) {
	list->clear();
	for (const auto& med : meds) {
		QTreeWidgetItem* item = new QTreeWidgetItem(list);
		item->setText(0, QString::fromStdString(med.getName()));
		item->setText(1, QString::fromStdString(med.getProd()));
		item->setText(2, QString::fromStdString(med.getSubst()));
		item->setText(3, QString::fromStdString(std::to_string(med.getPrice())));
		list->addTopLevelItem(item);
	}
	//actualList();
}


void GUI::initConnect() {
	//adauga
	QObject::connect(btnAdauga, &QPushButton::clicked, [&]() {
		auto nume = txtNume->text();
		auto prod = txtProd->text();
		auto subst = txtSubst->text();
		auto pret = txtPret->text();
		try {
			srv.adauga(nume.toStdString(), prod.toStdString(), subst.toStdString(), pret.toStdString());
			loadList();			
		}
		catch (ValidatorException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		catch (RepositoryException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		});

	//sterge
	QObject::connect(btnSterge, &QPushButton::clicked, [&]() {
		auto selectedItems = list->selectedItems();
		if (selectedItems.size() != 1) {
			QMessageBox::warning(nullptr, "Warning!", "Invalid selection!");
		}
		else {
			auto selectedItem = selectedItems.front();
			auto nume = selectedItem->text(0);
			try {
				srv.sterge(nume.toStdString());
				loadList();
			}
			catch (ValidatorException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
			catch (RepositoryException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		}
		});

	//modifica
	QObject::connect(btnModifica, &QPushButton::clicked, [&]() {
		auto selectedItems = list->selectedItems();
		if (selectedItems.size() != 1) {
			QMessageBox::warning(nullptr, "Warning!", "Invalid selection!");
		}
		else {
			auto selectedItem = selectedItems.front();
			auto _nume = selectedItem->text(0);
			auto nume = txtNume->text();
			auto prod = txtProd->text();
			auto subst = txtSubst->text();
			auto pret = txtPret->text();
			try {
				srv.modifica(_nume.toStdString(), nume.toStdString(), prod.toStdString(), subst.toStdString(), pret.toStdString());
				loadList();
			}
			catch (ValidatorException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
			catch (RepositoryException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		}
		});


	QObject::connect(btnFiltreaza, &QPushButton::clicked, [&]() {
		auto filterWidget = new GuiFilter(*this);
		filterWidget->show();
		});

	QObject::connect(btnStergeFiltre, &QPushButton::clicked, [&]() {
		loadList();
		});


	//sortari

	QObject::connect(boxSorteazaNume, &QCheckBox::clicked, [&]() {
		if (boxSorteazaNume->isChecked()){
			loadListWith(srv.sorteaza("1"));
			boxSorteazaProd->setCheckState(Qt::Unchecked);
			boxSorteazaSubst->setCheckState(Qt::Unchecked);
		}
		else {
			loadList();
		}
		});

	QObject::connect(boxSorteazaProd, &QCheckBox::clicked, [&]() {
		if (boxSorteazaProd->isChecked()) {
			loadListWith(srv.sorteaza("2"));
			boxSorteazaNume->setCheckState(Qt::Unchecked);
			boxSorteazaSubst->setCheckState(Qt::Unchecked);
		}
		else {
			loadList();
		}
		
		});

	QObject::connect(boxSorteazaSubst, &QCheckBox::clicked, [&]() {
		if(boxSorteazaSubst->isChecked()){
			loadListWith(srv.sorteaza("3"));
			boxSorteazaNume->setCheckState(Qt::Unchecked);
			boxSorteazaProd->setCheckState(Qt::Unchecked);
		}
		else {
			loadList();
		}
		});

	QObject::connect(btnRecipe, &QPushButton::clicked, [&]() {
		auto recipe = new GuiRecipe(*this);
		recipe->show();
		});

	QObject::connect(btnRecipeReadOnly, &QPushButton::clicked, [&]() {
		auto recipe = new GuiRecipeReadOnly(*this);
		recipe->show();
		});

	QObject::connect(btnStats, &QPushButton::clicked, [&]() {
		QWidget* statsWidget = new QWidget;
		QVBoxLayout* statsLayout = new QVBoxLayout;
		QTreeWidget* statsList = new QTreeWidget;
		const QStringList header{ "Producator", "Procent" };

		statsList->setHeaderLabels(header);
		statsList->setColumnCount(2);
		statsLayout->addWidget(statsList);
		statsWidget->setLayout(statsLayout);
		statsWidget->setMinimumSize(400, 300);

		auto statsDTOs = srv.stats();
		for (auto s : statsDTOs) {
			QTreeWidgetItem* item = new QTreeWidgetItem(statsList);
			item->setText(0, QString::fromStdString(s.getTip()));
			item->setText(1, QString::fromStdString(std::to_string(s.getNr())) + " %");
		}
 		statsWidget->show();
		});

	QObject::connect(btnAddToRecipe, &QPushButton::clicked, [&]() {
		auto selectedItems = list->selectedItems();
		if (selectedItems.size() != 1) {
			QMessageBox::warning(nullptr, "Warning", "Invalid selection!");
		}
		else {
			QString name = selectedItems.front()->text(0);
			try {
				srv.addOnRecipe(name.toStdString());
			}
			catch (RepositoryException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		}
		});

	QObject::connect(btnDeleteFromRecipe, &QPushButton::clicked, [&]() {
		auto selectedItems = list->selectedItems();
		if (selectedItems.size() != 1) {
			QMessageBox::warning(nullptr, "Warning", "Invalid selection!");
		}
		else {
			QString name = selectedItems.front()->text(0);
			try {
				srv.deleteFromRecipe(name.toStdString());
			}
			catch (RepositoryException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		}
		});

	QObject::connect(btnClearRecipe, &QPushButton::clicked, [&]() {
		try {
			srv.clearRecipe();
		}
		catch (RepositoryException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		});


	QObject::connect(btnGenerateRecipe, &QPushButton::clicked, [&]() {
		QWidget* numberInputWidget = new QWidget;
		QVBoxLayout* numberInputWidgetLayout = new QVBoxLayout;
		QFormLayout* numberInput = new QFormLayout;
		QLineEdit* txtNumber = new QLineEdit;
		QPushButton* btnGenerate = new QPushButton{ "Genereaza" };

		numberInput->addRow("Nr. Medicamente", txtNumber);
		numberInputWidgetLayout->addLayout(numberInput);
		numberInputWidgetLayout->addWidget(btnGenerate);
		numberInputWidget->setLayout(numberInputWidgetLayout);

		numberInputWidget->show();

		QObject::connect(btnGenerate, &QPushButton::clicked, [&, txtNumber, numberInputWidget]() {
			QString nr = txtNumber->text();
			try {
				srv.generateRecipe(nr.toStdString());
				numberInputWidget->close();
			}
			catch (ValidatorException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
			catch (RepositoryException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
			});
		});


	QObject::connect(btnExportRecipe, &QPushButton::clicked, [&]() {
		QWidget* exportWidget = new QWidget;
		QVBoxLayout* exportWidgetLayout = new QVBoxLayout;
		QHBoxLayout* fileInputLayout = new QHBoxLayout;
		QFormLayout* fileInputForm = new QFormLayout;
		QLineEdit* txtFile = new QLineEdit;
		QPushButton* btnExport = new QPushButton{ "Export" };
		QLabel* csvLabel = new QLabel{ ".csv" };

		fileInputForm->addRow("Nume fisier", txtFile);
		fileInputLayout->addLayout(fileInputForm);
		fileInputLayout->addWidget(csvLabel);
		exportWidgetLayout->addLayout(fileInputLayout);
		exportWidgetLayout->addWidget(btnExport);
		exportWidget->setLayout(exportWidgetLayout);

		exportWidget->show();

		QObject::connect(btnExport, &QPushButton::clicked, [&, txtFile, exportWidget]() {
			QString filename = txtFile->text();
			try {
				srv.exportRecipe(filename.toStdString());
				exportWidget->close();
				QMessageBox::information(nullptr, "Recipe", "Reteta a fost exportata in " + filename + ".csv");
			}
			catch (ValidatorException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
			catch (std::ofstream::failure ex) { GuiExceptionHandler h(ex); h.showErrors(); }
			});
		});


	QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
		try {
			srv.undo();
			loadList();
			
		}
		catch (ValidatorException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		});


	QObject::connect(btnExit, &QPushButton::clicked, [&]() {
		auto action = QMessageBox::question(nullptr, "Exit", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No);
		if (action == QMessageBox::Yes) {
			close();
		}
		});
	
}


void GuiFilter::initGuiFilter() {
	auto filterLayout = new QVBoxLayout;
	this->setLayout(filterLayout);
	this->setWindowTitle("Filtrare");

	filterLayout->addWidget(btnFiltreazaNume);
	filterLayout->addWidget(btnFiltreazaProd);
	filterLayout->addWidget(btnFiltreazaSubst);
	filterLayout->addWidget(btnFiltreazaPret);
	filterLayout->addWidget(btnFiltreazaPret);
	filterLayout->addWidget(btnClose);

	//filter input for name
	auto filterInputLayout = new QVBoxLayout;
	readFilterWidget->setLayout(filterInputLayout);
	auto filterInputName = new QFormLayout;
	filterInputName->addRow("Introduceti filtru", txtFilter);
	filterInputLayout->addLayout(filterInputName);
	filterInputLayout->addWidget(btnStartFilterNames);
	

	//filter input for prices
	auto filterInputPriceLayout = new QVBoxLayout;
	readFilterPrices->setLayout(filterInputPriceLayout);
	auto filterInputPrice = new QFormLayout;
	filterInputPrice->addRow("Pret minim", pretMin);
	filterInputPrice->addRow("Pret maxim", pretMax);
	filterInputPriceLayout->addLayout(filterInputPrice);
	filterInputPriceLayout->addWidget(btnStartFilterPrice);
}

void GuiFilter::initConnectFilter() {
	//filterWidget
	QObject::connect(btnFiltreazaNume, &QPushButton::clicked, [&]() {
		type = "1";
		readFilterWidget->show();
		});
	QObject::connect(btnFiltreazaProd, &QPushButton::clicked, [&]() {
		type = "2";
		readFilterWidget->show();
		});
	QObject::connect(btnFiltreazaSubst, &QPushButton::clicked, [&]() {
		type = "3";
		readFilterWidget->show();
		});
	QObject::connect(btnFiltreazaPret, &QPushButton::clicked, [&]() {
		type = "4";
		readFilterPrices->show();
		});
	QObject::connect(btnClose, &QPushButton::clicked, [&]() {
		close();
		});


	//filters
	QObject::connect(btnStartFilterPrice, &QPushButton::clicked, [&]() {
	
		QString pmin = pretMin->text();
		QString pmax = pretMax->text();
		try {
			
			gui.loadListWith(srv.filtreaza(type, pmin.toStdString(), pmax.toStdString()));
			
		}
		catch (ValidatorException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		this->readFilterPrices->close();
		this->close();
		});


	QObject::connect(btnStartFilterNames, &QPushButton::clicked, [&]() {
		QString filter = txtFilter->text();
		try {
			gui.loadListWith(srv.filtreaza(type, filter.toStdString(), ""));
		}
		catch (ValidatorException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		this->readFilterWidget->close();
		this->close();
		});
}


void GuiRecipe::initGuiRecipe() {
	//recipe layout
	auto recipeLayout = new QHBoxLayout();
	this->setWindowTitle("Reteta");
	this->setLayout(recipeLayout);

	auto recipeRightLayout = new QVBoxLayout();
	auto recipeLeftLayotut = new QVBoxLayout();

	recipe->setColumnCount(4);
	const QStringList headerList = { "Nume", "Producator", "Substanta activa", "Pret" };
	recipe->setHeaderLabels(headerList);
	recipeLeftLayotut->addWidget(recipe);
	
	recipeRightLayout->addWidget(btnClearRecipe);
	recipeRightLayout->addWidget(btnGenerateRecipe);
	recipeRightLayout->addWidget(btnExportRecipe);
	recipeRightLayout->addWidget(btnExit);

	recipeLayout->addLayout(recipeLeftLayotut);
	recipeLayout->addLayout(recipeRightLayout);
}


void GuiRecipe::initRecipeConnect() {
	rec.addObserver(this);
	QObject::connect(btnClearRecipe, &QPushButton::clicked, [&]() {
		try {
			srv.clearRecipe();
		}
		catch (RepositoryException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
		});


	QObject::connect(btnGenerateRecipe, &QPushButton::clicked, [&]() {
		QWidget* numberInputWidget = new QWidget;
		QVBoxLayout* numberInputWidgetLayout = new QVBoxLayout;
		QFormLayout* numberInput = new QFormLayout;
		QLineEdit* txtNumber = new QLineEdit;
		QPushButton* btnGenerate = new QPushButton{ "Genereaza" };

		numberInput->addRow("Nr. Medicamente", txtNumber);
		numberInputWidgetLayout->addLayout(numberInput);
		numberInputWidgetLayout->addWidget(btnGenerate);
		numberInputWidget->setLayout(numberInputWidgetLayout);

		numberInputWidget->show();

		QObject::connect(btnGenerate, &QPushButton::clicked, [&, txtNumber, numberInputWidget]() {
			QString nr = txtNumber->text();
			try {
				srv.generateRecipe(nr.toStdString());
				numberInputWidget->close();
			}
			catch (ValidatorException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
			catch (RepositoryException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
			});
		});


	QObject::connect(btnExportRecipe, &QPushButton::clicked, [&]() {
		QWidget* exportWidget = new QWidget;
		QVBoxLayout* exportWidgetLayout = new QVBoxLayout;
		QHBoxLayout* fileInputLayout = new QHBoxLayout;
		QFormLayout* fileInputForm = new QFormLayout;
		QLineEdit* txtFile = new QLineEdit;
		QPushButton* btnExport = new QPushButton{ "Export" };
		QLabel* csvLabel = new QLabel{ ".csv" };

		fileInputForm->addRow("Nume fisier", txtFile);
		fileInputLayout->addLayout(fileInputForm);
		fileInputLayout->addWidget(csvLabel);
		exportWidgetLayout->addLayout(fileInputLayout);
		exportWidgetLayout->addWidget(btnExport);
		exportWidget->setLayout(exportWidgetLayout);

		exportWidget->show();

		QObject::connect(btnExport, &QPushButton::clicked, [&, txtFile, exportWidget]() {
			QString filename = txtFile->text();
			try {
				srv.exportRecipe(filename.toStdString());
				exportWidget->close();
				QMessageBox::information(nullptr, "Recipe", "Reteta a fost exportata in " + filename + ".csv");
			}
			catch (ValidatorException& ex) { GuiExceptionHandler h(ex); h.showErrors(); }
			catch (std::ofstream::failure ex) { GuiExceptionHandler h(ex); h.showErrors(); }
			});
		});

	QObject::connect(btnExit, &QPushButton::clicked, [&]() {
		this->close();
		});
}


void GuiRecipe::loadRecipe() {
	recipe->clear();
	vector<Medicament> meds = srv.getRecipe();
	for (const auto med : meds) {
		QTreeWidgetItem* item = new QTreeWidgetItem(recipe);
		item->setText(0, QString::fromStdString(med.getName()));
		item->setText(1, QString::fromStdString(med.getProd()));
		item->setText(2, QString::fromStdString(med.getSubst()));
		item->setText(3, QString::fromStdString(std::to_string(med.getPrice())));
		recipe->addTopLevelItem(item);
	}
}


void GuiExceptionHandler::showErrors() {
	QString errs_to_show;
	for (auto er : this->errors) {
		errs_to_show.push_back(QString::fromStdString(er + '\n'));
	}
	QMessageBox::critical(nullptr, "Error!", errs_to_show);
}



void GuiRecipeReadOnly::initGuiRecipeRO() {
	//recipe layout
	auto paintLayout = new QVBoxLayout;
	this->setMinimumSize(1200, 800);
	this->setWindowTitle("Recipe Read Only");
}

void GuiRecipeReadOnly::paintEvent(QPaintEvent* ev) {
	for (int i = 0; i < rec.getRecipeSize(); i++) {
		QPainter painter{ this };
		QPen pen;
		const QColor color(rand() % 255, rand() % 255, rand() % 255, 255);
		pen.setColor(color);
		pen.setWidth(10);
		painter.setPen(pen);
		int top_left = (rand() * (rand() % 101)) % width();
		int top_right = (rand() * (rand() % 101)) % height();
		int bottom_left = (rand() * (rand() % 101)) % (width() - top_left);
		int bottom_right = (rand() * (rand() % 101)) % (height() - top_right);
		painter.drawRect(top_left, top_right, bottom_left, bottom_right);
	}
}