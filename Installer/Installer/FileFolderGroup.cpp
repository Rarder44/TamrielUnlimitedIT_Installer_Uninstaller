#pragma once
#include "stdafx.h"
#include "FileFolderGroup.h"


FileFolderGroup::FileFolderGroup()
{
}


FileFolderGroup::~FileFolderGroup()
{
}



void FileFolderGroup::Merge(FileFolderGroup ffg)
{
	for each (String s in ffg.File)
		AddFile(s);
	for each (String s in ffg.Dir)
		AddDir(s);
}
void FileFolderGroup::AddFile(String path)
{
	File.push_back(path);
}
void FileFolderGroup::AddDir(String path)
{
	Dir.push_back(path);
}