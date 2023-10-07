#include <set>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <optional>
#include <vector>
#include "AssetmanagerController.h"
#include <bit7z/BitArchiveItemInfo.hpp>

using namespace Controller;

void AssetmanagerController::PrintStartMessage()
{
    m_pUI->ClearScreen();
    m_pUI->PrintOnScreen("Please select an option:", true);
    m_pUI->PrintOnScreen("1. Compress files", true);
    m_pUI->PrintOnScreen("2. Uncompress files", true);
    m_pUI->PrintOnScreen("3. Add asset", true);
    m_pUI->PrintOnScreen("4. Remove asset", true);
    m_pUI->PrintOnScreen("5. List all assets with metadata", true);
    m_pUI->PrintOnScreen("6. Quit", true);
}

void AssetmanagerController::PrintFinalMessage() 
{
    m_pUI->PrintOnScreen("Press ENTER to continue.....", true);
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
        m_pUI->PrintOnScreen("Invalid Option Selected", true);
        break;
    }

    PrintFinalMessage();
    return quit;
}

void AssetmanagerController::QuitApplication() {
    m_pUI->PrintOnScreen("Quitting!", true);
}

bool AssetmanagerController::IsSupportedExtension(const std::string& filePath)
{
    static std::set<std::string> supportedExtensions = { ".tga", ".wav", ".json", ".7z" };
    std::string fileExtension = std::filesystem::path(filePath).extension().string();
    return supportedExtensions.count(fileExtension) > 0;
}

bool AssetmanagerController::GetValidPath(const std::string& firstMessage, std::string& filepath)
{
    m_pUI->PrintOnScreen(firstMessage, true);
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
            m_pUI->PrintOnScreen("The provided path does not exist.Please entre file path again", true);
        }
    }
    else 
    {
        m_pUI->PrintOnScreen("Invalid file or unsupported extension. Please entre file path again", true);
    }

    filepath.clear();
    return false;
}

void AssetmanagerController::GetUserInput(const std::string& directoryPath, std::set<std::string>& files) 
{
    std::string path;
    if (GetValidPath("Please enter full path of the file you want to compress", path)) {
        auto result = files.insert(path);
        if (!result.second) {
            m_pUI->PrintOnScreen("File already exists, please check the path and enter a new one", true);
        }
    }

    do 
    {
        m_pUI->PrintOnScreen("Do you want to enter more files? Enter Y for Yes or N for No", true);
        m_pUI->GetInputString(path);

        while ((path != "Y") && (path != "N"))
        {
            m_pUI->PrintOnScreen("Invalid input. Please Enter Y to add more files or N to compress current selection", true);
            m_pUI->GetInputString(path);
        }
        if (path == "N") {
            break;
        }
        else {
           
            if (GetValidPath("Please enter full path of the file you want to compress", path)) {
                auto result = files.insert(path);
                if (!result.second) {
                    m_pUI->PrintOnScreen("File already exists, please check the path and enter a new one", true);
                }
            }
        }
    } while (true);

}

bool AssetmanagerController::HasSpecialCharacters(const std::string& str)
{
    return std::any_of(str.begin(), str.end(), [](char c) {
        return !std::isalnum(c) && c != '_';
        });
}

