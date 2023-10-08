#include "AssetmanagerController.h"
#include <bit7z/BitArchiveItemInfo.hpp>

#include <set>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <optional>
#include <vector>

using namespace Controller;

void AssetmanagerController::PrintStartMessage()
{
    m_pUI->ClearScreen();
    m_pUI->PrintOnScreen("Please select an option:");
    m_pUI->PrintOnScreen("1. Compress files");
    m_pUI->PrintOnScreen("2. Uncompress files");
    m_pUI->PrintOnScreen("3. Add asset");
    m_pUI->PrintOnScreen("4. Remove asset");
    m_pUI->PrintOnScreen("5. List all assets with metadata");
    m_pUI->PrintOnScreen("6. Quit");
}

void AssetmanagerController::PrintFinalMessage() 
{
    m_pUI->PrintOnScreen("Press ENTER to continue.....");
    m_pUI->WaitForUserInput();
}

bool AssetmanagerController::StartApplication()
{
    bool quit = false;
    PrintStartMessage();
    int UserInput;
    m_pUI->GetInputInteger(UserInput);

    switch (UserInput)
    {
    case UserInputType::CompressFiles:
        Compress();
        break;
    case UserInputType::UncompressFiles:
        Uncompress();
        break;
    case UserInputType::AddAsset:
        AddAsset();
        break;
    case UserInputType::RemoveAsset:
        RemoveAsset();
        break;
    case UserInputType::ListAllAssetsWithMetadata:
        ListAllAssetsWithMetadata();
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

void AssetmanagerController::QuitApplication() {
    m_pUI->PrintOnScreen("Quitting!");
}

/*

bool AssetmanagerController::IsSupportedExtension(const std::string& filePath)
{
    static std::set<std::string> supportedExtensions = { ".tga", ".wav", ".json", ".7z" };
    std::string fileExtension = std::filesystem::path(filePath).extension().string();
    return supportedExtensions.count(fileExtension) > 0;
}

bool AssetmanagerController::GetValidPath(const std::string& firstMessage, std::string& filepath)
{
    m_pUI->PrintOnScreen(firstMessage);
    m_pUI->GetInputString(filepath);

    if (std::filesystem::is_regular_file(filepath) && IsSupportedExtension(filepath))
    {
        if (std::filesystem::exists(filepath))
        {
            std::replace(filepath.begin(), filepath.end(), '\\', '/');
            return true;
        }
        else
        {
            m_pUI->PrintOnScreen("The provided path does not exist.Please entre file path again");
        }
    }
    else
    {
        m_pUI->PrintOnScreen("Invalid file or unsupported extension. Please entre file path again");
    }

    filepath.clear();
    return false;
}

bool AssetmanagerController::HasSpecialCharacters(const std::string& str)
{
    return std::any_of(str.begin(), str.end(), [](char c) {
        return !std::isalnum(c) && c != '_';
        });
}

*/

void AssetmanagerController::Compress() 
{
    std::set<std::string> files;
    std::string directoryPath;
    GetInputToCompress(directoryPath, files);
    if (files.empty())
    {
        m_pUI->PrintOnScreen("No files are selected for operation.");
        return;
    }

    directoryPath = std::filesystem::path(*(files.rbegin())).parent_path().string();

    std::string outputfilename;
    m_pUI->PrintOnScreen("Please enter the name of the output compressed file");

    do {
        m_pUI->GetInputString(outputfilename);
        if (m_pFileOper->HasSpecialCharacters(outputfilename)) {
            m_pUI->PrintOnScreen("Invalid input. Please enter valid file name.");
            continue;
        }

        outputfilename = directoryPath + "/" + outputfilename + ".7z";

        if (m_pFileOper->IsFileExist(outputfilename)) {
            m_pUI->PrintOnScreen("The file already exists at the specified path. Please enter a different name");
        }
        else {
            break;
        }
    } while (true);

    std::optional<std::string> error = m_pModel->compressFile(files, outputfilename);
    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
        return;
    }
    m_pUI->PrintOnScreen("Files has been successfuly compressed. Please check the following directory path: " + directoryPath, true);
}

void AssetmanagerController::Uncompress()
{
    std::string zipfile, outputdir;

    GetInputToUncompress(zipfile, outputdir);

    std::optional<std::string> error = m_pModel->UncompressFile(zipfile, outputdir);
    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
    }
    else
    {
        m_pUI->PrintOnScreen("Files have been successfully uncompressed. Please check the following directory path: " + outputdir, true);
    }

}

