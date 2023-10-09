**INTRODUCTION**

This asset management tool offers comprehensive capabilities for the efficient handling and storage of assets. It currently provides support for a variety of file formats, including '.tga,' '.wav,' and '.json,' and facilitates their storage within '.7z' compressed files. Key functionalities of this asset manager include the ability to: “Compress Selected Files, Uncompress Compressed Files, Add Files to Compressed Files, Remove Files from Compressed Files, List Details with Metadata”

**BRIEF DESCRIPTION** 

1.	Compression algorithm
Now it is important to use right compression format and compression library. After extensive search I found “bit7z”. bit7z is a cross-platform C++ static library that allows the compression/extraction of archive files through a clean and simple wrapper interface to the dynamic libraries from the 7-zip project. It supports compression and extraction to and from the filesystem or the memory, reading archives metadata, updating existing ones, creating multi-volume archives, operation progress callbacks, and many other functionalities.
Bit7z also supports compression and extraction in different format, updating existing file archive. Renaming, updating, or deleting old items in existing file archives. And most important testing archives for errors and receive error while reading or writing into archive to prevent inconsistent state.
2.	CLI or GUI interface
I arrived at the conclusion that implementing an asset manager with a Command-Line Interface (CLI) was the most appropriate course of action. This decision marked a pivotal moment in the project's development, streamlining subsequent processes and contributing to a more effective and efficient solution.
3.	Architecture and code clarity
In pursuit of code clarity and future-proofing, the Model-View-Controller (MVC) design pattern was judiciously employed. This architectural choice facilitates the clear separation of user interface (UI) elements and data handling components, offering a flexible foundation for potential code enhancements.
Below is a concise overview of the key classes within this structured framework:
1. View::AssetManagerView: This class, inheriting from IAssetManagerView, serves as the interface for user interaction, efficiently gathering user input and displaying output on the screen.
2. Model::AssetManagerModel: Responsible for direct interaction with the 'bit7z' library, this class is the backbone of the application, overseeing tasks such as archive creation, reading, and editing.
3. Controller::AssetManagementController: Positioned within the controller namespace, this pivotal class takes on the role of initializing the application and coordinating the various components. It orchestrates the collaboration between the UI, archive operations, file operations, and input/output operations.
4. Controller::FileOperations: Also situated within the controller namespace, this class specializes in file-related operations. It diligently handles tasks such as validating file paths, folder paths, and verifying file extensions to ensure the integrity of the asset management process.
5. Controller::IOOperation: This controller namespace class is entrusted with gathering user input pertaining to compression and decompression operations, ensuring seamless communication between the user and the application.
6. Controller::ArchiveOperation: As another component within the controller namespace, this class takes on the vital role of validating user-provided data before forwarding it to the model for further processing, ensuring data integrity throughout the application's workflow.
By adhering to the MVC design pattern and embodying the SOLID principles in the design of these classes, the codebase achieves a robust foundation, poised for future expansion and maintenance while enhancing overall code clarity and maintainability.
