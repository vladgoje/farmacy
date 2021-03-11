#include "GuiModel.h"
#include <sstream>
#include <qpaintengine.h>

//MAIN LIST MODEL
int MyListModel::rowCount(const QModelIndex& parent) const {
	return meds.size();
}

int MyListModel::columnCount(const QModelIndex& parent) const {
	return 4;
}

QVariant MyListModel::data(const QModelIndex& index, int role) const {
	int row = index.row();
	int col = index.column();

	if (role == Qt::DisplayRole) {
		Medicament m = meds[row];
		if (col == 0)
			return QString::fromStdString(m.getName());
		if (col == 1)
			return QString::fromStdString(m.getProd());
		if (col == 2)
			return QString::fromStdString(m.getSubst());
		if (col == 3)
			return QString::fromStdString(std::to_string(m.getPrice()));
	}
	if (role == Qt::BackgroundRole)
		if (row % 2 == 0) {
			QBrush background(QColor(255, 230, 200, 100));
			return background;
		}
	if (role == Qt::ForegroundRole) {
		Medicament m = meds[row];
		if (m.getPrice() >= 700 && col == 3) {
			QBrush background(Qt::red);
			return background;
		}
		else if (m.getPrice() <= 300 && col == 3) {
			QBrush background(Qt::darkGreen);
			return background;
		}
	}
	return QVariant{};
}

QVariant MyListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return QString("Nume");
		case 1:
			return QString("Producator");
		case 2:
			return QString("Substanta activa");
		case 3:
			return QString("Pret");
		}
	}
	return QVariant();
}

void MyListModel::setMeds(const vector<Medicament>& meds) {
	this->meds = meds;
	emit layoutAboutToBeChanged();
	auto topLeft = createIndex(0, 0);
	auto bottomRight = createIndex(rowCount(), columnCount());
	emit dataChanged(topLeft, bottomRight);
	emit layoutChanged();
}


//INIT MODEL GUI
void GuiModel::initGUI() {
	this->setWindowTitle("ModelView GUI");
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

	//view
	list->setModel(myModel);
	topLeftLayout->addWidget(list);
	

	topLayout->addLayout(topLeftLayout);

	//top right layout
	auto topRightLayout = new QVBoxLayout;
	auto topRightOptions = new QGridLayout;
	QPixmap farmacy("farmacy.png");
	QPixmap recipe("recipe.png");
	auto farmLabel = new QLabel; farmLabel->setPixmap(farmacy);
	auto recipeLabel = new QLabel; recipeLabel->setPixmap(recipe);
	topRightOptions->addWidget(farmLabel, 0, 0, Qt::AlignCenter);
	topRightOptions->addWidget(recipeLabel, 0, 1, Qt::AlignCenter);
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

	btnListLayout->setMargin(20);
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(btnListLayout);
}

//actual sort type
void GuiModel::actualSortType() {
	if (boxSorteazaNume->isChecked())
		sortType = "1";
	else if (boxSorteazaProd->isChecked())
		sortType = "2";
	else if (boxSorteazaSubst->isChecked())
		sortType = "3";
	else
		sortType = "0";
}

// reload list
void GuiModel::reloadList(vector<Medicament> meds) {
	myModel->setMeds(meds);
}

