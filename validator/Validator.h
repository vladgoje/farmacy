#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Domain.h"

using std::string;
using std::ostream;
using std::vector;

class ValidatorException {
	vector<string> errors;
	//friend class Validator;
public:
	vector<string> getErrors() { return this->errors; }
	ValidatorException(const vector<string>& msgs);
	friend ostream& operator<<(ostream& out, const ValidatorException& ex);
};

ostream& operator<<(ostream& out, const ValidatorException& ex);

class Validator {

public:
	void valideazaMed(const Medicament& med, const string& price) const;
	void valideazaFiltrare(const string& type, const string& filter1, const string& filter2) const;
	void valideazaString(const string& name) const;
	void valideazaType(const string& type) const;
	void valideazaId(const string& id) const;
	int valideazaPrice(const string& id) const;
};