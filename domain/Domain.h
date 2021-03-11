#pragma once
#include <iostream>
#include <string>
#include <vector>

using std::ostream;
using std::string;
using std::vector;

class Medicament {
private:
	string name;
	string producer;
	string subst;
	double price = 0.0;

public:
	//custom implicit constr
	Medicament() noexcept = default;
	//constr
	Medicament (const string& n, const string& p, const string& s, const string& pr) throw() :name{ n }, producer{ p }, subst{ s }, price{ std::stod(pr)}{}
	//avoid copy  
	Medicament(const Medicament& ot) noexcept: name{ ot.getName() }, producer{ ot.getProd() }, subst{ ot.getSubst() }, price{ ot.getPrice() }{}
	Medicament(Medicament&&) = default;

	string getName() const throw() {
		return this->name;
	}
	string getProd() const throw() {
		return this->producer;
	}
	string getSubst() const throw() {
		return this->subst;
	}
	double getPrice() const noexcept {
		return this->price;
	}

	void setName(string val) throw() {
		this->name = val;
	}
	void setProd(string val) throw() {
		this->producer = val;
	}
	void setSubst(string val) throw() {
		this->subst = val;
	}
	void setPrice(double val) noexcept {
		this->price = val;
	}

	Medicament& operator=(const Medicament& ot) noexcept;
	Medicament& operator=( Medicament&&) noexcept = default;
	bool operator==(const Medicament& med) noexcept;
	bool operator!=(const Medicament& med) noexcept;

	friend ostream& operator<<(ostream& out, const Medicament& med);

	virtual ~Medicament() = default;
};

ostream& operator<<(ostream& out, const Medicament& med);

class StatsDTO {
private:
	string tip;
	float nr;
public:
	StatsDTO() = default;
	StatsDTO(const string& _tip, const float _nr) : tip{ _tip }, nr{ _nr } {}
	string getTip() throw(){
		return this->tip;
	}
	float getNr() noexcept{
		return this->nr;
	}
	void setNr(float value) {
		this->nr = value;
	}
};


