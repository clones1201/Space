#include <fstream>
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>

#include <atlbase.h>

#include "rapidjson/writer.h"
#include "rapidjson/document.h"

#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace std;
using namespace rapidjson;
 
int main(int argc, char*argv[])
{
	try
	{
		std::string path = argv[1];
		std::string filename = std::string(argv[2]);

		std::fstream tempFile((path + "/" + filename).c_str(), std::ios_base::in);
		if (!tempFile.is_open())
		{
			throw exception("Missing Template File");
		}
		std::istreambuf_iterator<char> beg(tempFile), end;
		std::string json(beg, end);

		Document dc;
		dc.Parse(json.c_str());

		Value& material = dc["Material"];
		if (material.IsNull())
		{
			throw exception("No material");
		}

		Value& shaders = material["Shaders"];
		if (shaders.IsNull())
		{
			throw exception("No Shaders");
		}
		if (!shaders.IsArray())
		{
			throw exception("errors in Shaders");
		}
		for (auto iter = shaders.Begin(); iter != shaders.End(); ++iter)
		{
			Value& shader = *iter;
			Value& SourceFileName = shader["SourceFile"];
			if (SourceFileName.IsNull() || !SourceFileName.IsString())
				continue;

			Value& Target = shader["Target"];
			if (Target.IsNull() || !Target.IsString())
				continue;

			std::string suffix = std::string(Target.GetString());
			suffix = suffix.substr(2, suffix.size());

			Value& VertexShaderEntry = shader["VertexShader"];
			if (VertexShaderEntry.IsNull() || !VertexShaderEntry.IsString())
				continue;

			Value& PixelShaderEntry = shader["PixelShader"];
			if (PixelShaderEntry.IsNull() || !PixelShaderEntry.IsString())
				continue;
			
			std::fstream source(
				(path + "/" + SourceFileName.GetString()).c_str(),
				std::ios_base::in);
			if (!source.is_open())
			{
				throw exception("Missing Template File");
			}
			std::istreambuf_iterator<char> beg(source), end;
			std::string sourceCode(beg, end);

			CComPtr<ID3DBlob> pVSBlob = nullptr;
			CComPtr<ID3DBlob> pPSBlob = nullptr;
			CComPtr<ID3DBlob> pErrorBlob = nullptr;
			HRESULT hr = D3DCompile(
				sourceCode.c_str(), sourceCode.size(), SourceFileName.GetString(),
				nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, VertexShaderEntry.GetString(),
				("vs" + suffix).c_str(),
				D3DCOMPILE_OPTIMIZATION_LEVEL3,
				0,
				&pVSBlob,
				&pErrorBlob
				);
			if (FAILED(hr))
			{
				pVSBlob = nullptr;
				std::cout << (char*)pErrorBlob->GetBufferPointer() << endl;
			}
			pErrorBlob = nullptr;
			hr = D3DCompile(
				sourceCode.c_str(), sourceCode.size(), SourceFileName.GetString(),
				nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, PixelShaderEntry.GetString(),
				("ps" + suffix).c_str(),
				D3DCOMPILE_OPTIMIZATION_LEVEL3,
				0,
				&pPSBlob,
				&pErrorBlob
				);

			if (FAILED(hr))
			{
				pPSBlob = nullptr;
				std::cout << (char*)pErrorBlob->GetBufferPointer() << endl;
			}

			if (pPSBlob != nullptr && pVSBlob != nullptr)
			{
				wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t> converter;
				wstring wpath = converter.from_bytes(path + SourceFileName.GetString());
				D3DWriteBlobToFile(pVSBlob, (wpath + L"_vs.cso").c_str(), true);
				D3DWriteBlobToFile(pPSBlob, (wpath + L"_ps.cso").c_str(), true);
			}
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << endl;
		return -1;
	}
	return 0;
}