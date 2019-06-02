//	Datamancer Arcology Module
//
//	Datamancer Server Implementation
//	Copyright (c) 2019 Kronosaur Productions, LLC. All Rights Reserved.

#include "pch.h"

struct SOptions
	{
	CString sMachineName;
	bool bDebug;
	};

DECLARE_CONST_STRING(STR_DEBUG_SWITCH,					"/debug")
DECLARE_CONST_STRING(STR_MACHINE_NAME_SWITCH,			"/machine:")

void ParseCommandLine (int argc, char *argv[], SOptions *retOptions);

int main (int argc, char* argv[])
	{
	CArchonProcess Module;

	//	We expect to be started from Arcology.exe, so if we are run without
	//	parameters then we just show help and exit.

	if (argc == 1)
		{
		printf("Datamancer: This program is a module of Arcology.exe.\n");
		return 1;
		}

	//	See if we have the debug flag

	SOptions Options;
	ParseCommandLine(argc, argv, &Options);

	//	Boot the module

	SProcessDesc Config;
	Config.sMachineName = Options.sMachineName;
	Config.dwFlags = 0;
	Config.dwFlags |= (Options.bDebug ? PROCESS_FLAG_DEBUG : 0);

	//	Add the engine

	SEngineDesc *pDesc = Config.Engines.Insert();
	pDesc->pEngine = new CDatamancerEngine;

	//	Boot

	Module.Boot(Config);

	//	Run

	Module.Run();

	return 0;
	}

void ParseCommandLine (int argc, char *argv[], SOptions *retOptions)
	{
	int i;

	//	Init

	retOptions->bDebug = false;

	//	Look for switches

	for (i = 1; i < argc; i++)
		{
		CString sArg(argv[i]);

		if (strEquals(sArg, STR_DEBUG_SWITCH))
			retOptions->bDebug = true;
		else if (strStartsWith(sArg, STR_MACHINE_NAME_SWITCH))
			retOptions->sMachineName = strSubString(sArg, STR_MACHINE_NAME_SWITCH.GetLength());
		}
	}
