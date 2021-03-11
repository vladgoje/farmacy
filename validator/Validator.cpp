#include "Validator.h"
#include <float.h>
#include <vector>

using std::vector;

//================Validator Exception===========
ValidatorException::ValidatorException(const vector<string>& msgs) {
	for(const auto& msg : msgs)
		this->errors.push_back(msg);
}

ostream& operator<<(ostream& out, const ValidatorException& ex) {
	vector<string> msgs = ex.errors;
	for(const auto& msg : msgs)
		out << msg << "\n";
	return out;
}

//================Validator===========

void Validator::valideazaMed(const Medicament& med, const string& price) const{
	vector<string> errors;
	if (med.getName().size() == 0) 
		errors.push_back("Nume nu poate fi vid");
	if (med.getProd().size() == 0)
		errors.push_back("Producator nu poate fi vid");
	if (med.getSubst().size() == 0)
		errors.push_back("Substanta nu poate fi vida");
	if (valideazaPrice(price) == -1)
		errors.push_back("Pret invalid");
	if (errors.size())
		throw ValidatorException(errors);
}

void Validator::valideazaFiltrare(const string& type, const string& filter1, const string& filter2) const {
	vector<string> errors;
	if (type != "1" && type != "2" && type != "3" && type != "4"){
		errors.push_back("Tipul de filtrare nu e valid! Reintroduceti");
		throw ValidatorException(errors);
	}
	
	if (type == "4")
	{
		valideazaString(filter1);
		valideazaString(filter2);
		if (valideazaPrice(filter1) == -1 || valideazaPrice(filter2) == -1) {
			errors.push_back("Preturile introduse nu sunt valide! Reintroduceti");
			throw ValidatorException(errors);
		}
		if (std::stof(filter1) > std::stof(filter2))
		{
			errors.push_back("Preturile introduse nu sunt valide! Reintroduceti");
			throw ValidatorException(errors);
		}
	}
	else {
		valideazaString(filter1);
	}
}

void Validator::valideazaString(const string& filter) const {
	vector<string> errors;
	if (filter.size() <= 0)
		errors.push_back("Campul introdus nu poate fi vid! Reintroduceti");
	if (errors.size())
		throw ValidatorException(errors);
}

void Validator::valideazaType(const string& type) const {
	vector<string> errors;
	if (type != "0" && type != "1" && type != "2" && type != "3")
		errors.push_back("Tipul de sortare nu e valid! Reintroduceti");
	if (errors.size())
		throw ValidatorException(errors);
}

void Validator::valideazaId(const string& id) const {
	vector<string> errors;
	if(id.size() == 0)
	{
		errors.push_back("Numar invalid!");
		throw ValidatorException(errors);
	}
	for (const auto& c : id) {
		if (c < '0' || c > '9') {
			errors.push_back("Numar invalid!");
			throw ValidatorException(errors);
		}
	}
}

int Validator::valideazaPrice(const string& price) const {
	vector<string> errors;
	if (price.size() == 0){
		return -1;
	}
	for (const auto& c : price) {
		if (c < '0' || c > '9')
			if (c != '.') {
				return -1;
			}
	}
	if (const size_t n = std::count(price.begin(), price.end(), '.') > 1) {
		return -1;
	}
	return 0;
}