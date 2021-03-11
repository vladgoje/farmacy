#include "Repository.h"
#include <fstream>
#include <algorithm>

//=============Repo Exception================
RepositoryException::RepositoryException(const string& msg) {
	this->errors.push_back(msg);
}

ostream& operator<<(ostream& out, const RepositoryException& ex) {
	vector<string> msgs = ex.errors;
	for (const auto& msg : msgs) {
		out << msg << '\n';
	}
	return out;
}

//=============Repo================
void Repository::adauga(const Medicament& med) {
	if (exist(med.getName()))
		throw RepositoryException("Medicamentul introdus exista deja!");
	med_map[med.getName()] = med;
}


bool Repository::exist(const string& name) const{
	if (this->med_map.find(name) == med_map.end()){
		return false;
	}
	return true;
}


vector<Medicament> Repository::getAll() const noexcept{
	vector<Medicament> meds;
	std::transform(med_map.begin(), med_map.end(), std::back_inserter(meds), [](std::pair<string, Medicament> p) {
		return p.second;
		});
	return meds;
}


void Repository::modifica(const string& _name, const Medicament& med) {
	if (!exist(_name))
		throw RepositoryException("Medicamentul cu numele introdus nu exista!");
	if (exist(med.getName()) && med.getName() != _name)
		throw RepositoryException("Medicamentul modificat exista deja!");
	med_map.erase(_name);
	med_map[med.getName()] = med;
}


Medicament Repository::sterge(const string& name) {
	if (!exist(name))
		throw RepositoryException("Medicamentul cu numele introdus nu exista!");
	Medicament del = med_map[name];
	med_map.erase(name);
	return del;
}


const Medicament Repository::get(const string& name) {
	if(!exist(name))
		throw RepositoryException("Medicamentul cu numele introdus nu exista!");
	return med_map[name];
}


const Medicament Repository::search(const string& name){
	return get(name);
}


//-------------ProbRepo
void ProbRepo::adauga(const Medicament& med) {
	_throw();
	Repository::adauga(med);}


void ProbRepo::modifica(const string& _name, const Medicament& med) {
	_throw();
	Repository::modifica(_name, med);}


Medicament ProbRepo::sterge(const string& name) {
	_throw();
	return Repository::sterge(name);}


const Medicament ProbRepo::get(const string& name) {
	_throw();
	return Repository::get(name);}


const Medicament ProbRepo::search(const string& name) {
	_throw();
	return Repository::search(name);}



//filerepo
void FileRepo::adauga(const Medicament& med) {
	Repository::adauga(med);
	writeToFile();
}

void FileRepo::modifica(const string& _name, const Medicament& med) {
	if (exist(med.getName()) && med.getName() != _name)
		throw RepositoryException("Medicamentul modificat exista deja!");
	Repository::sterge(_name);
	Repository::adauga(med);
	writeToFile();
}
Medicament FileRepo::sterge(const string& name) {
	Medicament med = Repository::sterge(name);
	writeToFile();
	return med;
}

void FileRepo::loadFromFile(){
	std::ifstream fin(this->filename);
	if (!fin.is_open()) {
		throw RepositoryException("Unable to open the file:" + this->filename);
	}

	while (!fin.eof()) {
		string name;
		fin >> name;
		if (name == "")
			break;
		string prod;
		fin >> prod;
		string subst;
		fin >> subst;
		string price;
		fin >> price;
		Medicament med{ name, prod, subst, price };
		Repository::adauga(med);
	}
	fin.close();
}

void FileRepo::writeToFile() {
	std::ofstream fout;
	fout.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	fout.open(this->filename, std::ios::out | std::ios::trunc);
	vector<Medicament> all = getAll();
	for (const auto& med : all) {
		fout << med.getName() << ' ' << med.getProd() <<' ' << med.getSubst() << ' ' << med.getPrice() << '\n';
	}
	fout.close();
}

//recipe
vector<Medicament> Reteta::getRecipe() const noexcept {
	return this->meds;
}


int Reteta::getRecipeSize() const noexcept {
	return this->meds.size();
}


void Reteta::clearRecipe() {
	if (this->meds.size() == 0)
		throw(RepositoryException("Reteta este deja goala!"));
	this->meds.clear();
	notify();
}


void Reteta::addOnRecipe(const Medicament& med) {
	for (const auto& _med : this->meds)
		if (_med.getName() == med.getName())
			throw(RepositoryException("Medicamentul exista deja pe reteta!"));
	this->meds.push_back(med);
	notify();
}


void Reteta::deleteFromRecipe(const string& name) {
	int ok = 0;
	vector<Medicament>::iterator it = this->meds.begin();
	while (it != this->meds.end()) {
		if ((*it).getName() == name) {
			this->meds.erase(it);
			ok = 1;
			break;
		}
		++it;
	}
	if(!ok)
		throw(RepositoryException("Medicamentul nu exista pe reteta!"));
	notify();
}


int Reteta::generateReceipe(const Medicament& med) noexcept{
	for (const auto& _med : this->meds)
		if (_med.getName() == med.getName())
			return 0;
	this->meds.push_back(med);
	notify();
	return 1;
}

ostream& operator<<(ostream& out, const vector<Medicament>& meds) {
	for (const auto& med : meds)
		out << med;
	return out;
}

