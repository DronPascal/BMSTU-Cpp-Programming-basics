#include "MeshLoader.h"
#include <algorithm>
#include <map>
#include <unordered_set>
#include <iomanip>

//проверка узла на вершинность
bool MeshLoader::isVertex(unsigned int id, ...) const
{
	bool result = true;
	for (unsigned int* ptr = &id; id > 0; id--)
	{
		if (id < FE_Container.size())
		result = result && Nodes_Container[id].flag;
		else return false;
	}
	return result;
}
bool MeshLoader::isVertex(const FiniteElement& fe) const
{
	bool result = true;
		for (auto& id : fe.nodes_id)
			if (id < FE_Container.size())
				result = result && Nodes_Container[id].flag;
			else return false;
	return result;
}

//	1) методы, позволяющие получить STL-контейнеры узлов, конечных элементов и граничных элементов
std::vector<Node>& MeshLoader::Get_Nodes_Container()
{
	return Nodes_Container;
}
std::vector<FiniteElement>& MeshLoader::Get_FE_Container()
{
	return FE_Container;
}
std::vector <BoundaryFiniteElement>& MeshLoader::Get_BFE_Container()
{
	return BFE_Container;
}

//	2) метод, позволяющий найти КЭ по ID трех его вершинных узлов
std::vector<FiniteElement> MeshLoader::Get_FE_by_3ID(unsigned int id1, unsigned int id2, unsigned int id3) const
{
	std::vector<FiniteElement> founded;
	if (isVertex(id1, id2, id3))
	{
		auto pred = [&id1, &id2, &id3](const FiniteElement& cur_ke)
		{
			int overlap = 0;
			for (auto list_iter = cur_ke.nodes_id.begin(); list_iter != cur_ke.nodes_id.end(); ++list_iter)
			{
				if ((*list_iter) == id1 || (*list_iter) == id2 || (*list_iter) == id3) overlap++;
			}
			if (overlap == 3) return true;
			//else if (overlab>3) Неправильно построена сетка 
			else return false;
		};

		auto cur = ++FE_Container.begin();
		auto end = FE_Container.end();
		while (cur != end)
		{
			cur = std::find_if(cur, end, pred);
			if (cur != end)
			{
				founded.push_back(*cur);
				cur++;
			}
		}
	}
	return founded;
}

//	3) метод, позволяющий найти КЭ по ребру, заданному с помощью ID двух узлов
auto pair_func = [&](unsigned int& id1, unsigned int& id2)
{
	if (id1 < id2)
		return std::make_pair(id1, id2);
	else
		return std::make_pair(id2, id1);
};
void MeshLoader::make_FE_Edge_map()
{
	if (FE_based_on_Edge.empty())
	{
		for (auto iter = ++FE_Container.begin(); iter != FE_Container.end(); ++iter)
		{
			FE_based_on_Edge.insert(std::make_pair(pair_func((*iter).nodes_id[0], (*iter).nodes_id[1]), (*iter)));
			FE_based_on_Edge.insert(std::make_pair(pair_func((*iter).nodes_id[0], (*iter).nodes_id[2]), (*iter)));
			FE_based_on_Edge.insert(std::make_pair(pair_func((*iter).nodes_id[0], (*iter).nodes_id[3]), (*iter)));
			FE_based_on_Edge.insert(std::make_pair(pair_func((*iter).nodes_id[1], (*iter).nodes_id[2]), (*iter)));
			FE_based_on_Edge.insert(std::make_pair(pair_func((*iter).nodes_id[1], (*iter).nodes_id[3]), (*iter)));
			FE_based_on_Edge.insert(std::make_pair(pair_func((*iter).nodes_id[2], (*iter).nodes_id[3]), (*iter)));
		}
	}
}
std::vector<FiniteElement> MeshLoader::Get_FE_by_Edge_Fast(unsigned int id1, unsigned int id2)
{
	std::vector<FiniteElement> founded;
	if (isVertex(id1, id2))
	{
		make_FE_Edge_map();
		auto diap = FE_based_on_Edge.equal_range(pair_func(id1, id2));
		if (diap.first != diap.second)
			for (auto iter = diap.first; iter != diap.second; ++iter)
			{
				founded.push_back((*iter).second);
			}
	}
	return founded;
}
std::vector<FiniteElement> MeshLoader::Get_FE_by_Edge_Slow(unsigned int id1, unsigned int id2) const {
	std::vector<FiniteElement> founded;
	if (isVertex(id1, id2))
	{
		auto cur = FE_Container.begin();
		auto end = FE_Container.end();
		while (cur != end) {
			cur = std::find_if(cur, end, [id1, id2](FiniteElement cur_fe) {
				return std::find(cur_fe.nodes_id.begin(), cur_fe.nodes_id.end(), id1) != cur_fe.nodes_id.end()
					&& std::find(cur_fe.nodes_id.begin(), cur_fe.nodes_id.end(), id2) != cur_fe.nodes_id.end(); });
			if (cur != end) {
				founded.push_back(*cur);
				++cur;
			}
		}
	}
	return founded;
}

