#include "AssetmanagerController.h"
#include "AssetmanagerModel.h"
#include "AssetmanagerView.h"

int main(int, const char* [])
{
    //updateLoop();

    //WIKI - https://rikyoz.dev/bit7z/

    //STATIC LIB - https://github.com/rikyoz/bit7z/releases/tag/v4.0.1

    View::AssetmanagerView ui;
    Model::AssetmanagerModel model;
    Controller::FileOperations fileOper;
    Controller::IOOperation ioOper(&ui, &fileOper);
    Controller::ArchiveOperation archivOper(&ui, &model, &fileOper, &ioOper);

   Controller::AssetManagentController controller(&ui, &archivOper, &fileOper, &ioOper);

   // Run the application using start application function 
   while (controller.StartApplication() == false)
   {

   }

   return 0;
}




