#pragma once
#include <list>
#include "String.h"

class FileFolderGroup
{

public:
	list<String> File;
	list<String> Dir;

	void Merge(FileFolderGroup ffg);
	void AddFile(String path);
	void AddDir(String path);

	FileFolderGroup();
	~FileFolderGroup();
};

