#pragma once
#include "Service.h"
#include <assert.h>

class Tests {
	//teste domain
	void testGetter();
	void testSetter();
	void testMed();
	void testRecipe();
	void testDomain();
	

	//teste repo
	void testAdd_rep();
	void testGetAll_rep();
	void testGet_rep();
	void testModifica_rep();
	void testDelete_rep();
	void testCauta_rep();
	void testClearRecipe_rep();
	void testAddOnRecipe_rep();
	void testRepo();

	void testFileRepo();
	void testProbRepo();
	void testExtins();

	//teste service
	void testAdd_srv();
	void testGetAll_srv();
	void testGet_srv();
	void testModifica_srv();
	void testDelete_srv();
	void testCauta_srv();
	void testFiltreaza1_srv();
	void testFiltreaza2_srv();
	void testSortare_srv();
	void testStats_srv();
	void testAddOnRecipe_srv();
	void testClearRecipe_srv();
	void testGenerateRecipe_srv();
	void testGetRecipeSize_srv();
	void testExport_srv();
	void testPriceSum_srv();
	void deleteFromRecipe_srv();
	void testService();

	//teste validator
	void testValideazaMed();
	void testValideazaFiltrare();
	void testValideazaString();
	void testValideazaType();
	void testValideazaID();
	void testValideazaPrice();
	void testValidator();

	//test undo
	void testUndo();

public:
	void testAll();
};