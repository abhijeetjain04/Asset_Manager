#include "AssetmanagerModel.h"

#include <bit7z/bitarchivewriter.hpp>
#include <bit7z/Bit7zLibrary.hpp>

Model::AssetmanagerModel::AssetmanagerModel()
{
}

Model::AssetmanagerModel::~AssetmanagerModel()
{
}

#if 0
void Model::AssetmanagerModel::compressFile(wchar_t* pFile)
{
	bit7z::Bit7zLibrary lib("7z.dll");
	bit7z::BitArchiveWriter Writearchive(lib, bit7z::BitFormat::SevenZip);

	std::wstring ws(pFile);
	// your new String
	std::string str(ws.begin(), ws.end());
	pFile += wcslen(pFile) + 1;
	std::string path = str + std::string("\\");
	while (*pFile)
	{
		std::wstring file(pFile);
		std::string str1(file.begin(), file.end());
		std::string final = path + str1;
		//bit7z::tstring path (path + str1);
		Writearchive.addFile(final);
		//wprintf(L"%s\n", pFile);
		// Move to the next file name (if any)
		pFile += wcslen(pFile) + 1;
	}
	std::string output = str + "\\output.7z";
	Writearchive.compressTo(output);
}

#endif // 0


std::optional<std::string> Model::AssetmanagerModel::compressFile(std::set<std::string>& files, const std::string& output)
{
	bit7z::BitArchiveWriter Writearchive(m_lib, bit7z::BitFormat::SevenZip);

	try {
		for (auto& file : files)
		{
			Writearchive.addFile(file);
		}
		Writearchive.compressTo(output);
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}

}

std::optional<std::string> Model::AssetmanagerModel::UncompressFile(const std::string& zipfile, const std::string& outputdir)
{
	bit7z::BitArchiveReader Readarchive{ m_lib, zipfile, bit7z::BitFormat::SevenZip };
	try {
	// Testing the archive
	Readarchive.test();
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}
	// Extracting the archive
	Readarchive.extractTo(outputdir);
	return {};
}
