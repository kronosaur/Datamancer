//	CDatamancerEngine.cpp
//
//	CDatamancerEngine class
//	Copyright (c) 2019 Kronosaur Productions, LLC. All Rights Reserved.

#include "pch.h"

DECLARE_CONST_STRING(PORT_AEON_NOTIFY,					"Aeon.notify")
DECLARE_CONST_STRING(PORT_DATAMANCER_COMMAND,			"Datamancer.command")
DECLARE_CONST_STRING(ADDRESS_DATAMANCER_COMMAND,		"Datamancer.command@~/~")

DECLARE_CONST_STRING(ENGINE_NAME_DATAMANCER,			"Datamancer")

DECLARE_CONST_STRING(MSG_ERROR_UNABLE_TO_COMPLY,		"Error.unableToComply")
DECLARE_CONST_STRING(MSG_OK,							"OK")
DECLARE_CONST_STRING(MSG_LOG_DEBUG,						"Log.debug")
DECLARE_CONST_STRING(MSG_LOG_ERROR,						"Log.error")
DECLARE_CONST_STRING(MSG_REPLY_DATA,					"Reply.data")

DECLARE_CONST_STRING(RIGHT_ARC_ADMIN,					"Arc.admin")

//	Message table --------------------------------------------------------------

DECLARE_CONST_STRING(MSG_AEON_ON_START,					"Aeon.onStart")
DECLARE_CONST_STRING(MSG_ARC_HOUSEKEEPING,				"Arc.housekeeping")

DECLARE_CONST_STRING(MSG_DATAMANCER_STATUS,				"Datamancer.status")

CDatamancerEngine::SMessageHandler CDatamancerEngine::m_MsgHandlerList[] =
	{
		//	Aeon.onStart
		{	MSG_AEON_ON_START,					&CDatamancerEngine::MsgAeonOnStart },

		//	Arc.housekeeping
		{	MSG_ARC_HOUSEKEEPING,				&TSimpleEngine::MsgNull },

		//	Datamancer.status
		{	MSG_DATAMANCER_STATUS,				&CDatamancerEngine::MsgStatus },
	};

int CDatamancerEngine::m_iMsgHandlerListCount = SIZEOF_STATIC_ARRAY(CDatamancerEngine::m_MsgHandlerList);

CDatamancerEngine::CDatamancerEngine (void) : TSimpleEngine(ENGINE_NAME_DATAMANCER, 3)

//	CDatamancerEngine constructor

	{
	}

CDatamancerEngine::~CDatamancerEngine (void)

//	CDatamancerEngine destructor

	{
	}

void CDatamancerEngine::MsgStatus (const SArchonMessage &Msg, const CHexeSecurityCtx *pSecurityCtx)

//	MsgStatus
//
//	Luminous.status

	{
	SendMessageReply(MSG_REPLY_DATA, CString("Status OK"), Msg);
	}

void CDatamancerEngine::OnBoot (void)

//	OnBoot
//
//	Boot the engine

	{
	//	Register our ports

	AddPort(ADDRESS_DATAMANCER_COMMAND);
	AddVirtualPort(PORT_DATAMANCER_COMMAND, ADDRESS_DATAMANCER_COMMAND, FLAG_PORT_NEAREST);

	//	Subscribe to Aeon notifications

	AddVirtualPort(PORT_AEON_NOTIFY, ADDRESS_DATAMANCER_COMMAND, FLAG_PORT_ALWAYS);
	}

void CDatamancerEngine::OnMarkEx (void)

//	OnMarkEx
//
//	Mark datums for garbage collection (we can also use this
//	opportunity to garbage collect our own stuff).

	{
	}

void CDatamancerEngine::OnStartRunning (void)

//	OnStartRunning
//
//	Start running

	{
	}

void CDatamancerEngine::OnStopRunning (void)

//	OnStopRunning
//
//	Stop running

	{
	}