void GuiModel::connectSignals() {
	// selection
	QObject::connect(list->selectionModel(), &QItemSelectionModel::selectionChanged, [&] {
		if (list->selectionModel()->selectedIndexes().isEmpty()) {
			txtNume->setText(""); txtProd->setText(""); txtSubst->setText(""); txtPret->setText("");
			return;
		}
		auto row = list->selectionModel()->selectedIndexes().at(0).row();
		auto col0 = list->model()->index(row, 0);
		auto col1 = list->model()->index(row, 1);
		auto col2 = list->model()->index(row, 2);
		auto col3 = list->model()->index(row, 3);
		txtNume->setText(list->model()->data(col0, Qt::DisplayRole).toString());
		txtProd->setText(list->model()->data(col1, Qt::DisplayRole).toString());
		txtSubst->setText(list->model()->data(col2, Qt::DisplayRole).toString());
		txtPret->setText(list->model()->data(col3, Qt::DisplayRole).toString());
		});


	// adauga
	QObject::connect(btnAdauga, &QPushButton::clicked, [&]() {
		auto nume = txtNume->text();
		auto prod = txtProd->text();
		auto subst = txtSubst->text();
		auto pret = txtPret->text();
		try {
			srv.adauga(nume.toStdString(), prod.toStdString(), subst.toStdString(), pret.toStdString());
			actualSortType();
			reloadList(srv.sorteaza(sortType));
		}
		catch (ValidatorException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
		catch (RepositoryException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
		});

	// sterge
	QObject::connect(btnSterge, &QPushButton::clicked, [&]() {
		if (list->selectionModel()->selectedIndexes().empty())
			QMessageBox::warning(nullptr, "Warning!", "Invalid selection!");
		else {
			auto row = list->selectionModel()->selectedIndexes().at(0).row();
			auto col0 = list->model()->index(row, 0);
			auto nume = list->model()->data(col0, Qt::DisplayRole).toString();
			try {
				srv.sterge(nume.toStdString());
				actualSortType();
				reloadList(srv.sorteaza(sortType));
				list->selectionModel()->clearSelection();
			}
			catch (ValidatorException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
			catch (RepositoryException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
		}
		});

	// modifica
	QObject::connect(btnModifica, &QPushButton::clicked, [&]() {
		if (list->selectionModel()->selectedIndexes().empty())
			QMessageBox::warning(nullptr, "Warning!", "Invalid selection!");
		else {
			auto row = list->selectionModel()->selectedIndexes().at(0).row();
			auto col0 = list->model()->index(row, 0);
			auto _nume = list->model()->data(col0, Qt::DisplayRole).toString();
			auto nume = txtNume->text();
			auto prod = txtProd->text();
			auto subst = txtSubst->text();
			auto pret = txtPret->text();
			try {
				srv.modifica(_nume.toStdString(), nume.toStdString(), prod.toStdString(), subst.toStdString(), pret.toStdString());
				actualSortType();
				reloadList(srv.sorteaza(sortType));
				list->selectionModel()->clearSelection();
			}
			catch (ValidatorException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
			catch (RepositoryException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
		}
		});

	// open filter widget
	QObject::connect(btnFiltreaza, &QPushButton::clicked, [&]() {
		auto filterWidget = new GuiModelFilter(*this);
		filterWidget->show();
		});

	// delete filters
	QObject::connect(btnStergeFiltre, &QPushButton::clicked, [&]() {
		actualSortType();
		reloadList(srv.sorteaza(sortType));
		});


	//sortari
	//sort by name
	QObject::connect(boxSorteazaNume, &QCheckBox::clicked, [&]() {
		if (boxSorteazaNume->isChecked()) {
			boxSorteazaProd->setCheckState(Qt::Unchecked);
			boxSorteazaSubst->setCheckState(Qt::Unchecked);
			actualSortType();
			reloadList(srv.sorteaza(sortType));
		}
		else reloadList(srv.getAll());
		});

	//sort by prod
	QObject::connect(boxSorteazaProd, &QCheckBox::clicked, [&]() {
		if (boxSorteazaProd->isChecked()) {
			boxSorteazaNume->setCheckState(Qt::Unchecked);
			boxSorteazaSubst->setCheckState(Qt::Unchecked);
			actualSortType();
			reloadList(srv.sorteaza(sortType));
		}
		else reloadList(srv.getAll());
		});

	//sort by subst
	QObject::connect(boxSorteazaSubst, &QCheckBox::clicked, [&]() {
		if (boxSorteazaSubst->isChecked()) {
			boxSorteazaNume->setCheckState(Qt::Unchecked);
			boxSorteazaProd->setCheckState(Qt::Unchecked);
			actualSortType();
			reloadList(srv.sorteaza(sortType));	
		}
		else reloadList(srv.getAll());
		});

	// show main recipe
	QObject::connect(btnRecipe, &QPushButton::clicked, [&]() {
		auto recipe = new GuiModelRecipe(*this);
		recipe->show();
		});

	// show recipe read only (paint)
	QObject::connect(btnRecipeReadOnly, &QPushButton::clicked, [&]() {
		auto recipe = new GuiModelRecipeReadOnly(*this);
		recipe->show();
		});

	//add do recipe
	QObject::connect(btnAddToRecipe, &QPushButton::clicked, [&]() {
		if (list->selectionModel()->selectedIndexes().empty())
			QMessageBox::warning(nullptr, "Warning!", "Invalid selection!");
		else {
			QString name = txtNume->text();
			try {
				srv.addOnRecipe(name.toStdString());
				list->selectionModel()->clearSelection();
			}
			catch (RepositoryException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
		}
		});

	// delete med from recipe
	QObject::connect(btnDeleteFromRecipe, &QPushButton::clicked, [&]() {
		if (list->selectionModel()->selectedIndexes().empty())
			QMessageBox::warning(nullptr, "Warning!", "Invalid selection!");
		else {
			QString name = txtNume->text();
			try {
				srv.deleteFromRecipe(name.toStdString());
				list->selectionModel()->clearSelection();
			}
			catch (RepositoryException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
		}
		});

	// clear recipe
	QObject::connect(btnClearRecipe, &QPushButton::clicked, [&]() {
		try {
			srv.clearRecipe();
		}
		catch (RepositoryException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
		});

	// generate recipe
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
			catch (ValidatorException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
			catch (RepositoryException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
			});
		});

	// export
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
			catch (ValidatorException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
			catch (std::ofstream::failure ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
			});
		});


	// stats
	QObject::connect(btnStats, &QPushButton::clicked, [&]() {
		GuiModelStats* stats = new GuiModelStats(*this);
		stats->show();
		});

	
	// undo
	QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
		try {
			srv.undo();
			actualSortType();
			reloadList(srv.sorteaza(sortType));
		}
		catch (ValidatorException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
		});


	//exit
	QObject::connect(btnExit, &QPushButton::clicked, [&]() {
		auto action = QMessageBox::question(nullptr, "Exit", "Are you sure you want to exit?", QMessageBox::Yes | QMessageBox::No);
		if (action == QMessageBox::Yes) {
			close();
		}
		});
}



/*----------GUI MODEL FILTER------------*/

void GuiModelFilter::initGuiFilter() {
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

void GuiModelFilter::initConnectFilter() {
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

			gui.reloadList(srv.filtreaza(type, pmin.toStdString(), pmax.toStdString()));

		}
		catch (ValidatorException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
		this->readFilterPrices->close();
		this->close();
		});


	QObject::connect(btnStartFilterNames, &QPushButton::clicked, [&]() {
		QString filter = txtFilter->text();
		try {
			gui.reloadList(srv.filtreaza(type, filter.toStdString(), ""));
		}
		catch (ValidatorException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
		this->readFilterWidget->close();
		this->close();
		});
}


void GuiModelRecipe::initGuiRecipe() {
	//recipe layout
	auto recipeLayout = new QHBoxLayout();
	this->setWindowTitle("Reteta");
	this->setLayout(recipeLayout);

	auto recipeRightLayout = new QVBoxLayout();
	auto recipeLeftLayotut = new QVBoxLayout();

	recipe->setModel(recipeModel);
	recipeLeftLayotut->addWidget(recipe);

	recipeRightLayout->addWidget(btnClearRecipe);
	recipeRightLayout->addWidget(btnGenerateRecipe);
	recipeRightLayout->addWidget(btnExportRecipe);
	recipeRightLayout->addWidget(btnExit);

	recipeLayout->addLayout(recipeLeftLayotut);
	recipeLayout->addLayout(recipeRightLayout);
}


void GuiModelRecipe::initRecipeConnect() {
	rec.addObserver(this);
	QObject::connect(btnClearRecipe, &QPushButton::clicked, [&]() {
		try {
			srv.clearRecipe();
		}
		catch (RepositoryException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
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
			catch (ValidatorException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
			catch (RepositoryException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
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
			catch (ValidatorException& ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
			catch (std::ofstream::failure ex) { GuiModelExceptionHandler h(ex); h.showErrors(); }
			});
		});

	QObject::connect(btnExit, &QPushButton::clicked, [&]() {
		this->close();
		});
}


void GuiModelRecipe::loadRecipe() {
	recipeModel->setMeds(srv.getRecipe());
}


void GuiModelExceptionHandler::showErrors() {
	QString errs_to_show;
	for (auto er : this->errors) {
		errs_to_show.push_back(QString::fromStdString(er + '\n'));
	}
	QMessageBox::critical(nullptr, "Error!", errs_to_show);
}


void GuiModelRecipeReadOnly::initGuiRecipeRO() {
	//recipe layout
	auto paintLayout = new QVBoxLayout;
	this->setMinimumSize(1200, 800);
	this->setWindowTitle("Recipe Read Only");
}

void GuiModelRecipeReadOnly::paintEvent(QPaintEvent* ev) {
	for (int i = 0; i < srv.getRecipeSize(); i++) {
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


void GuiModelStats::initStats() {
	QVBoxLayout* statsLayout = new QVBoxLayout;
	const QStringList header{ "Producator", "Procent" };
	statsList->setHeaderLabels(header);
	statsList->setColumnCount(2);
	statsLayout->addWidget(statsList);
	this->setLayout(statsLayout);
	this->setMinimumSize(400, 300);
}

void GuiModelStats::connectStats() {
	srv.addObserver(this);
}

void GuiModelStats::loadStats() {
	statsList->clear();
	auto statsDTOs = srv.stats();
	for (auto s : statsDTOs) {
		QTreeWidgetItem* item = new QTreeWidgetItem(statsList);
		item->setText(0, QString::fromStdString(s.getTip()));
		item->setText(1, QString::fromStdString(std::to_string(s.getNr())) + " %");
		statsList->addTopLevelItem(item);
	}
}