#pragma once

namespace hooks
{
	extern std::vector<const char*> renderTypeNames;

	void CheckType(int renderType, const char* typeName);
	int Init(int index);
	bool InitImages(ID3D11Device* pDevice);

}