#include "Tests.h"
#include <fstream>
#include <random>

// ============= TESTE DOMAIN =====================
void Tests::testGetter() {
	Medicament b{ "nume", "prod", "subst", "12.3" };
	assert(b.getName() == "nume");
	assert(b.getProd() == "prod");
	assert(b.getSubst() == "subst");
	assert(b.getPrice() == 12.3);
}

void Tests::testSetter() {
	Medicament b{ "nume", "prod", "subst", "12.3" };

	b.setName("mod1");
	b.setProd("mod2");
	b.setSubst("mod3");
	b.setPrice(45.6);

	assert(b.getName() == "mod1");
	assert(b.getProd() == "mod2");
	assert(b.getSubst() == "mod3");
	assert(b.getPrice() == 45.6);
}

void Tests::testMed() {
	Medicament b{ "nume", "prod", "subst", "12.3" };
	assert(b.getName() == "nume");
	assert(b.getProd() == "prod");
	assert(b.getSubst() == "subst");
	assert(b.getPrice() == 12.3);
	Medicament c{ "nume", "x", "x", "12.3" };
	assert(b == c);
	assert((b != c) == false);
	Medicament d = c;

	assert(c.getName() == d.getName());
	assert(c.getProd() == d.getProd());
	assert(c.getSubst() == d.getSubst());
	assert(c.getPrice() == d.getPrice());
	std::ofstream out; out << b;
	out.clear();
}

void Tests::testRecipe() {
	vector<Medicament> meds;
	Reteta r{ meds };
	assert(r.getRecipeSize() == 0);
	Medicament a{ "nume1", "prod", "subst", "12.3" };
	Medicament b{ "nume2", "prod", "subst", "12.3" };
	Medicament c{ "nume3", "prod", "subst", "12.3" };
	r.addOnRecipe(a);
	assert(r.getRecipeSize() == 1);
	r.addOnRecipe(b);
	assert(r.getRecipeSize() == 2);
	r.addOnRecipe(c);
	assert(r.getRecipeSize() == 3);
	r.clearRecipe();
	assert(r.getRecipeSize() == 0);
}

void Tests::testDomain() {
	testGetter();
	testSetter();
	testMed();
	testRecipe();
}

// ================ TESTE REPO =======================
void Tests::testAdd_rep() {
	Repository repo;
	assert(repo.getAll().size() == 0);
	Medicament a{ "nume", "prod", "subst", "12.3" };
	repo.adauga(a);


	auto all = repo.getAll();
	assert(all.size() == 1);
	assert(all.at(0).getName() == "nume");
	assert(all.at(0).getSubst() == "subst");
	assert(all.at(0).getProd() == "prod");
	assert(all.at(0).getPrice() == 12.3);

	try
	{repo.adauga(Medicament{ "nume", "x", "x", "12.3" }); assert(false);	}
	catch (const RepositoryException& ex) { assert(true); std::ofstream out; out << ex; out.clear(); }
}

void Tests::testGetAll_rep() {
	Repository repo;
	repo.adauga(Medicament{ "nume1", "prod1", "subst1", "12.3" });
	repo.adauga(Medicament{ "nume2", "prod2", "subst2", "12.3" });
	repo.adauga(Medicament{ "nume3", "prod3", "subst3", "12.3" });
	vector<Medicament> all = repo.getAll();
	assert(all.size() == 3);
	assert(all.at(0).getName() == "nume1");
	assert(all.at(1).getProd() == "prod2");
	assert(all.at(2).getSubst() == "subst3");
	assert(all.at(2).getPrice() == 12.3);
}

void Tests::testGet_rep() {
	Repository repo;
	repo.adauga(Medicament{ "nume1", "prod1", "subst1", "12.3" });
	const Medicament med = repo.get("nume1");
	assert(med.getName() == "nume1");
	try
	{const Medicament& no_med = repo.get("x"); assert(false); no_med.getName();}
	catch (RepositoryException&) { assert(true); }
}

