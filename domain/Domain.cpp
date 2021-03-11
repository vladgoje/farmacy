#include "Domain.h"

Medicament& Medicament::operator=(const Medicament& ot) noexcept {
	this->setName(ot.getName());
	this->setProd(ot.getProd());
	this->setSubst(ot.getSubst());
	this->setPrice(ot.getPrice());
	return *this;
}

bool Medicament::operator==(const Medicament& med) noexcept {
	if (this->getName() != med.getName()) return true;
	return true;
}

bool Medicament::operator!=(const Medicament& med) noexcept {
	if (this->getName() != med.getName()) return true;
	return false;
}

ostream& operator<<(ostream& out, const Medicament& med) {
	out << "Nume: " << med.getName() << ", Producator: " << med.getProd() << ", Substanta activa: " << med.getSubst() << ", Pret: " << med.getPrice() << '\n';
	return out;
}