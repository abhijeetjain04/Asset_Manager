#include "AssetmanagerController.h"
#include <bit7z/BitArchiveItemInfo.hpp>

#include <stdlib.h>
#include <set>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <optional>
#include <vector>

using namespace Controller;

Controller::AssetManagentController::AssetManagentController(View::AssetmanagerView* ui, Controller::ArchiveOperation* archiveOperation,
    Controller::FileOperations* fileOperation,
    Controller::IOOperation* ioOperation):m_pUI(ui), m_ArchiveOperation(archiveOperation),m_FileOperation(fileOperation),
m_IoOperation(ioOperation)
{
}

Controller::ArchiveOperation::ArchiveOperation(View::AssetmanagerView* ui, Model::AssetmanagerModel* model, Controller::FileOperations* fileOperation,
    Controller::IOOperation* ioOperation):m_pUI(ui),m_pModel(model), m_pFileOperation(fileOperation), m_pIoOperation(ioOperation)
{
}

Controller::IOOperation::IOOperation(View::AssetmanagerView* ui, Controller::FileOperations* fileOperation)
    : m_pUI(ui),m_pFileOperation(fileOperation)
{
}

bool Controller::AssetManagentController::StartApplication()
{
    bool quit = false;
    PrintStartMessage();
    int UserInput;
    m_pUI->GetInputInteger(UserInput);

    switch (UserInput)
    {
    case UserInputType::CompressFiles:
        m_ArchiveOperation->Compress();
        break;
    case UserInputType::UncompressFiles:
        m_ArchiveOperation->Uncompress();
        break;
    case UserInputType::AddAsset:
        m_ArchiveOperation->AddAsset();
        break;
    case UserInputType::RemoveAsset:
        m_ArchiveOperation->RemoveAsset();
        break;
    case UserInputType::PrintArchiveFilesWithMetadata:
        m_ArchiveOperation->PrintArchiveFilesWithMetadata();
        break;
    case UserInputType::QuitApplication:
        QuitApplication();
        quit = true;
        break;

    default:
        m_pUI->PrintOnScreen("Invalid Option Selected");
        break;
    }

    PrintFinalMessage();
    return quit;
}

void Controller::AssetManagentController::QuitApplication()
{
    m_pUI->PrintOnScreen("Quitting!");
    exit(3);
}

void Controller::AssetManagentController::PrintStartMessage()
{
    m_pUI->ClearScreen();
    m_pUI->PrintOnScreen("Please select an option:");
    m_pUI->PrintOnScreen("1. Compress files");
    m_pUI->PrintOnScreen("2. Uncompress files");
    m_pUI->PrintOnScreen("3. Add asset");
    m_pUI->PrintOnScreen("4. Remove asset");
    m_pUI->PrintOnScreen("5. List all files with metadata");
    m_pUI->PrintOnScreen("6. Quit");
}

void Controller::AssetManagentController::PrintFinalMessage()
{
    m_pUI->PrintOnScreen("Press ENTER to continue.....");
    m_pUI->WaitForUserInput();
}

void Controller::IOOperation::GetInputToCompress(std::set<std::string>& files)
{
    std::string inputFilePath, errorMessage;
    do
    {
        m_pUI->PrintOnScreen("Please enter the full path of the file");
        m_pUI->GetInputString(inputFilePath);

        if (m_pFileOperation->IsValidFilePath(inputFilePath, errorMessage) && m_pFileOperation->IsSupportedExtension(inputFilePath, errorMessage))
        {
            auto inserted = files.insert(inputFilePath);
            if (!inserted.second)
            {
                m_pUI->PrintOnScreen("File already exists, please check the path and enter a new one");
            }
        }
        else
        {
            m_pUI->PrintOnScreen(errorMessage);
        }

        m_pUI->PrintOnScreen("Do you want to add more files? Enter Y for Yes or N for No");
        m_pUI->GetInputString(inputFilePath);

        while ((inputFilePath != "Y") && (inputFilePath != "N"))
        {
            m_pUI->PrintOnScreen("Invalid input. Please Enter Y to add more files or N to continue");
            m_pUI->GetInputString(inputFilePath);
        }

    } while (inputFilePath == "Y");
}