void Tests::testModifica_rep() {
	Repository repo;
	repo.adauga(Medicament{ "nume", "prod", "subst", "12.3" });
	repo.adauga(Medicament{ "x", "x", "x", "1" });
	repo.modifica("nume", Medicament{ "mod", "mod", "mod", "45.6" });
	vector<Medicament> all = repo.getAll();
	Medicament test = repo.get("mod");
	assert(test.getName() == "mod");
	assert(test.getProd() == "mod");
	assert(test.getSubst() == "mod");
	assert(test.getPrice() == 45.6);
	try
	{repo.modifica("y", Medicament{"a", "a", "a", "1.2" }); assert(false);}
	catch (RepositoryException&) { assert(true); }
	try
	{repo.modifica("mod", Medicament{ "x", "x", "x", "2" }); assert(false);}
	catch (RepositoryException&) { assert(true); }
}

void Tests::testDelete_rep() {
	Repository repo;
	assert(repo.getAll().size() == 0);
	repo.adauga(Medicament{ "nume1", "prod1", "subst1", "12.3" });
	repo.adauga(Medicament{ "nume2", "prod2", "subst2", "12.3" });
	vector<Medicament> all = repo.getAll();
	assert(all.size() == 2);
	repo.sterge("nume1");
	all = repo.getAll();
	assert(all.size() == 1);
	repo.sterge("nume2");
	all = repo.getAll();
	assert(all.size() == 0);

	try
	{repo.sterge("a"); assert(false);}
	catch (RepositoryException&) { assert(true); }
}

void Tests::testCauta_rep() {
	Repository repo;
	repo.adauga(Medicament{ "nume", "prod1", "subst1", "12.3" });
	repo.adauga(Medicament{ "x", "prod1", "subst1", "12.3" });

	const Medicament& med = repo.search("x");
	assert(med.getName() == "x");
	assert(med.getProd() == "prod1");
	assert(med.getSubst() == "subst1");
	assert(med.getPrice() == 12.3);

	try
	{const Medicament& no_med = repo.search("y"); assert(false); no_med.getName();}
	catch (RepositoryException&) { assert(true); }
}

void Tests::testClearRecipe_rep() {
	Reteta repo;

	assert(repo.getRecipe().size() == 0);
	repo.addOnRecipe(Medicament{ "nume", "prod1", "subst1", "12.3" });
	assert(repo.getRecipe().size() == 1);
	repo.clearRecipe();
	assert(repo.getRecipe().size() == 0);
	repo.addOnRecipe(Medicament{ "nume2", "prod1", "subst1", "12.3" });
	repo.addOnRecipe(Medicament{ "x", "prod1", "subst1", "12.3" });
	assert(repo.getRecipe().size() == 2);
	repo.clearRecipe();
	assert(repo.getRecipe().size() == 0);

	try
	{repo.clearRecipe(); assert(false); }
	catch (RepositoryException&) { assert(true); }
}

void Tests::testAddOnRecipe_rep() {
	Reteta repo;
	assert(repo.getRecipe().size() == 0);
	repo.addOnRecipe(Medicament{ "nume", "prod1", "subst1", "12.3" });
	assert(repo.getRecipe().size() == 1);
	repo.clearRecipe();
	repo.addOnRecipe(Medicament{ "nume", "prod1", "subst1", "12.3" });
	repo.addOnRecipe(Medicament{ "x", "prod1", "subst1", "12.3" });
	assert(repo.getRecipe().size() == 2);
	repo.clearRecipe();
	assert(repo.getRecipe().size() == 0);


	repo.addOnRecipe(Medicament{ "nume", "prod1", "subst1", "12.3" });
	try
	{repo.addOnRecipe(Medicament{ "nume", "prod1", "subst1", "12.3" }); assert(false); }
	catch (RepositoryException&) { assert(true); }
}

