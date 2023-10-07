#include "AssetmanagerModel.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <bit7z/Bit7zLibrary.hpp>
#include <bit7z/bitarchiveeditor.hpp>
#include <bit7z/bitarchivewriter.hpp>
#include <bit7z/BitArchiveItemInfo.hpp>

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
	try {
		bit7z::BitArchiveWriter Writearchive(m_lib, bit7z::BitFormat::SevenZip);
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
	return {};
}

std::optional<std::string> Model::AssetmanagerModel::UncompressFile(const std::string& zipfile, const std::string& outputdir)
{
	try {
	bit7z::BitArchiveReader Readarchive{ m_lib, zipfile, bit7z::BitFormat::SevenZip };
	// Testing the archive
	Readarchive.test();
	Readarchive.extractTo(outputdir);
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}
	// Extracting the archive
	return {};
}

std::optional<std::string> Model::AssetmanagerModel::AddFileToArchive(const std::string& zipfile, std::set<std::string>& filesToAdd)
{
	try {
		bit7z::BitArchiveEditor editorearchive(m_lib, zipfile, bit7z::BitFormat::SevenZip);
		for (auto& file : filesToAdd)
		{
			editorearchive.addFile(file);
		}
		//editorearchive.compressTo(outputdir);
		editorearchive.applyChanges();
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}
	return {};
}

std::optional<std::string> Model::AssetmanagerModel::RemoveFileFromArchive(const std::string& zipfile, const std::string& fileToDelete)
{
	try
	{
		bit7z::BitArchiveEditor editorearchive(m_lib, zipfile, bit7z::BitFormat::SevenZip);
		editorearchive.deleteItem(fileToDelete);
		editorearchive.applyChanges();
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}

	return {};
}

std::optional<std::string> Model::AssetmanagerModel::ListAllAssetsWithMetadata(const std::string& zipfile, std::vector<std::unordered_map<std::string, std::string>>& arc_items)
{
	try
	{
		bit7z::BitArchiveReader Readarchive{ m_lib,zipfile, bit7z::BitFormat::SevenZip };// add try catch
		std::unordered_map<std::string, std::string> properties;
		properties["ItemsCount"] = std::to_string(Readarchive.itemsCount());
		properties["FoldersCount"] = std::to_string(Readarchive.foldersCount());
		properties["FilesCount"] = std::to_string(Readarchive.filesCount());
		properties["Size"] = std::to_string(Readarchive.size());
		properties["PackedSize"] = std::to_string(Readarchive.packSize());
		arc_items.emplace_back(properties);

		std::unordered_map<std::string, std::string> eachItem;
		for (auto& item : Readarchive.items()) {
			eachItem["Meta data of element"] = item.name();
			eachItem["ItemIndex"]= std::to_string(item.index());
			eachItem["Extension"] = item.extension();
			eachItem["Path"]= item.path();
			eachItem["IsDir"]= std::to_string(item.isDir());
			eachItem["Size"]= std::to_string(item.size());
			eachItem["PackedSize"] = std::to_string(item.packSize());
			eachItem["CRC"]= std::to_string(item.crc());
			arc_items.emplace_back(eachItem);
			eachItem.clear();
		}
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}

	return {};
}