//	4) метод, возвращающий контейнер граничных узлов по ID границы;
std::vector<Node> MeshLoader::Get_BoundaryNodes_by_borderId(unsigned int id) const
{
	std::set<Node> founded;
	for (auto iter = BFE_Container.begin(); iter != BFE_Container.end(); ++iter)
	{
		if ((*iter).border_id == id)
			for (auto l_iter = (*iter).nodes_id.begin(); l_iter != (*iter).nodes_id.end(); l_iter++)
			{
				if (isVertex(*l_iter))
					founded.insert(Nodes_Container[(*l_iter)]);
			}
	}
	return std::vector<Node>(founded.begin(), founded.end());
}

//	5) метод, возвращающий контейнер КЭ с заданным ID материала;
std::vector<FiniteElement> MeshLoader::Get_FE_by_matId(unsigned int id) const
{
	std::vector<FiniteElement> KE_container;
	for (auto iter = FE_Container.begin(); iter != FE_Container.end(); ++iter)
	{
		if ((*iter).mat_id == id)
			KE_container.push_back(*iter);
	}
	return KE_container;
}

//	6) метод, возвращающий контейнер граничных КЭ с заданным ID границы;
std::vector<BoundaryFiniteElement> MeshLoader::Get_BFE_by_borderId(unsigned int id) const
{
	std::vector<BoundaryFiniteElement> KE_container;
	for (auto iter = BFE_Container.begin(); iter != BFE_Container.end(); ++iter)
	{
		if ((*iter).border_id == id)
			KE_container.push_back(*iter);
	}
	return KE_container;
}

//	7) метод, производящий вставку новых узлов в середины ребер существующих КЭ (включая граничные КЭ).
void MeshLoader::Add_NewNodes()
{
	if (isVertex(FE_Container[1]))
	{
		//копии контейнеров для отладки
		std::vector<Node> new_Nodes_Container(Nodes_Container.begin(), Nodes_Container.end());
		std::vector<FiniteElement> new_FE_Container(FE_Container.begin(), FE_Container.end());
		std::vector<BoundaryFiniteElement> new_BFE_Container(BFE_Container.begin(), BFE_Container.end());

		std::unordered_set<Edge> Edges;
		std::vector<std::vector<unsigned int>> n = { {0,1},{0,2},{0,3},{1,2},{1,3},{2,3} };
		std::vector<std::vector<unsigned int>> bn = { {0,1},{0,2},{1,2} };
		unsigned int size = (unsigned int)new_Nodes_Container.size();

		//заполнение хеш таблицы ребер и контейнера новых узлов
		for (auto iter = ++FE_Container.begin(); iter != FE_Container.end(); ++iter)
		{
			auto& elem = *iter;
			for (unsigned int i = 0; i < n.size(); i++)
			{
				unsigned int n1 = n[i][0], n2 = n[i][1];
				Edge new_edge(elem.nodes_id[n1], elem.nodes_id[n2], size);
				if (Edges.find(new_edge) == Edges.end())
				{
					Node new_node(size++,
						(Nodes_Container[elem.nodes_id[n1]].x1 + Nodes_Container[elem.nodes_id[n2]].x1) / 2,
						(Nodes_Container[elem.nodes_id[n1]].x2 + Nodes_Container[elem.nodes_id[n2]].x2) / 2,
						(Nodes_Container[elem.nodes_id[n1]].x3 + Nodes_Container[elem.nodes_id[n2]].x3) / 2,
						false);
					new_Nodes_Container.push_back(new_node);
					Edges.insert(new_edge);
				}
			}
		}
		//создание нового контенера с дополненными кэ 
		for (auto iter = ++new_FE_Container.begin(); iter != new_FE_Container.end(); ++iter)
		{
			auto& elem = *iter;
			for (unsigned int i = 0; i < n.size(); i++)
			{
				auto n1 = n[i][0], n2 = n[i][1];
				Edge cur_edge(elem.nodes_id[n1], elem.nodes_id[n2], 0);
				elem.nodes_id.push_back((*Edges.find(cur_edge)).center_node_id);
			}
		}
		//создание нового контейнера с дополненными граничными кэ 
		for (auto iter = ++new_BFE_Container.begin(); iter != new_BFE_Container.end(); ++iter)
		{
			auto& elem = *iter;
			for (unsigned int i = 0; i < bn.size(); i++)
			{
				auto bn1 = bn[i][0], bn2 = bn[i][1];
				Edge cur_edge(elem.nodes_id[bn1], elem.nodes_id[bn2], 0);
				elem.nodes_id.push_back((*Edges.find(cur_edge)).center_node_id);
			}
		}
		//Перемещение новых контейнеров на место старых
		Nodes_Container = std::move(new_Nodes_Container);
		FE_Container = std::move(new_FE_Container);
		BFE_Container = std::move(new_BFE_Container);
	}
	else std::cout << "Добавление узлов уже было произведено\n";
}

