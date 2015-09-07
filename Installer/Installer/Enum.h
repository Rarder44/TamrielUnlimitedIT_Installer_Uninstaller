#pragma once


enum VersionDotNet
{
	vNotFound,
	v1_1,
	v2_0,
	v3_0,
	v3_5,
	v4_0,
	v4_5,
	v4_5_1,
	v4_5_2,
	v4_6,
	v4_6Later
};

enum Command 
{ 
	CreateReg,
	DeleteReg,
	GetReg,
	Copy,
	Delete,
	CheckVersion,
	SetVersion,
	ExecuteCommand,
	ExecuteCommandSync,
	GetCurrentRegVersion,
};

enum TypeRun
{
	Sync,
	Async
};

