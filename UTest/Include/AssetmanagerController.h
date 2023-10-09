#pragma once

#include "AssetmanagerView.h"
#include "AssetmanagerModel.h"

namespace Controller
{
    enum UserInputType {
        CompressFiles = 1, UncompressFiles, AddAsset, RemoveAsset, PrintArchiveFilesWithMetadata, QuitApplication
    };

    class FileOperations
    {
    public:
        FileOperations() = default;
        ~FileOperations() = default;
        bool IsValidFilePath(std::string& filepath, std::string& errorMessage);
        bool IsValidFolderPath(std::string& folderepath, std::string& errorMessage);
        bool Is7zFile(std::string& filepath, std::string& errorMessage);
        bool IsEmptyFolder(std::string& filepath);
        bool IsSupportedExtension(const std::string& filePath, std::string& errorMessage);
        bool HasSpecialCharacters(const std::string& str);
        bool IsFileExist(std::string& filepath);
        void GetCompleteFilename(const std::string& filepath, std::string& completeFilename);
        void GetDirectoryPath(const std::string& filepath, std::string& directoryPath);
    private:
    };

    class IOOperation
    {
    public:
        IOOperation(View::AssetmanagerView* ui, Controller::FileOperations* fileOperation);
        ~IOOperation() = default;
        void GetInputToCompress(std::set<std::string>& files);
        void GetInputToUncompress(std::string& zipFile, std::string& directoryPath);
        void GetValidArchivePath(std::string& zipFile);
    private:
        View::AssetmanagerView* m_pUI;
        Controller::FileOperations* m_pFileOperation;
    };

    class ArchiveOperation {
    public:
        ArchiveOperation(View::AssetmanagerView* ui, Model::AssetmanagerModel* model, Controller::FileOperations* fileOperation,
        Controller::IOOperation* ioOperation);
        ~ArchiveOperation() = default;
        void Compress();
        void Uncompress();
        void AddAsset();
        void RemoveAsset();
        void PrintArchiveFilesWithMetadata();
    private:
        View::AssetmanagerView* m_pUI;
        Model::AssetmanagerModel* m_pModel;
        Controller::FileOperations* m_pFileOperation;
        Controller::IOOperation* m_pIoOperation;
    };

    class AssetManagentController 
    {
    public:
        AssetManagentController(View::AssetmanagerView* ui,Controller::ArchiveOperation* archiveOperation,
            Controller::FileOperations* fileOperation,
            Controller::IOOperation* ioOperation);

        bool StartApplication();

    private:
        void PrintStartMessage();
        void PrintFinalMessage();
        void QuitApplication();

        View::AssetmanagerView* m_pUI;
        Controller::ArchiveOperation* m_ArchiveOperation;
        Controller::FileOperations* m_FileOperation;
        Controller::IOOperation* m_IoOperation;
    };    
}
