#include <QtWidgets/QApplication>
#include <fstream>
#include "GUI.h"
#include "GuiModel.h"
#include "UI.h"
#include "Tests.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	Tests t;
	t.testAll();
	Reteta recipe;

	//Repository repo;
	//ProbRepo repo{ 0.5 };
	FileRepo repo{ "meds.txt" };
	//FileRepo repo{ "medsBig.txt" };

	Validator val;
	Service srv{ repo, val, recipe };

	/*std::ofstream fout("medsBig.txt");
	for (int i = 0; i < 10000; i++) {
		fout << "m" + std::to_string(i) << " " << srv.randomString() << " " << srv.randomString() << " " << srv.randomPrice() << '\n';
	}*/

	//UI ui{ srv };
	//GUI gui(srv);
	GuiModel gui{ srv };
	
	gui.show();

    return a.exec();
}