//	8) метод, создающий контейнер, n - ый элемент которого хранит контейнер всех «соседей» по ребрам для узла n;
std::vector<std::vector<Node>> MeshLoader::Get_AdjacentNodesContainer()
{
	if (AdjacentNodesContainer.empty())
	{
		std::vector<std::set<unsigned int>> Nodes_NeighboursIDs(Nodes_Container.size()); //vector<set>
		for (auto& Elem : FE_Container)
		{
			for (auto id1 : Elem.nodes_id)
			{
				for (auto id2 : Elem.nodes_id)
				{
					if (isVertex(id1, id2))
						Nodes_NeighboursIDs[id1].insert(id2);
				}
				Nodes_NeighboursIDs[id1].erase(id1);
			}
		}
		std::vector<std::vector<Node>> fAdjacentNodesContainer(1, std::vector<Node>());
		fAdjacentNodesContainer.reserve(Nodes_Container.size());
		fAdjacentNodesContainer.resize(Nodes_Container.size());
		for (unsigned int i = 1; i < Nodes_Container.size(); ++i) {
			std::vector<Node> Neighbours;
			for (unsigned int id : Nodes_NeighboursIDs[i])
			{
				if (isVertex(id))
					Neighbours.push_back(Nodes_Container[id]);
			}

			fAdjacentNodesContainer[i] = std::move(Neighbours);
		}
		AdjacentNodesContainer = std::move(fAdjacentNodesContainer);
	}
	return AdjacentNodesContainer;
}

//	9) методы, реализующие отформатированный вывод Node, Element и BoundaryFiniteElement на консоль;
void MeshLoader::Print_Nodes_Container() const
{
	int size = (int)Nodes_Container.size();
	if (size > 1)
	{
		std::cout << "Кол-во узлов: " << size - 1 << "\nРазмерность пр-ва: " << " 3\n";
		std::cout << std::setw(18) << std::right << "Координаты узла " << std::endl;
		for (int i = 1; i < size; i++)
		{
			std::cout << Nodes_Container[i];
		}
	}
}
void MeshLoader::Print_FE_Container() const
{
	int size = (int)FE_Container.size();
	if (size > 1)
	{
		std::cout << "Кол-во КЭ: " << size - 1 << "\nКол-во узлов в КЭ: " << FE_Container[1].nodes_id.size() << "\n";
		std::cout << std::setw(4) << std::right << "Мат." << std::setw(33) << std::right << "ID узлов			" << std::endl;
		for (int i = 1; i < size; i++)
		{
			std::cout << FE_Container[i];
		}
	}
}
void MeshLoader::Print_BFE_Container() const
{

	int size = (int)BFE_Container.size();
	if (size > 1)
	{
		std::cout << "Кол-во гр. КЭ: " << size - 1 << "\nКол-во узлов в гр. КЭ: " << BFE_Container[1].nodes_id.size() << "\n";
		std::cout << std::setw(4) << std::right << "Гр. " << std::setw(39) << std::right << "ID узлов			" << std::endl;
		for (int i = 1; i < size; i++)
		{
			std::cout << BFE_Container[i];
		}
	}
}
void MeshLoader::Print_MeshData() const
{
	Print_Nodes_Container();
	std::cout << std::endl;
	Print_FE_Container();
	std::cout << std::endl;
	Print_BFE_Container();
	std::cout << std::endl;
}

void MeshLoader::Print_Node(const Node& n) const
{
	std::cout << n;
}
void MeshLoader::Print_FiniteElement(const FiniteElement& fe) const
{
	std::cout << fe;
}
void MeshLoader::Print_BoundaryFiniteElement(const BoundaryFiniteElement& bfe) const
{
	std::cout << bfe;
}