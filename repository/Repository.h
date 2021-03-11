#pragma once
#include "Domain.h" 
#include "Observer.h"
#include <vector>
#include <unordered_map>
using std::string;
using std::ostream;
using std::vector;
using std::unordered_map;

class RepositoryException {
	vector<string> errors;
public:
	RepositoryException(const string& msg);
	vector<string> getErrors() {
		return this->errors;
	}
	friend ostream& operator<<(ostream& out, const RepositoryException& ex);
};

ostream& operator<<(ostream& out, const RepositoryException& ex);


class IRepo {
public:
	virtual vector<Medicament> getAll() const noexcept = 0;
	virtual void adauga(const Medicament& med) = 0;
	virtual void modifica(const string& _name, const Medicament& med) = 0;
	virtual Medicament sterge(const string& name) = 0;
	virtual const Medicament search(const string& name) = 0;
	virtual const Medicament get(const string& name) = 0;
	virtual ~IRepo() = default;
};


class Repository : public IRepo {

private:
	unordered_map<string, Medicament> med_map;
	vector<Medicament> meds;

protected:
	bool exist(const string& name) const;

public:
	Repository() = default;
	Repository(const Repository& ot) = delete;

	vector<Medicament> getAll() const noexcept override;
	virtual void adauga(const Medicament& med) override;
	virtual void modifica(const string& _name, const Medicament& med) override;
	virtual Medicament sterge(const string& name) override;
	const Medicament search(const string& name) override;
	const Medicament get(const string& name) override;
};


class ProbRepo : public Repository {
private:
	double prob;
	unordered_map<string, Medicament> med_map;
public:
	ProbRepo(const double _prob) : prob{ _prob } {}
	ProbRepo(const ProbRepo& ot) = delete;

	virtual void adauga(const Medicament & med) override;
	virtual void modifica(const string & _name, const Medicament & med) override;
	virtual Medicament sterge(const string & name) override;
	const Medicament search(const string & name) override;
	const Medicament get(const string & name) override;
	void _throw() {
		bool chance = (rand() % 101) < (this->prob * 100);
		if (chance) {
			throw RepositoryException("chaos");}}
};

class FileRepo : public Repository {
private:
	string filename;
	unordered_map<string, Medicament> med_map;
	vector<Medicament> meds;

	void loadFromFile();
	void writeToFile();
public:
	FileRepo(const string& _filename) {
		this->filename = _filename;
		loadFromFile();
	}
	FileRepo(const FileRepo& ot) = delete;

	void adauga(const Medicament& med) override;
	void modifica(const string& _name, const Medicament& med) override;
	Medicament sterge(const string& name) override;
};



/////////////////cos
class Reteta : public Observable {
private:
	vector<Medicament> meds;
public:
	friend class Repository;
	friend class GuiRecipe;
	Reteta() noexcept = default;
	Reteta(const vector<Medicament>& v) : meds{ v } {}
	
	vector<Medicament> getRecipe() const noexcept;
	void clearRecipe();
	void addOnRecipe(const Medicament& med);
	void deleteFromRecipe(const string& name);
	int generateReceipe(const Medicament& med) noexcept;
	int getRecipeSize() const noexcept;
	friend ostream& operator<<(ostream& out, const vector<Medicament>& ex);

};

ostream& operator<<(ostream& out, const vector<Medicament>& ex);