void Controller::IOOperation::GetInputToUncompress(std::string& zipFile, std::string& directoryPath)
{
    std::string errorMessage;

   GetValidArchivePath(zipFile);

    // Validate directoryPath
    while (true)
    {
        m_pUI->PrintOnScreen("Please enter the path of the folder");
        m_pUI->GetInputString(directoryPath);

        bool validDirPath = m_pFileOperation->IsValidFolderPath(directoryPath, errorMessage);

        if (!validDirPath)
        {
            m_pUI->PrintOnScreen(errorMessage);
            continue;
        }

        if (!m_pFileOperation->IsEmptyFolder(directoryPath))
        {
            m_pUI->PrintOnScreen("(WARNING) The selected folder is not empty. Please enter Y to replace the files or N to enter a new path.");
            std::string wantToProceed;
            m_pUI->GetInputString(wantToProceed);

            while (wantToProceed != "Y" && wantToProceed != "N")
            {
                m_pUI->PrintOnScreen("Invalid input. Please enter Y to proceed or N to enter a new path");
                m_pUI->GetInputString(wantToProceed);
            }

            if (wantToProceed == "Y")
                break;
        }
        else
        {
            break;
        }
    }
}

void Controller::IOOperation::GetValidArchivePath(std::string& zipFile)
{
    std::string errorMessage;

    // Validate zipFile path
    while (true)
    {
        m_pUI->PrintOnScreen("Please enter the full path of the 7z folder");
        m_pUI->GetInputString(zipFile);
        bool validZipPath = m_pFileOperation->IsValidFilePath(zipFile, errorMessage) && m_pFileOperation->Is7zFile(zipFile, errorMessage);

        if (validZipPath)
            break;

        m_pUI->PrintOnScreen(errorMessage);
    }
}

void Controller::ArchiveOperation::Compress()
{
    std::set<std::string> inputFiles;
    m_pIoOperation->GetInputToCompress(inputFiles);
    if (inputFiles.empty())
    {
        m_pUI->PrintOnScreen("No files are selected for operation.");
        return;
    }

    std::string directoryPath;
    m_pFileOperation->GetDirectoryPath(*(inputFiles.rbegin()), directoryPath);

    std::string outputFileName;
    m_pUI->PrintOnScreen("Please enter the name of the output compressed file");

    do {
        m_pUI->GetInputString(outputFileName);
        if (m_pFileOperation->HasSpecialCharacters(outputFileName)) {
            m_pUI->PrintOnScreen("Invalid input. Please enter valid file name.");
            continue;
        }

        outputFileName = directoryPath + "/" + outputFileName + ".7z";

        if (m_pFileOperation->IsFileExist(outputFileName)) {
            m_pUI->PrintOnScreen("The file already exists at the specified path. Please enter a different name");
        }
        else {
            break;
        }
    } while (true);

    std::optional<std::string> error = m_pModel->compressFile(inputFiles, outputFileName);
    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
        return;
    }
    m_pUI->PrintOnScreen("Files has been successfuly compressed. Please check the following directory path: " + directoryPath, true);
}

void Controller::ArchiveOperation::Uncompress()
{
    std::string zipFile, outputDirectory;

    m_pIoOperation->GetInputToUncompress(zipFile, outputDirectory);

    std::optional<std::string> error = m_pModel->UncompressFile(zipFile, outputDirectory);
    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
    }
    else
    {
        m_pUI->PrintOnScreen("Files have been successfully uncompressed. Please check the following directory path: " + outputDirectory, true);
    }
}

