#pragma once

#include "AssetmanagerView.h"
#include "AssetmanagerModel.h"

namespace Controller
{
    enum UserInputType {
        CompressFiles = 1, UncompressFiles, AddAsset, RemoveAsset, ListAllAssetsWithMetadata, QuitApplication
    };

    class FileOperations
    {
    public:
        bool IsValidFilePath(std::string& filepath, std::string& errorMessage);
        bool IsValidFolderPath(std::string& folderepath, std::string& errorMessage);
        bool Is7zFile(std::string& filepath, std::string& errorMessage);
        bool IsFolderEmpty(std::string& filepath);
        bool IsSupportedExtension(const std::string& filePath, std::string& errorMessage);
        bool HasSpecialCharacters(const std::string& str);
        bool IsFileExist(std::string& filepath);
        void GetCompleteFilename(const std::string& filepath, std::string& completeFilename);
        void GetDirectoryPath(const std::string& filepath, std::string& directoryPath);
    private:
    };

    class AssetmanagerController
    {
    public:
        AssetmanagerController(View::AssetmanagerView* ui, Model::AssetmanagerModel* model)
            : m_pUI(ui), m_pModel(model),m_pFileOper(new FileOperations()) {}

        ~AssetmanagerController()
        {
            delete m_pFileOper;
        }

        bool StartApplication();
        void QuitApplication();

    private:
        void PrintStartMessage();
        void PrintFinalMessage();
        void GetInputToCompress(std::set<std::string>& files);
        void GetInputToUncompress(std::string& zipFile, std::string& directoryPath);
        void GetValidArchivePath(std::string& zipFile);
        void Compress();
        void Uncompress();
        void AddAsset();
        void RemoveAsset();
        void ListAllAssetsWithMetadata();

        View::AssetmanagerView* m_pUI;
        Model::AssetmanagerModel* m_pModel;
        FileOperations* m_pFileOper;
    };
}