void Tests::testRepo() {
	testAdd_rep();
	testGetAll_rep();
	testGet_rep();
	testModifica_rep();
	testDelete_rep();
	testCauta_rep();
	testClearRecipe_rep();
	testAddOnRecipe_rep();
}

// =================== TESTE SERVICE ======================
void Tests::testAdd_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };

	assert(srv.getAll().size() == 0);
	srv.adauga("nume", "prod", "subst", "12.3");
	vector<Medicament> all = srv.getAll();
	assert(all.size() == 1);
	assert(all.at(0).getName() == "nume");
	assert(all.at(0).getSubst() == "subst");
	assert(all.at(0).getProd() == "prod");
	assert(all.at(0).getPrice() == 12.3);

	try
	{ srv.adauga("", "", "", "-4"); assert(false); }
	catch (const ValidatorException& ex) { assert(true); std::ofstream out; out << ex; out.clear(); }
	try
	{ srv.adauga("x", "", "y", "123.3"); assert(false); }
	catch (ValidatorException& ) { assert(true); }
}

void Tests::testGetAll_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("nume1", "prod1", "subst1", "12.3");
	srv.adauga("nume2", "prod2", "subst2", "12.3");
	srv.adauga("nume3", "prod3", "subst3", "12.3");

	vector<Medicament> all = srv.getAll();
	assert(all.size() == 3);
	assert(all.at(0).getName() == "nume1");
	assert(all.at(1).getProd() == "prod2");
	assert(all.at(2).getSubst() == "subst3");
	assert(all.at(2).getPrice() == 12.3);
}

void Tests::testGet_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("nume1", "prod1", "subst1", "12.3");
	const Medicament med = repo.get("nume1");
	assert(med.getName() == "nume1");
}

void Tests::testModifica_srv(){
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("nume", "prod", "subst", "12.3");
	srv.modifica("nume", "mod", "mod", "mod", "45.6");

	assert((*srv.getAll().begin()).getName() == "mod");
	assert((*srv.getAll().begin()).getProd() == "mod");
	assert((*srv.getAll().begin()).getSubst() == "mod");
	assert((*srv.getAll().begin()).getPrice() == 45.6);

	try
	{ srv.modifica("nume", "", "", "", "-1"); assert(false); }
	catch (ValidatorException&) { assert(true); }
	try
	{ srv.modifica("", "a", "a", "a", "1.2"); assert(false); }
	catch (ValidatorException&) { assert(true); }
}

void Tests::testDelete_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	assert(srv.getAll().size() == 0);
	srv.adauga("nume1", "prod1", "subst1", "12.3");
	srv.adauga("nume2", "prod2", "subst2", "12.3");
	assert(srv.getAll().size() == 2);
	srv.sterge("nume1");
	assert(srv.getAll().size() == 1);
	srv.sterge("nume2");
	assert(srv.getAll().size() == 0);
	assert(srv.getAll().size() == 0);
}

void Tests::testCauta_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("nume", "prod1", "subst1", "12.3");
	srv.adauga("x", "prod1", "subst1", "12.3");

	const Medicament med = srv.cauta("x");
	assert(med.getName() == "x");
	assert(med.getProd() == "prod1");
	assert(med.getSubst() == "subst1");
	assert(med.getPrice() == 12.3);
}

void Tests::testFiltreaza1_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("nume1", "prod1", "x", "12.3");
	srv.adauga("nume2", "prod2", "subst2", "12.3");
	srv.adauga("nume3", "prod3", "x", "45.6");

	vector<Medicament> all = srv.filtreaza("3", "x", "");
	assert(all.size() == 2);
	assert(all.at(0).getName() == "nume1");
	assert(all.at(0).getSubst() == "x");
	assert(all.at(0).getPrice() == 12.3);
	assert(all.at(1).getName() == "nume3");
	assert(all.at(1).getSubst() == "x");
	assert(all.at(1).getPrice() == 45.6);

	all = srv.filtreaza("1", "nume1", "");
	assert(all.size() == 1);
	all = srv.filtreaza("2", "prod1", "");
	assert(all.size() == 1);

	try
	{ vector<Medicament> nul_vect = srv.filtreaza("1", "", ""); assert(false); }
	catch (ValidatorException&) { assert(true); }
	try
	{ vector<Medicament> nul_vect = srv.filtreaza("7", "a", "b"); assert(false); }
	catch (ValidatorException&) { assert(true); }
}

