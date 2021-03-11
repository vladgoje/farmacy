#pragma once
#include <vector>
using std::vector;

class Observer {
public:
	virtual void update() = 0;
};

class Observable {
	vector<Observer*> interesati;
protected:
	void notify() {
		for (auto obs : interesati) {
			obs->update();
		}
	}
public:
	void addObserver(Observer* obs) {
		interesati.push_back(obs);
	}
	void removeObserver(Observer* obs) {
		interesati.erase(std::remove(interesati.begin(), interesati.end(), obs));
	}
};