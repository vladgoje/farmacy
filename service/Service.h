#pragma once
#include "Repository.h"
#include "Validator.h"
#include "Undo.h"

using std::string;
using std::vector;
using::std::unique_ptr;

class Service : public Observable {
private:
	Repository& repo;
	Validator& val;
	Reteta& recipe;
	std::vector<unique_ptr<ActiuneUndo>> undoActions;

public:
	Service(Repository& repo, Validator& val, Reteta& recipe) noexcept :repo{ repo }, val{ val }, recipe{ recipe } {}
	Service(const Service& ot) = delete;

	void adauga(const string& name, const string& prod, const string& subst, const string& price);
	void modifica(const string& _name, const string& name, const string& prod, const string& subst, const string& price);
	const vector<Medicament> getAll() noexcept;
	void sterge(const string& name);
	const Medicament cauta(const string& name);

	vector<Medicament> filtreaza(const string& type, const string& filter1, const string& filter2);
	vector<Medicament> filtreaza_name(const string& name);
	vector<Medicament> filtreaza_prod(const string& prod);
	vector<Medicament> filtreaza_subst(const string& subst);
	vector<Medicament> filtreaza_pret(const double pret1, const double filter2);
	vector<Medicament> sorteaza(const string& type);
	vector<StatsDTO> stats();

	//reteta
	vector<Medicament> getRecipe() const noexcept;
	Reteta& getRecipeRef() const noexcept;
	int getRecipeSize() const noexcept;
	void clearRecipe();
	void addOnRecipe(const string& name);
	void deleteFromRecipe(const string& name);
	void generateRecipe(const string& nr);
	void exportRecipe(const string& fname);
	string randomString();
	string randomPrice();
	double priceSum();

	//undo
	void undo();
};