void Tests::testFiltreaza2_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("nume1", "prod1", "subst1", "1.2");
	srv.adauga("nume2", "prod2", "subst2", "3.4");
	srv.adauga("nume3", "prod3", "subst3", "5.6");

	vector<Medicament> all = srv.filtreaza("4", "1.2", "3.5");
	assert(all.size() == 2);
	assert(all.at(0).getName() == "nume1");
	assert(all.at(0).getPrice() == 1.2);
	assert(all.at(1).getName() == "nume2");
	assert(all.at(1).getPrice() == 3.4);

	try
	{ vector<Medicament> nul_vect = srv.filtreaza("4", "abc", "def"); assert(false); }
	catch (ValidatorException&) { assert(true); }	
}

void Tests::testSortare_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("a", "c", "b", "3.4");
	srv.adauga("b", "b", "a", "1.2");
	srv.adauga("c", "a", "c", "1.2");
	srv.adauga("x", "x", "a", "4.5");
	 
	vector<Medicament> sorted = srv.sorteaza("1");

	assert(sorted.at(0).getName() == "a");
	assert(sorted.at(0).getSubst() == "b");
	assert(sorted.at(0).getPrice() == 3.4);
	assert(sorted.at(1).getName() == "b");
	assert(sorted.at(1).getSubst() == "a");
	assert(sorted.at(1).getPrice() == 1.2);
	
	sorted = srv.sorteaza("2");

	assert(sorted.at(0).getName() == "c");
	assert(sorted.at(0).getProd() == "a");
	assert(sorted.at(0).getPrice() == 1.2);
	assert(sorted.at(1).getName() == "b");
	assert(sorted.at(1).getProd() == "b");
	assert(sorted.at(1).getPrice() == 1.2);

	sorted = srv.sorteaza("3");

	assert(sorted.at(0).getSubst() == "a");
	assert(sorted.at(0).getPrice() == 1.2);
	assert(sorted.at(1).getSubst() == "a");
	assert(sorted.at(1).getPrice() == 4.5);
	
	try 
	{ vector<Medicament> no_sort = srv.sorteaza("7"); assert(false); }
	catch (ValidatorException&) { assert(true); }
}

void Tests::testStats_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("a", "x", "b", "3.4");
	srv.adauga("b", "x", "a", "1.2");
	srv.adauga("c", "x", "c", "1.2");
	srv.adauga("d", "y", "a", "4.5");
	srv.adauga("e", "y", "a", "4.5");
	srv.adauga("f", "z", "a", "4.5");

	vector<StatsDTO> stats = srv.stats();
	int sum_x{ 0 }, sum_y{ 0 }, sum_z{ 0 };
	assert(stats.size() == 3);
	vector<StatsDTO>::iterator it = stats.begin();
	while (it != stats.end())
	{
		if ((*it).getTip() == "x")
		{
			sum_x++;
			assert((*it).getNr() == 50.0);
		}
		if ((*it).getTip() == "y")
			sum_y++;
		if ((*it).getTip() == "z")
			sum_z++;
		++it;
	}
	assert(sum_x == 1);
	assert(sum_y == 1);
	assert(sum_z == 1);
}

void Tests::testAddOnRecipe_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("a", "x", "b", "3.4");
	srv.adauga("b", "x", "a", "1.2");
	srv.adauga("c", "x", "c", "1.2");
	srv.addOnRecipe("a");
	assert(srv.getRecipe().size() == 1);
	srv.addOnRecipe("b");
	assert(srv.getRecipe().size() == 2);
	srv.addOnRecipe("c");
	assert(srv.getRecipe().size() == 3);
	srv.clearRecipe();
	assert(srv.getRecipe().size() == 0);

	try
	{srv.addOnRecipe(""); assert(false); }
	catch (ValidatorException&) { assert(true); }
}

