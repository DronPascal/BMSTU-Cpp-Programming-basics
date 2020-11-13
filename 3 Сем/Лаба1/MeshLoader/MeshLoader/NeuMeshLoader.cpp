#include "NeuMeshLoader.h"
#include "Exceptions.h"

void NeuMeshLoader::LoadMesh(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file) {
		throw NoFileFoundException(filename);
	}
	else std::cout << "Ôàéë \"" << filename << "\" îòêðûò." << std::endl;
	//ÇÀÏÎËÍÅÍÈÅ ÊÎÍÒÅÉÍÅÐÎÂ

	//Óçëîâ
	int count, node_count;
	file >> count;
	Get_Nodes_Container().reserve(count + 1);
	Get_Nodes_Container().resize(count + 1);

	for (int i = 0; i < count; i++)
	{
		Node& cur_node = Get_Nodes_Container()[i + 1];
		cur_node.id = i + 1;
		cur_node.flag = true;
		file >> cur_node.x1 >> cur_node.x2 >> cur_node.x3;
	}

	//ÊÝ
	file >> count;
	node_count = 4;
	Get_FE_Container().reserve(count + 1);
	Get_FE_Container().resize(count + 1);

	for (int i = 0; i < count; i++)
	{
		FiniteElement& cur_fe = Get_FE_Container()[i + 1];
		cur_fe.ke_id = i + 1;
		file >> cur_fe.mat_id;
		int buf;
		for (int j = 0; j < node_count; j++)
		{
			file >> buf;
			cur_fe.nodes_id.push_back(buf);
		}
	};

	//Ãðàíè÷íûõ ÊÝ
	file >> count;
	node_count = 3;
	Get_BFE_Container().reserve(count + 1);
	Get_BFE_Container().resize(count + 1);

	for (int i = 0; i < count; i++)
	{
		BoundaryFiniteElement& cur_bfe = Get_BFE_Container()[i + 1];
		cur_bfe.ke_id = i + 1;
		file >> cur_bfe.border_id;
		int buf;
		for (int j = 0; j < node_count; j++)
		{
			file >> buf;
			cur_bfe.nodes_id.push_back(buf);
		}
	};

	file.close();
}