void AssetmanagerController::Compress() 
{
    std::set<std::string> files;
    std::string directoryPath;
    GetUserInput(directoryPath, files);
    if (files.empty())
    {
        m_pUI->PrintOnScreen("No files are selected for operation.", true);
        return;
    }

    directoryPath = std::filesystem::path(*(files.rbegin())).parent_path().string();

    std::string outputfilename;
    m_pUI->PrintOnScreen("Please enter the name of the output compressed file", true);

    do {
        m_pUI->GetInputString(outputfilename);
        if (HasSpecialCharacters(outputfilename)) {
            m_pUI->PrintOnScreen("File name contains special characters. Please enter a name without special characters", true);
            continue;
        }

        outputfilename = directoryPath + "/" + outputfilename + ".7z";

        if (std::filesystem::exists(outputfilename)) {
            m_pUI->PrintOnScreen("The file already exists at the specified path. Please enter a different name", true);
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

/*
void Controller::AssetmanagerController::Uncompress()
{
    std::string zipfile;
    std::string outputdir;

    if (GetValidPath(zipfile))
    {
       NEWFOLDER:
        m_pUI->PrintOnScreen("Please enter the path of folder where you want to uncompress", true);
        m_pUI->GetInputString(outputdir);
        while (!std::filesystem::is_directory(outputdir))
        {
            m_pUI->PrintOnScreen("Folder path is not valid. Please enter the valid folder path", true);
            m_pUI->GetInputString(outputdir);
        }
        if (!std::filesystem::is_empty(outputdir))
        {
            std::string wantToP;
            m_pUI->PrintOnScreen("Folder is not empty. Your files may be over ridden Do you want to proceed Y/N", true);
            m_pUI->GetInputString(wantToP);
            while ((wantToP != "Y") && (wantToP != "N"))
            {
                m_pUI->PrintOnScreen("Invalid input. Please Enter Y to proceed or N to enter new path", true);
                m_pUI->GetInputString(wantToP);
            }
            if ((wantToP == "N"))
            {
                goto NEWFOLDER;
            }
        }

        std::optional<std::string> error = m_pModel->UncompressFile(zipfile, outputdir);
        if (error.has_value())
        {
            m_pUI->PrintOnScreen(error.value(), true);
            return;
        }
        m_pUI->PrintOnScreen("Files has been successfuly Uncompressed. Please check the following directory path: " + outputdir, true);

    }
}
*/

void AssetmanagerController::Uncompress()
{
    std::string zipfile, outputdir;

    if (GetValidPath("Please enter full path of the 7z file you want to uncompress",zipfile))
    {
        bool validOutputDirEntered = false;

        while (!validOutputDirEntered)
        {
            m_pUI->PrintOnScreen("Please enter the path of the folder where you want to uncompress", true);
            m_pUI->GetInputString(outputdir);

            if (std::filesystem::is_directory(outputdir))
            {
                if (std::filesystem::is_empty(outputdir))
                {
                    validOutputDirEntered = true;
                }
                else
                {
                    std::string wantToProceed;
                    m_pUI->PrintOnScreen("Folder is not empty. Your files may be overwritten. Do you want to proceed Y/N", true);
                    m_pUI->GetInputString(wantToProceed);
                    while ((wantToProceed != "Y") && (wantToProceed != "N"))
                    {
                        m_pUI->PrintOnScreen("Invalid input. Please Enter Y to proceed or N to enter a new path", true);
                        m_pUI->GetInputString(wantToProceed);
                    }
                    if (wantToProceed == "Y")
                    {
                        validOutputDirEntered = true;
                    }
                }
            }
            else
            {
                m_pUI->PrintOnScreen("Folder path is not valid. Please enter a valid folder path", true);
            }
        }

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
}

void Controller::AssetmanagerController::AddAsset()
{
    std::set<std::string> files;
    std::string directoryPath, zipfile;

    GetUserInput(directoryPath, files);

    if (files.empty())
    {
        m_pUI->PrintOnScreen("No files are selected for operation.", true);
        return;
    }

    while (!GetValidPath("Please enter the path of 7z file you want to edit", zipfile)) {}

    /* m_pUI->PrintOnScreen("Please enter the name of the output compressed file", true);

    do {
        m_pUI->GetInputString(outputfilename);
        if (HasSpecialCharacters(outputfilename)) {
            m_pUI->PrintOnScreen("File name contains special characters. Please enter a name without special characters", true);
        }
        else
        {
            outputfilename = directoryPath + "/" + outputfilename + ".7z";

            if (std::filesystem::exists(outputfilename)) {
                m_pUI->PrintOnScreen("The file already exists at the specified path. Please enter a different name", true);
            }
            else {
                break;
            }
        }
    } while (true);*/

    std::optional<std::string> error = m_pModel->AddFileToArchive(zipfile, files);

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
    m_pUI->PrintOnScreen("Enter the name of file you want to delete", true);
    m_pUI->GetInputString(fileToDelete);
    
    m_pUI->PrintOnScreen("Enter the name of zip file you want to edit", true);
    m_pUI->GetInputString(zipfile);
   
    while (!std::filesystem::exists(zipfile))
    {
        m_pUI->PrintOnScreen("Entered file does not exist. Check the file and enter path again", true);
        m_pUI->GetInputString(zipfile);
    }

    std::optional<std::string> error = m_pModel->RemoveFileFromArchive(zipfile, fileToDelete);
    if (error.has_value())
    {
        m_pUI->PrintOnScreen(error.value(), true);
    }
    else
    {
        m_pUI->PrintOnScreen("File has been deleted successfully", true);
    }
}

void Controller::AssetmanagerController::ListAllAssetsWithMetadata()
{
    std::string zipfile;
    m_pUI->PrintOnScreen("Enter the name of zip file you want print", true);
    m_pUI->GetInputString(zipfile);

    while (!std::filesystem::exists(zipfile))
    {
        m_pUI->PrintOnScreen("Entered file does not exist. Check the file and enter path again", true);
        m_pUI->GetInputString(zipfile);
    }

    std::vector<std::unordered_map<std::string, std::string>> arc_items;
    std::optional<std::string> error = m_pModel->ListAllAssetsWithMetadata(zipfile, arc_items);
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

    m_pUI->PrintOnScreen("Archived items with metadata", true);
}

