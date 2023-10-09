#include "AssetmanagerModel.h"
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <utility>


#include <bit7z/Bit7zLibrary.hpp>
#include <bit7z/bitarchiveeditor.hpp>
#include <bit7z/bitarchivewriter.hpp>
#include <bit7z/BitArchiveItemInfo.hpp>

/*
std::optional<std::string> Model::AssetmanagerModel::compressFile(const std::set<std::string>& files, const std::string& outputDirectory)
{
	try {
		bit7z::BitArchiveWriter Writearchive(m_lib, bit7z::BitFormat::SevenZip);
		for (auto& file : files)
		{
			Writearchive.addFile(file);
		}
		Writearchive.compressTo(outputDirectory);
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}
	return {};
}

*/

std::optional<std::string> Model::AssetmanagerModel::compressFile(const std::set<std::string>& files, std::string_view outputDirectory)
{
	try {
		bit7z::BitArchiveWriter Writearchive(m_lib, bit7z::BitFormat::SevenZip);
		for (auto& file : files)
		{
			Writearchive.addFile(file);
		}
		Writearchive.compressTo(outputDirectory.data());
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}
	return {};
}

std::optional<std::string> Model::AssetmanagerModel::UncompressFile(const std::string& zipFile, const std::string_view outputDirectory)
{
	try {
	bit7z::BitArchiveReader Readarchive{ m_lib, zipFile, bit7z::BitFormat::SevenZip };
	// Testing the archive
	Readarchive.test();
	Readarchive.extractTo(outputDirectory.data());
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}
	// Extracting the archive
	return {};
}

std::optional<std::string> Model::AssetmanagerModel::AddFileToArchive(const std::string& zipFile, std::set<std::string>& filesToAdd)
{
	try {
		bit7z::BitArchiveEditor editorearchive(m_lib, zipFile, bit7z::BitFormat::SevenZip);
		for (auto& file : filesToAdd)
		{
			editorearchive.addFile(file);
		}
		//editorearchive.compressTo(outputDirectory);
		editorearchive.applyChanges();
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}
	return {};
}

std::optional<std::string> Model::AssetmanagerModel::RemoveFileFromArchive(const std::string& zipFile, const std::string& fileToDelete)
{
	try
	{
		bit7z::BitArchiveEditor editorearchive(m_lib, zipFile, bit7z::BitFormat::SevenZip);
		editorearchive.deleteItem(fileToDelete);
		editorearchive.applyChanges();
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}

	return {};
}

std::optional<std::string> Model::AssetmanagerModel::ArchiveDetailsWithMetadata(const std::string& zipFile, std::vector<std::unordered_map<std::string, std::string>>& archiveItems)
{
	try
	{
		bit7z::BitArchiveReader Readarchive{ m_lib,zipFile, bit7z::BitFormat::SevenZip };
		std::unordered_map<std::string, std::string> properties;
		properties["ItemsCount"] = std::to_string(Readarchive.itemsCount());
		properties["FoldersCount"] = std::to_string(Readarchive.foldersCount());
		properties["FilesCount"] = std::to_string(Readarchive.filesCount());
		properties["Size"] = std::to_string(Readarchive.size());
		properties["PackedSize"] = std::to_string(Readarchive.packSize());
		archiveItems.emplace_back(properties);

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
			archiveItems.emplace_back(eachItem);
			eachItem.clear();
		}
	}
	catch (const bit7z::BitException& ex)
	{
		return ex.what();
	}

	return {};
}

std::pair<std::string, bool> Model::AssetmanagerModel::ArchiveContainsFile(const std::string& zipFile, std::string& Files)
{
	bool fileAlreadyExist = false;
	try
	{
		bit7z::BitArchiveReader Readarchive{ m_lib, zipFile, bit7z::BitFormat::SevenZip };
		fileAlreadyExist = Readarchive.contains(Files);
	}
	catch (const bit7z::BitException& ex)
	{
		return { ex.what(),fileAlreadyExist };
	}

	return {"",fileAlreadyExist };
}