void Tests::testClearRecipe_srv(){
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("a", "x", "b", "3.4");
	srv.adauga("b", "x", "a", "1.2");
	srv.adauga("c", "x", "c", "1.2");
	assert(srv.getRecipe().size() == 0);
	srv.addOnRecipe("a");
	assert(srv.getRecipe().size() == 1);
	srv.clearRecipe();
	assert(srv.getRecipe().size() == 0);
	srv.addOnRecipe("a");
	srv.addOnRecipe("b");
	assert(srv.getRecipe().size() == 2);
	srv.clearRecipe();
	assert(srv.getRecipe().size() == 0);
}

void Tests::testGenerateRecipe_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	assert(srv.getRecipe().size() == 0);
	srv.generateRecipe("7");
	assert(srv.getRecipe().size() == 7);
	srv.generateRecipe("14");
	assert(srv.getRecipe().size() == 21);
	srv.generateRecipe("3");
	assert(srv.getRecipe().size() == 24);
	try
	{srv.generateRecipe("a"); assert(false);}
	catch (ValidatorException&) { assert(true); }
	try
	{srv.generateRecipe("-4"); assert(false);}
	catch (ValidatorException&) { assert(true); }
	try
	{srv.generateRecipe("3.5"); assert(false);}
	catch (ValidatorException&) { assert(true); }

	recipe.addOnRecipe(Medicament{"a", "a", "a", "1"});
	recipe.generateReceipe(Medicament{ "a", "a", "a", "1" });
}

void Tests::testGetRecipeSize_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.generateRecipe("7");
	assert(srv.getRecipeSize() == 7);
}

void Tests::testExport_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	recipe.addOnRecipe(Medicament{ "a", "a", "a", "1" });
	recipe.addOnRecipe(Medicament{ "b", "b", "b", "1" });
	recipe.addOnRecipe(Medicament{ "c", "c", "c", "1" });
	recipe.addOnRecipe(Medicament{ "d", "d", "d", "1" });
	srv.exportRecipe("test");
	
	string line;
	std::ifstream fin("test.csv");

	getline(fin, line);
	//assert(line == "Nume: a, Producator: a, Substanta activa: a, Pret: 1");
	getline(fin, line);
	//assert(line == "Nume: b, Producator: b, Substanta activa: b, Pret: 1");
	getline(fin, line);
	//assert(line == "Nume: c, Producator: c, Substanta activa: c, Pret: 1");
	getline(fin, line);
	//assert(line == "Nume: d, Producator: d, Substanta activa: d, Pret: 1");

	try { srv.exportRecipe("?");  assert(false); }
	catch (std::ofstream::failure) {assert(true);}
}

void Tests::testPriceSum_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("a", "x", "b", "1");
	srv.adauga("b", "x", "a", "2");
	srv.adauga("c", "x", "c", "3");
	assert(srv.priceSum() == 6);
}

void Tests::deleteFromRecipe_srv() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	recipe.addOnRecipe(Medicament{ "a", "a", "a", "1" });
	recipe.addOnRecipe(Medicament{ "b", "b", "b", "1" });
	repo.adauga(Medicament{ "a", "a", "a", "1" });
	repo.adauga(Medicament{ "b", "b", "b", "1" });
	assert(recipe.getRecipeSize() == 2);
	srv.deleteFromRecipe("b");
	assert(recipe.getRecipeSize() == 1);
	srv.deleteFromRecipe("a");
	assert(recipe.getRecipeSize() == 0);

	try { srv.deleteFromRecipe("x"); assert(false); }
	catch (RepositoryException&) { assert(true); }
}

