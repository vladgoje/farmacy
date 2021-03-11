#include "Domain.h"
#include "Repository.h"

class ActiuneUndo {
public:
	virtual void doUndo() = 0;
	virtual ~ActiuneUndo() = default;
};


class UndoAdd : public ActiuneUndo {
	Medicament medAdaugat;
	IRepo& rep;

public:
	UndoAdd(IRepo& _rep, const Medicament& med) : rep{ _rep }, medAdaugat{ med } {}
	void doUndo() override {
		rep.sterge(medAdaugat.getName());
	}
};


class UndoDelete : public ActiuneUndo {
	Medicament medSters;
	IRepo& rep;

public:
	UndoDelete(IRepo& _rep, const Medicament& med) : rep{ _rep }, medSters{ med } {}
	void doUndo() override {
		rep.adauga(medSters);
	}
};


class UndoUpdate : public ActiuneUndo {
	Medicament medUpdate;
	string medName;
	IRepo& rep;

public:
	UndoUpdate(IRepo& _rep, const Medicament& med, const string& name) : rep{ _rep }, medUpdate{ med }, medName{ name } {}
	void doUndo() override {
		rep.modifica(medName, medUpdate);
	}
};

class UndoAddRecipe : public ActiuneUndo {
	Medicament medAdaugat;
	Reteta& rep;
public:
	UndoAddRecipe(Reteta& _rep, const Medicament& med) : rep{ _rep }, medAdaugat{ med } {}
	void doUndo() override {
		rep.deleteFromRecipe(medAdaugat.getName());
	}
};


class UndoDeleteRecipe : public ActiuneUndo {
	Medicament medSters;
	Reteta& rep;
public:
	UndoDeleteRecipe(Reteta& _rep, const Medicament& med) : rep{ _rep }, medSters{ med } {}
	void doUndo() override {
		rep.addOnRecipe(medSters);
	}
};


class UndoGenerateClearRecipe : public ActiuneUndo {
	vector<Medicament> backup;
	Reteta& rep;
public:
	UndoGenerateClearRecipe(Reteta& _rep, vector<Medicament> bp) : rep{ _rep }, backup{ bp } {}
	void doUndo() override {
		if(rep.getRecipe().size())
			rep.clearRecipe();
		for (auto med : backup) {
			rep.addOnRecipe(med);
		}
	}
};