void Controller::ArchiveOperation::AddAsset()
{
    std::set<std::string> filesToAdd;
    std::string directoryPath, zipFile;
    m_pIoOperation->GetInputToCompress(filesToAdd);
    if (filesToAdd.empty())
    {
        m_pUI->PrintOnScreen("No files are selected for operation.");
        return;
    }

    m_pIoOperation->GetValidArchivePath(zipFile);

    for (auto it = filesToAdd.begin(); it != filesToAdd.end();)
    {
        std::string fileName;
        m_pFileOperation->GetCompleteFilename(*it, fileName);
        auto [error, fileAlreadyExist] = m_pModel->ArchiveContainsFile(zipFile, fileName);
        if (!error.empty())
        {
            m_pUI->PrintOnScreen(error);
        }
        if (fileAlreadyExist)
        {
            m_pUI->PrintOnScreen("File " + fileName + " already exist in archive.Rename the file and try again later.");
            it = filesToAdd.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if (filesToAdd.empty())
    {
        m_pUI->PrintOnScreen("No files are selected for operation.");
        return;
    }

    std::optional<std::string> error = m_pModel->AddFileToArchive(zipFile, filesToAdd);

    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
    }
    else
    {
        m_pUI->PrintOnScreen("Files have been successfully added to the archive.", true);
    }
}

void Controller::ArchiveOperation::RemoveAsset()
{
    std::string fileToDelete, zipFile;
    m_pUI->PrintOnScreen("Enter the name of file you want to delete");
    m_pUI->GetInputString(fileToDelete);

    m_pIoOperation->GetValidArchivePath(zipFile);

    std::optional<std::string> error = m_pModel->RemoveFileFromArchive(zipFile, fileToDelete);
    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
    }
    else
    {
        m_pUI->PrintOnScreen("File has been deleted successfully");
    }
}

void Controller::ArchiveOperation::PrintArchiveFilesWithMetadata()
{
    std::string zipFile;

    m_pIoOperation->GetValidArchivePath(zipFile);

    std::vector<std::unordered_map<std::string, std::string>> archiveItems;
    std::optional<std::string> error = m_pModel->ArchiveDetailsWithMetadata(zipFile, archiveItems);
    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
    }

    m_pUI->PrintOnScreen("Archive properties for the archive " + std::filesystem::path(zipFile).filename().string(), true);

    for (auto& item : archiveItems)
    {
        for (auto& [key, value] : item)
        {
            m_pUI->PrintOnScreen(key + " = " + value, true);
        }
        m_pUI->PrintOnScreen(" ", true);
    }
}

bool FileOperations::IsValidFilePath(std::string& filepath, std::string& errorMessage)
{
    if (IsFileExist(filepath))
    {
        std::replace(filepath.begin(), filepath.end(), '\\', '/');
        errorMessage.clear();
        return true;
    }

    errorMessage = "Invalid input. Please enter the correct path again.";
    filepath.clear();
    return false;
}

bool Controller::FileOperations::Is7zFile(std::string& filepath, std::string& errorMessage)
{
    if (std::filesystem::path(filepath).extension() == ".7z")
    {
        return true;
    }
    else
    {
        errorMessage = "File is not 7z.Please enter the 7z file path again.";
        return false;
    }
}

bool Controller::FileOperations::IsEmptyFolder(std::string& filepath)
{
    return (std::filesystem::is_empty(filepath));
}

bool FileOperations::IsValidFolderPath(std::string& folderepath, std::string& errorMessage)
{
      if (std::filesystem::is_directory(folderepath))
      {
          return true;
      }
      else
      {
          errorMessage ="Folder path is not valid. Please enter correct path";
      }
      return false;
}

bool Controller::FileOperations::IsSupportedExtension(const std::string& filePath, std::string& errorMessage)
{
    static std::set<std::string> supportedExtensions = { ".tga", ".wav", ".json" };
    std::string fileExtension = std::filesystem::path(filePath).extension().string();
    if (supportedExtensions.count(fileExtension) > 0)
    {
        return true;
    }
    errorMessage = "Provided path does not exist or Unsupported extension. Please enter the file path again.";
    return false;

}

bool Controller::FileOperations::HasSpecialCharacters(const std::string& str)
{
    return std::any_of(str.begin(), str.end(), [](char c) {
        return !std::isalnum(c) && c != '_';
        });
}

bool Controller::FileOperations::IsFileExist(std::string& filepath)
{
    return std::filesystem::exists(filepath);
}

void Controller::FileOperations::GetCompleteFilename(const std::string& filepath, std::string& completeFilename)
{
    completeFilename = std::filesystem::path(filepath).filename().string();
}

void Controller::FileOperations::GetDirectoryPath(const std::string& filepath, std::string& directoryPath)
{
    directoryPath = std::filesystem::path(filepath).parent_path().string();
}

