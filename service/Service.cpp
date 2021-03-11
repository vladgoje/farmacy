#include "Service.h"
#include <unordered_map> 
#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>
#include <numeric>


void Service::undo() {
	if (undoActions.empty()) {
		vector<string> errors;
		errors.push_back("Nu mai exista actiuni pentru undo!");
		throw ValidatorException(errors);}

	undoActions.back()->doUndo();
	undoActions.pop_back();
	notify();
}

void Service::adauga(const string& name, const string& prod, const string& subst, const string& price) {
	Medicament med{ name, prod, subst, "0" };
	val.valideazaMed(med, price);
	med.setPrice(std::stod(price));
	repo.adauga(med);
	undoActions.push_back(std::make_unique<UndoAdd>(repo, med));
	notify();
}


void Service::modifica(const string& _name, const string& name, const string& prod, const string& subst, const string& price) {

	val.valideazaString(_name);
	Medicament med{ name, prod, subst, "0" };
	val.valideazaMed(med, price);
	med.setPrice(std::stod(price));
	Medicament mod = repo.get(_name);
	repo.modifica(_name, med);
	undoActions.push_back(std::make_unique<UndoUpdate>(repo, mod, med.getName()));
	notify();
}


void Service::sterge(const string& name) {

	val.valideazaString(name);
	Medicament med = repo.sterge(name);
	undoActions.push_back(std::make_unique<UndoDelete>(repo, med));
	notify();
}


const vector<Medicament> Service::getAll() noexcept {
	return repo.getAll();
}


const Medicament Service::cauta(const string& name) {
	val.valideazaString(name);
	return repo.search(name);
} 


vector<Medicament> Service::filtreaza_name(const string& filter1) {
	vector<Medicament> filtered;
	vector<Medicament> all = getAll();
	std::copy_if(all.begin(), all.end(), std::back_inserter(filtered), [filter1](const Medicament m) {
		return m.getName() == filter1; });
	return filtered;
}

vector<Medicament> Service::filtreaza_prod(const string& filter1) {
	vector<Medicament> filtered;
	vector<Medicament> all = getAll();
	std::copy_if(all.begin(), all.end(), std::back_inserter(filtered), [filter1](const Medicament m) {
		return m.getProd() == filter1; });
	return filtered;
}

vector<Medicament> Service::filtreaza_subst(const string& filter1) {
	vector<Medicament> filtered;
	vector<Medicament> all = getAll();
	std::copy_if(all.begin(), all.end(), std::back_inserter(filtered), [filter1](const Medicament m) {
		return m.getSubst() == filter1;});
	return filtered;
}

vector<Medicament> Service::filtreaza_pret(const double pret_min, const double pret_max) {
	vector<Medicament> filtered;
	vector<Medicament> all = getAll();
	std::copy_if(all.begin(), all.end(), std::back_inserter(filtered), [pret_min, pret_max](const Medicament& m) {
		return (m.getPrice() >= pret_min && m.getPrice() <= pret_max);});
	return filtered;
}


vector<Medicament> Service::filtreaza(const string& type, const string& filter1, const string& filter2) {
	val.valideazaFiltrare(type, filter1, filter2);
	if (type == "1")
		return filtreaza_name(filter1);
	if (type == "2")
		return filtreaza_prod(filter1);
	if (type == "3")
		return filtreaza_subst(filter1);
	else
		return filtreaza_pret(std::stod(filter1), std::stod(filter2));
}


vector<Medicament> Service::sorteaza(const string& type) {
	val.valideazaType(type);
	vector<Medicament> all = getAll();
	if (type == "0")
		return all;
	else if (type == "1") {
		std::sort(all.begin(), all.end(), [](const Medicament& m1, const Medicament& m2) noexcept {
			return m1.getName() < m2.getName(); });
		return all;}
	else if (type == "2") {
		std::sort(all.begin(), all.end(), [](const Medicament& m1, const Medicament& m2) noexcept {
			return m1.getProd() < m2.getProd(); });
		return all;}
	else {
		std::sort(all.begin(), all.end(), [](const Medicament& m1, const Medicament& m2) noexcept {
			if (m1.getSubst() == m2.getSubst())
				return m1.getPrice() < m2.getPrice();
			return m1.getSubst() < m2.getSubst();
			});
		return all;}}

vector<StatsDTO> Service::stats() {
	unordered_map<string, StatsDTO> stats;
	vector<Medicament> all = getAll();

	for(const auto& med : all)
		if (stats.find(med.getProd()) == stats.end())
			stats[med.getProd()] = StatsDTO(med.getProd(), 1);
		else {
			const auto nr = stats[med.getProd()].getNr() + 1;
			stats[med.getProd()].setNr(nr);
		}

	vector<StatsDTO> v_stats;
	int size = all.size();
	std::transform(stats.begin(), stats.end(), std::back_inserter(v_stats), [size](std::pair<string, StatsDTO> p) {
		p.second.setNr((p.second.getNr() / size) * 100);
		return p.second; });
	return v_stats;
}

vector<Medicament> Service::getRecipe() const noexcept {
	return this->recipe.getRecipe();
}

Reteta& Service::getRecipeRef() const noexcept {
	return this->recipe;
}

void Service::clearRecipe() {
	auto backup = recipe.getRecipe();
	if (backup.size() != 0) {
		undoActions.push_back(std::make_unique<UndoGenerateClearRecipe>(recipe, backup));
	}
	recipe.clearRecipe();
}

void Service::addOnRecipe(const string& name) {
	val.valideazaString(name);
	Medicament med = cauta(name);
	recipe.addOnRecipe(med);
	undoActions.push_back(std::make_unique<UndoAddRecipe>(recipe, med));
}

void Service::deleteFromRecipe(const string& name) {
	val.valideazaString(name);
	auto med = repo.get(name);
	recipe.deleteFromRecipe(name);
	undoActions.push_back(std::make_unique<UndoDeleteRecipe>(recipe, med));
}

void Service::generateRecipe(const string& nr) {
	val.valideazaId(nr);
	int _nr = 0;
	auto backup = recipe.getRecipe();
	while (_nr < std::stof(nr)) {
		Medicament med(randomString(), randomString(), randomString(), randomPrice());
		_nr += recipe.generateReceipe(med);
	}
	if(nr != "0")
		undoActions.push_back(std::make_unique<UndoGenerateClearRecipe>(recipe, backup));
}

int Service::getRecipeSize() const noexcept{
	return recipe.getRecipeSize();
}

void Service::exportRecipe(const string& fname) {
	val.valideazaString(fname);
	string csv = ".csv";
	string file_name = fname + csv;

	std::ofstream fout;
	fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	fout.open(file_name, std::ios::out | std::ios::trunc);
	fout << getRecipe();
	fout.close();
}

string Service::randomPrice() {
	string digits("123456789");
	std::mt19937 generator(std::random_device{}());
	std::shuffle(digits.begin(), digits.end(), generator);
	return digits.substr(0, 3);
}

string Service::randomString() {
	string letters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	std::mt19937 generator(std::random_device{}());
	std::shuffle(letters.begin(), letters.end(), generator);
	return letters.substr(0, 10);
}

double Service::priceSum() {
	vector<Medicament> all = getAll();
	double sum = 0;
	sum = std::accumulate(all.begin(), all.end(), static_cast<double>(0.0), [](double _sum, const Medicament& med) {
		return _sum + med.getPrice();
		});
	return sum;
}