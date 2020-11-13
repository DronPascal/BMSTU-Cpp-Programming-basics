//ЛАБОРАТОРНАЯ РАБОТА № 1
//по курсу «Основы программирования на C++»
//Разработка конечно - элементного загрузчика сетки
//Работу выполнили Паскал Андрей и Шибанов Александар ФН11-32б

#include "MeshLoader.h"
#include "NeuMeshLoader.h"
#include "AneuMeshLoader.h"
#include "Exceptions.h"

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

MeshLoader* check_filename(std::string& file);
int mesh_loadertest(MeshLoader* mesh_loader, const std::string& filename);

int main(int argc, char* argv[])
{
	setlocale(0, "russian");
	std::string filename = "MeshExample.aneu";

	if (argc == 1)
	{
		std::cout << "Запуск без аргументов из папки решения проекта." << std::endl;
	}
	else if (argc > 2)
	{
		std::cout << "Допускается лишь 1 агрумент - полное имя файла" << std::endl;
	}
	else
	{
		std::cout << "Полное имя файла: " << argv[1] << std::endl;
		filename = argv[1];
	}

	MeshLoader* mesh_loader = check_filename(filename);
	mesh_loadertest(mesh_loader, filename);
}

MeshLoader* check_filename(std::string& file)
{
	MeshLoader* mesh_pointer = nullptr;

	if (file.find(".aneu") != std::string::npos)
	{
		std::cout << "Аneu формат обнаружен" << std::endl;
		mesh_pointer = new AneuMeshLoader;
		return mesh_pointer;
	}
	else if (file.find(".neu") != std::string::npos)
	{
		std::cout << "Neu формат обнаружен" << std::endl;
		mesh_pointer = new NeuMeshLoader;
		return mesh_pointer;
	}
	else
	{
		std::cout << "Нечитаемый формат файла" << std::endl;
		if (file.find('.') == std::string::npos)
			throw BadFileFormatException();
		else throw BadFileFormatException(file.substr(file.find('.'), file.size() - file.find('.')));
	};
}

int mesh_loadertest(MeshLoader* mesh_loader, const std::string& filename) 
{
	try {
		mesh_loader->LoadMesh(filename);
	}
	catch (NoFileFoundException e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	std::cout << "\n\n";
	mesh_loader->Print_MeshData();
	std::vector<FiniteElement> f_elsements;
	std::vector<Node> nodes;
	std::vector<std::vector<Node>> neighbours;
	std::vector<BoundaryFiniteElement> bf_elsements;

	std::cout << "Найденные элементы Get_FE_by_3ID(69, 40, 39): \n";
	f_elsements = mesh_loader->Get_FE_by_3ID(69, 40, 39);
	for (auto e : f_elsements) { std::cout << e; } std::cout << std::endl;
	f_elsements.clear();

	std::cout << "\nНайденные элементы Get_FE_by_Edge_Fast(3, 48): \n";
	f_elsements = mesh_loader->Get_FE_by_Edge_Fast(3, 48);
	for (auto e : f_elsements) { std::cout << e; } std::cout << std::endl;
	f_elsements.clear();

	std::cout << "\nНайденные элементы Get_FE_by_Edge_Slow(3, 48): \n";
	f_elsements = mesh_loader->Get_FE_by_Edge_Slow(3, 48);
	for (auto e : f_elsements) { std::cout << e; } std::cout << std::endl;
	f_elsements.clear();

	std::cout << "\nНайденные элементы Get_BoundaryNodes_by_borderId(12): \n";
	nodes = mesh_loader->Get_BoundaryNodes_by_borderId(12);
	for (auto e : nodes) { std::cout << e; }  std::cout << std::endl;
	nodes.clear();

	std::cout << "\nНайденные элементы Get_FE_by_matId(1): \n";
	f_elsements = mesh_loader->Get_FE_by_matId(1);
	for (auto e : f_elsements) { std::cout << e; } std::cout << std::endl;
	f_elsements.clear();

	std::cout << "\nНайденные элементы Get_BFE_by_borderId(12): \n";
	bf_elsements = mesh_loader->Get_BFE_by_borderId(31);
	for (auto e : bf_elsements) { std::cout << e; } std::cout << std::endl;
	bf_elsements.clear();

	std::cout << "\nНайденные элементы Get_AdjacentNodesContainer(): \n";
	neighbours = mesh_loader->Get_AdjacentNodesContainer();
	for (unsigned int i = 1; i < neighbours.size(); ++i)
	{
		if (mesh_loader->isVertex(i))
		{
			std::cout << "Соседи (по ребрам) узла с ID = " << i << ": ";
			for (auto n : neighbours[i]) std::cout << n.id << " ";
			std::cout << std::endl;
		}
	}

	std::cout << "\nДобавление новых узлов в центр ребер: \n";
	mesh_loader->Add_NewNodes();
	mesh_loader->Print_MeshData();

	return 0;
}
// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"