void Tests::testService() {
	testAdd_srv();
	testGetAll_srv();
	testGet_srv();
	testModifica_srv();
	testDelete_srv();
	testCauta_srv();
	testFiltreaza1_srv();
	testFiltreaza2_srv();
	testSortare_srv();
	testStats_srv();
	testAddOnRecipe_srv();
	testClearRecipe_srv();
	testGenerateRecipe_srv();
	testGetRecipeSize_srv();
	testPriceSum_srv();
	deleteFromRecipe_srv();
	testExport_srv();
}


// ========================TESTE VALIDATOR ========================
void Tests::testValideazaMed() {
	Validator val;
	val.valideazaMed(Medicament{ "a","b","c","12.34" }, "12.34");
	assert(true);
	try
	{val.valideazaMed(Medicament{ "","","","0" }, ""); assert(false);}
	catch (ValidatorException&) { assert(true); }
	try
	{val.valideazaMed(Medicament{ "a","","c","-1.5" }, "-1.5"); assert(false);}
	catch (ValidatorException&) { assert(true); }
}

void Tests::testValideazaFiltrare() {
	Validator val;
	val.valideazaFiltrare("1", "abc", "");
	assert(true);

	val.valideazaFiltrare("2", "3", "14");
	assert(true);

	try
	{val.valideazaFiltrare ("1", "", ""); assert(false); }
	catch (ValidatorException&) { assert(true); }
	try
	{val.valideazaFiltrare("4", "abc", "abc"); assert(false); }
	catch (ValidatorException&) { assert(true); }
	try
	{val.valideazaFiltrare("4", "-1", "999"); assert(false); }
	catch (ValidatorException&) { assert(true); }
	try
	{val.valideazaFiltrare("4", "1", "-999"); assert(false); }
	catch (ValidatorException&) { assert(true); }
	try
	{val.valideazaFiltrare("4", "7", "6"); assert(false); }
	catch (ValidatorException&) { assert(true); }
	try
	{val.valideazaFiltrare("7", "a", "a"); assert(false); }
	catch (ValidatorException&) { assert(true); }
}

void Tests::testValideazaString() {
	Validator val;
	val.valideazaString("abc"); assert(true);
	try
	{val.valideazaString(""); assert(false); }
	catch (ValidatorException&) { assert(true); }
}

void Tests::testValideazaType() {
	Validator val;
	val.valideazaType("1"); assert(true);
	val.valideazaType("2"); assert(true);
	val.valideazaType("3"); assert(true);
	val.valideazaType("0"); assert(true);
	try
	{val.valideazaType(""); assert(false); }
	catch (ValidatorException&) { assert(true); }
	try
	{val.valideazaType("7"); assert(false); }
	catch (ValidatorException&) { assert(true); }
}

void Tests::testValideazaID() {
	Validator val;
	val.valideazaId("7"); assert(true);
	val.valideazaId("0"); assert(true);
	try
	{val.valideazaId("a"); assert(false); }
	catch (ValidatorException&) { assert(true); }
	try
	{val.valideazaId(""); assert(false); }
	catch (ValidatorException&) { assert(true); }
	try
	{val.valideazaId("-1"); assert(false); }
	catch (ValidatorException&) { assert(true); }
}

void Tests::testValideazaPrice() {
	Validator val;
	assert(val.valideazaPrice("7") == 0);
	assert(val.valideazaPrice("0") == 0);
	assert(val.valideazaPrice("a") == -1);
	assert(val.valideazaPrice("") == -1);
	assert(val.valideazaPrice("-1") == -1);
	assert(val.valideazaPrice("1.1.1") == -1);
}

void Tests::testValidator() {
	testValideazaMed();
	testValideazaFiltrare();
	testValideazaString();
	testValideazaType();
	testValideazaID();
	testValideazaPrice();
}