void Controller::AssetmanagerController::AddAsset()
{
    std::set<std::string> filesToAdd;
    std::string directoryPath, zipFile;
    GetInputToCompress(directoryPath, filesToAdd);
    if (filesToAdd.empty())
    {
        m_pUI->PrintOnScreen("No files are selected for operation.");
        return;
    }

    GetValidArchivePath(zipFile);
  
    for (auto it = filesToAdd.begin(); it != filesToAdd.end();)
    {
        std::string filename;
        m_pFileOper->GetCompleteFilename(*it, filename);
        auto [Rerror, fileAlreadyExist] = m_pModel->ArchiveContainsFile(zipFile, filename);
        if (!Rerror.empty())
        {
            m_pUI->PrintOnScreen(Rerror);
        }
        if (fileAlreadyExist)
        {
            m_pUI->PrintOnScreen("File " + filename+ " already exist in archive.Rename the file and try again later.");
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
        m_pUI->PrintOnScreen("Files have been successfully added to the archive. Please check the following directory path: " + directoryPath, true);
    }
}

void Controller::AssetmanagerController::RemoveAsset()
{
    std::string fileToDelete, zipfile;
    m_pUI->PrintOnScreen("Enter the name of file you want to delete");
    m_pUI->GetInputString(fileToDelete);

    GetValidArchivePath(zipfile);

    std::optional<std::string> error = m_pModel->RemoveFileFromArchive(zipfile, fileToDelete);
    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
    }
    else
    {
        m_pUI->PrintOnScreen("File has been deleted successfully");
    }
}

void Controller::AssetmanagerController::ListAllAssetsWithMetadata()
{
    std::string zipfile;

    GetValidArchivePath(zipfile);

    std::vector<std::unordered_map<std::string, std::string>> arc_items;
    std::optional<std::string> error = m_pModel->ArchiveDetailsWithMetadata(zipfile, arc_items);
    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
    }

    m_pUI->PrintOnScreen("Archive properties for the archive "+ std::filesystem::path(zipfile).filename().string(),true);

    for (auto& item : arc_items)
    {
        for (auto& [key, value] : item)
        {
            m_pUI->PrintOnScreen(key + " = " + value, true);
        }
        m_pUI->PrintOnScreen(" ", true);
    }

    m_pUI->PrintOnScreen("Archived items with metadata");
}

void AssetmanagerController::GetInputToCompress(const std::string& directoryPath, std::set<std::string>& files)
{
    std::string path, errorMessage;
    do
    {
        m_pUI->PrintOnScreen("Please enter the full path of the file you want to compress");
        m_pUI->GetInputString(path);

        if (m_pFileOper->IsValidFilePath(path, errorMessage) && m_pFileOper->IsSupportedExtension(path, errorMessage))
        {
            auto result = files.insert(path);
            if (!result.second)
            {
                m_pUI->PrintOnScreen("File already exists, please check the path and enter a new one");
            }
        }
        else
        {
            m_pUI->PrintOnScreen(errorMessage);
        }

        m_pUI->PrintOnScreen("Do you want to enter more files? Enter Y for Yes or N for No");
        m_pUI->GetInputString(path);

        while ((path != "Y") && (path != "N"))
        {
            m_pUI->PrintOnScreen("Invalid input. Please Enter Y to add more files or N to compress the current selection");
            m_pUI->GetInputString(path);
        }

    } while (path == "Y");
}

void Controller::AssetmanagerController::GetInputToUncompress(std::string& zipFile, std::string& directoryPath)
{
   std::string errorMessage;

   GetValidArchivePath(zipFile);

   // Validate directoryPath
   while (true)
   {
       m_pUI->PrintOnScreen("Please enter the path of the folder where you want to uncompress");
       m_pUI->GetInputString(directoryPath);

       bool validDirPath = m_pFileOper->IsValidFolderPath(directoryPath, errorMessage);

       if (!validDirPath)
       {
           m_pUI->PrintOnScreen(errorMessage);
           continue;
       }

       if (!m_pFileOper->IsFolderEmpty(directoryPath))
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

void Controller::AssetmanagerController::GetValidArchivePath(std::string& zipFile)
{
    std::string errorMessage;

    // Validate zipFile path
    while (true)
    {
        m_pUI->PrintOnScreen("Please enter the full path of the 7z file you want to uncompress");
        m_pUI->GetInputString(zipFile);
        bool validZipPath = m_pFileOper->IsValidFilePath(zipFile, errorMessage) && m_pFileOper->Is7zFile(zipFile, errorMessage);

        if (validZipPath)
            break;

        m_pUI->PrintOnScreen(errorMessage);
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

    errorMessage = "The file does not exist. Please enter the file path again.";
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

bool Controller::FileOperations::IsFolderEmpty(std::string& filepath)
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




