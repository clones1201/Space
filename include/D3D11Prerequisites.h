#ifndef __SPACE_D3D11PREREQUISITES_H__
#define __SPACE_D3D11PREREQUISITES_H__

#include "Prerequisites.h"
#include <D3D11.h>
#include <D3DX11.h>
#include <D3D11Shader.h>

#ifndef _DEBUG
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#else
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "d3dx11d.lib")
#pragma comment(lib, "d3dcompiler.lib")
#endif

// Define versions for if DirectX is in use (Win32 only)
#define DIRECT3D_VERSION 0x1100

// some D3D commonly used macros
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#endif