//===============undo=======================
void Tests::testUndo() {
	Repository repo; Validator val; Reteta recipe;  Service srv{ repo, val , recipe };
	srv.adauga("a", "x", "b", "1");
	srv.adauga("b", "x", "a", "2");
	srv.modifica("b", "c", "c", "c", "4");
	
	assert(srv.cauta("c").getName() == "c");
	assert(srv.cauta("c").getPrice() == 4);

	srv.addOnRecipe("a");
	assert(srv.getRecipeSize() == 1);
	srv.undo();
	assert(srv.getRecipeSize() == 0);


	srv.undo();

	try { srv.cauta("b"); assert (true); }
	catch (RepositoryException&) { assert(false); }

	assert(srv.cauta("b").getName() == "b");
	assert(srv.cauta("b").getPrice() == 2);

	srv.sterge("b");

	try { srv.cauta("b"); assert(false); }
	catch (RepositoryException&) { assert(true); }
	assert(srv.getAll().size() == 1);

	srv.undo();

	try { Medicament x = srv.cauta("b"); assert(true); }
	catch (RepositoryException&) { assert(false); }
	assert(srv.getAll().size() == 2);
	assert(srv.cauta("b").getName() == "b");
	assert(srv.cauta("b").getPrice() == 2);

	srv.undo();
	assert(srv.getAll().size() == 1);
	srv.undo();
	assert(srv.getAll().size() == 0);
	try { srv.undo(); assert(false); }
	catch (ValidatorException&) { assert(true); }
}

void Tests::testFileRepo() {
	FileRepo repo2("testmed.txt");
	try { repo2.adauga(Medicament{ "nume", "prod", "subst", "12.3" }); assert(true); }
	catch (RepositoryException&) { assert(true); };
	try {
	repo2.modifica("a", Medicament{ "nume", "prod", "subst", "12.3" }); assert(true);}
	catch (RepositoryException&) { assert(true); };
	try {
		repo2.sterge("nume"); assert(true);}
	catch (RepositoryException&) { assert(true); };
}


void Tests::testProbRepo() {
	ProbRepo repo(1);
	try {
		repo.adauga(Medicament{ "nume", "prod", "subst", "12.3" });assert(false);}
	catch (RepositoryException&) { assert(true); };
	try {
		repo.modifica("a", Medicament{ "nume", "prod", "subst", "12.3" });assert(false);}
	catch (RepositoryException&) { assert(true); };
	try {
		repo.sterge("nume");assert(false);}
	catch (RepositoryException&) { assert(true); };
	try {
		repo.search("nume");assert(false);}
	catch (RepositoryException&) { assert(true); };
	try {
		repo.get("nume");assert(false);}
	catch (RepositoryException&) { assert(true); };

	ProbRepo repo2(0);
	try {repo2.adauga(Medicament{ "nume", "prod", "subst", "12.3" }); assert(true);}
	catch (RepositoryException&) { assert(true); };
	try {
		repo2.modifica("a", Medicament{ "nume", "prod", "subst", "12.3" }); assert(true);}
	catch (RepositoryException&) { assert(true); };
	try {
		repo2.sterge("nume"); assert(true);}
	catch (RepositoryException&) { assert(true); };
	try {
		repo2.search("nume"); assert(true);}
	catch (RepositoryException&) { assert(true); };
	try {
		repo2.get("nume"); assert(true);}
	catch (RepositoryException&) { assert(true); };
}

void Tests::testExtins() {
	ProbRepo repo(0.5);
	for (int i = 0; i < 100; i++) {
		try {
			repo.adauga(Medicament{ "nume", "prod", "subst", "12.3" });
			std::cout << "adauga\n";
		}
		catch(RepositoryException&){
			std::cout << "nu adauga\n";
		}
		try {
			repo.sterge("nume");
			std::cout << "sterge\n";
		}
		catch (RepositoryException&) {
			std::cout << "nu sterge\n";
		}
	}
}

// ============= all ========================
void Tests::testAll(){
	testDomain();
	testRepo();
	testFileRepo();
	testProbRepo();
	testService();
	testValidator();
	testUndo();
	testExtins();
	std::cout << "teste trecute\n";
}