#include <iostream>
#include <filesystem>
#include "AssetmanagerController.h"

#include <bit7z/bit7z.hpp>
#include <bit7z/bitarchivewriter.hpp>
#include <bit7z/bitarchivereader.hpp>
#include <bit7z/bitarchiveeditor.hpp>

void CompressToArchive(bit7z::BitArchiveWriter& Writearchive)
{
    // Adding the items to be compressed (no compression is performed here)
    Writearchive.addFile("C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/TestObjects/example.txt");
    Writearchive.addFile("C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/TestObjects/sample.tga");
    //Writearchive.addDirectory("C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/");

    // Compressing the added items to the output archive
    Writearchive.compressTo("C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/Compressed/output.7z");
}

void AddFileToArchive(bit7z::BitArchiveEditor& editorearchive)
{
    //editorearchive.setUpdateMode(bit7z::UpdateMode::Append);
    editorearchive.addFile("C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/TestObjects/TestJESON.json");
    editorearchive.applyChanges();
    //editorearchive.updateItem()
    //editorearchive.compressTo("C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/Compressed/output1.7z");
}

void RemoveFileFromArchive(bit7z::BitArchiveEditor& editorearchive)
{
    bit7z::tstring name = "TestJESON.json";

    editorearchive.deleteItem(name);

    try {
        editorearchive.applyChanges();
    }
    catch (const bit7z::BitException& ex)
    { /* Do something with ex.what()...*/
        auto x1 = ex.code();
        auto x2 = ex.failedFiles();
        auto x3 = ex.what();
        std::cout << x3 << std::endl;
    }
}

void UncompressFiles(const bit7z::BitArchiveReader& Readarchive)
{
    // Testing the archive
    Readarchive.test();
    // Extracting the archive
    Readarchive.extractTo("C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/UnCompressed");
}

void PrintArchiveMetadata(const bit7z::BitArchiveReader& Readarchive)
{
    std::cout << "Archive properties" << std::endl;
    std::cout << "  Items count: " << Readarchive.itemsCount() << std::endl;
    std::cout << "  Folders count: " << Readarchive.foldersCount() << std::endl;
    std::cout << "  Files count: " << Readarchive.filesCount() << std::endl;
    std::cout << "  Size: " << Readarchive.size() << std::endl;
    std::cout << "  Packed size: " << Readarchive.packSize() << std::endl;
    std::cout << std::endl;

    // Printing the metadata of the archived items
    std::cout << "Archived items";
    auto arc_items = Readarchive.items();
    for (auto& item : arc_items) {
        std::cout << std::endl;
        std::cout << "  Item index: " << item.index() << std::endl;
        std::cout << "    Name: " << item.name() << std::endl;
        std::cout << "    Extension: " << item.extension() << std::endl;
        std::cout << "    Path: " << item.path() << std::endl;
        std::cout << "    IsDir: " << item.isDir() << std::endl;
        std::cout << "    Size: " << item.size() << std::endl;
        std::cout << "    Packed size: " << item.packSize() << std::endl;
        std::cout << "    CRC: " << item.crc() << std::endl;
    }
}

void updateLoop()
{
    bit7z::Bit7zLibrary lib{ "7z.dll" };
    bit7z::BitArchiveWriter Writearchive{ lib, bit7z::BitFormat::SevenZip };
    /*
          // Adding the items to be compressed (no compression is performed here)
         archive.addFile("C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/example.txt");
         archive.addDirectory("C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/");

         // Compressing the added items to the output archive
         archive.compressTo("output.7z");
         */
    try {
        //CompressToArchive(Writearchive);
    }
    catch (const bit7z::BitException& ex)
    { /* Do something with ex.what()...*/
        auto x1 = ex.code();
        auto x2 = ex.failedFiles();
        auto x3 = ex.what();
        std::cout << x3 << std::endl;
    }

    /*-------------------------------------------------------------------------------*/
    bit7z::tstring inFile = "C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/Compressed/output.7z";
    bit7z::BitArchiveEditor editorearchive(lib, inFile, bit7z::BitFormat::SevenZip);

    try {
       //AddFileToArchive(editorearchive);
    }
    catch (const bit7z::BitException& ex)
    { /* Do something with ex.what()...*/
        auto x1 = ex.code();
        auto x2 = ex.failedFiles();
        auto x3 = ex.what();
        std::cout << x3 << std::endl;
    }

    /*-------------------------------------------------------------------------------*/
    //bit7z::BitArchiveReader Readarchive{ lib, "path/to/archive.gz", bit7z::BitFormat::GZip };
   // bit7z::BitArchiveReader Readarchive{ lib,"C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/Compressed/output1.7z", bit7z::BitFormat::SevenZip };

    /*
          // Testing the archive
         Readarchive.test();
         // Extracting the archive
         Readarchive.extractTo("C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/UnCompressed");
         */
    try {
        //UncompressFiles(Readarchive);
    }
    catch (const bit7z::BitException& ex)
    { /* Do something with ex.what()...*/
        auto x1 = ex.code();
        auto x2 = ex.failedFiles();
        auto x3 = ex.what();
        std::cout << x3 << std::endl;
    }

    /*-------------------------------------------------------------------------------*/
    bit7z::tstring File = "C:/Users/acer/OneDrive/Documents/VS19_project/UTest/UTest/Compressed/output.7z";
    bit7z::BitArchiveEditor Reditorearchive(lib, File, bit7z::BitFormat::SevenZip);
    try {
        RemoveFileFromArchive(Reditorearchive);
    }
    catch (const bit7z::BitException& ex)
    { /* Do something with ex.what()...*/
        auto x1 = ex.code();
        auto x2 = ex.failedFiles();
        auto x3 = ex.what();
        std::cout << x3 << std::endl;
    }

    /*-------------------------------------------------------------------------------*/
   // PrintArchiveMetadata(Readarchive);
    /*-------------------------------------------------------------------------------*/

}

int main() 
{
   //updateLoop();

    //WIKI - https://rikyoz.dev/bit7z/

    //STATIC LIB - https://github.com/rikyoz/bit7z/releases/tag/v4.0.1

    View::AssetmanagerView ui;
    Model::AssetmanagerModel model;

   Controller::AssetmanagerController controller(&ui, &model);

   // Run the application using start application function 
   while (controller.StartApplication() == false)
   {

   }

   return 0;
}

#if 0
    namespace fs = std::filesystem;

    int main() {
        // Define the directory containing the ".tga" assets
        fs::path assetsDir = "assets";

        // Define the output archive file name
        std::string archiveName = "compressed_assets.7z";

        // Initialize the Bit7z archive
        bit7z::Bit7zArchive archive;

        try {
            // Open the output archive file for writing
            archive.create(archiveName);

            // Iterate through the ".tga" files in the assets directory
            for (const auto& entry : fs::directory_iterator(assetsDir)) {
                if (entry.is_regular_file() && entry.path().extension() == ".tga") {
                    // Add the ".tga" file to the archive
                    archive.addFile(entry.path().string(), entry.path().filename().string());
                }
            }

            // Finalize and close the archive
            archive.finalize();
            archive.close();

            std::cout << "Compression complete. Archive saved as: " << archiveName << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

        return 0;
    }
#endif // 0



