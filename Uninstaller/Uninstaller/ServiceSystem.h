#include "StringPath.h"




#include "FileFolderGroup.h"

using namespace std;
class ServiceSystem
{
public:
	ServiceSystem();
	~ServiceSystem();

	static bool SecureCopyFile(String from, String to);
	static bool CreateLink(String OriginFile, String DestFolder, String DestNameLink);
	static bool SecureDeleteFile(String path);
	
	static void SecureDeleteFileForce(String path);
	
	static FileFolderGroup  SecureRemoveDirectory(String path);
	static void SecureRemoveFileDirectoryWait(FileFolderGroup path);
	static void SecureRemoveDirectoryForce(String path);

	static void killProcessByName(String filename);

	static StringPath GetCurrentExecutiblePath();

};

