#include "../stdafx.h"

bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
bool LoadTextureFromFileDX9(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);
