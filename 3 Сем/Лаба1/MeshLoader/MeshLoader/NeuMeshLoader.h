#pragma once
#include "MeshLoader.h"
#include "MeshDataTypes.h"

class NeuMeshLoader : public MeshLoader
{
public:
	virtual void LoadMesh(const std::string&) override;
};