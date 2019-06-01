//	CDatamancerEngine.cpp
//
//	CDatamancerEngine class
//	Copyright (c) 2019 Kronosaur Productions, LLC. All Rights Reserved.

#include "pch.h"

DECLARE_CONST_STRING(ADDR_AEON_COMMAND,					"Aeon.command")
DECLARE_CONST_STRING(ADDR_DATAMANCER_COMMAND,			"Datamancer.command")

DECLARE_CONST_STRING(MSG_AEON_CREATE_TABLE,				"Aeon.createTable")
DECLARE_CONST_STRING(MSG_ERROR_ALREADY_EXISTS,			"Error.alreadyExists")
DECLARE_CONST_STRING(MSG_LOG_ERROR,						"Log.error")

DECLARE_CONST_STRING(TYPE_ARC_TABLE,					"Arc.table")

DECLARE_CONST_STRING(ERR_UNABLE_TO_CREATE_FILES_TABLE,	"Unable to create Datamancer.files table: %s")
DECLARE_CONST_STRING(ERR_EXPECTED_ARC_TABLE,			"Unable to interpret table definition.")

DECLARE_CONST_STRING(FILES_TABLE_NAME,					"Datamancer.files")
DECLARE_CONST_STRING(FILES_TABLE_DESC,					"Arc.table Datamancer.files { type:files }")

static constexpr DWORD MESSAGE_TIMEOUT =				30 * 1000;

class CBootSession : public ISessionHandler
	{
	public:
		CBootSession (CDatamancerEngine &Engine) : 
				m_Engine(Engine)
			{ }

	protected:
		//	ISessionHandler virtuals
		virtual bool OnProcessMessage (const SArchonMessage &Msg) override;
		virtual bool OnStartSession (const SArchonMessage &Msg, DWORD dwTicket) override;

	private:
		enum States
			{
			stateUnknown,
			stateCreatingCacheTable,
			};

		CDatamancerEngine &m_Engine;
		States m_iState = stateUnknown;
	};

void CDatamancerEngine::MsgAeonOnStart (const SArchonMessage &Msg, const CHexeSecurityCtx *pSecurityCtx)

//	MsgAeonOnStart
//
//	Aeon.onStart

	{
	CSmartLock Lock(m_cs);

	//	If Aeon is already running, then skip this. This can happen if Aeon
	//	restarts.

	if (m_bAeonInitialized)
		return;

	//	Start a session to create required tables

	//StartSession(Msg, new CBootSession(*this));
	}

//	CBootSession ---------------------------------------------------------------

bool CBootSession::OnProcessMessage (const SArchonMessage &Msg)

//	OnProcessMessage
//
//	Handle a response

	{
	switch (m_iState)
		{
		case stateCreatingCacheTable:
			{
			//	If we got an error creating the table, then something is
			//	seriously wrong with the arcology. [So we log it and wait for
			//	someone to fix it.]

			if (IsError(Msg) && !strEquals(Msg.sMsg, MSG_ERROR_ALREADY_EXISTS))
				{
				GetProcessCtx()->Log(MSG_LOG_ERROR, strPattern(ERR_UNABLE_TO_CREATE_FILES_TABLE, Msg.dPayload.AsString()));
				return false;
				}

			//	We're done initializing

			return false;
			}

		default:
			return false;
		}
	}

bool CBootSession::OnStartSession (const SArchonMessage &Msg, DWORD dwTicket)

//	OnStartSession
//
//	Start session

	{
	CString sError;
	m_iState = stateCreatingCacheTable;

	//	Create the /Arc.users table
	//	NOTE: We always create the table and leave it to Aeon to send back an "already exists"
	//	message.

	CHexeProcess Process;
	if (!Process.LoadStandardLibraries(&sError))
		{
		GetProcessCtx()->Log(MSG_LOG_ERROR, sError);
		return false;
		}

	//	Parse the table definition into a document

	CHexeDocument TableDoc;
	CStringBuffer Stream(FILES_TABLE_DESC);
	if (!TableDoc.InitFromStream(Stream, Process, &sError))
		{
		GetProcessCtx()->Log(MSG_LOG_ERROR, sError);
		return false;
		}

	//	Get the table description (we expect only 1)

	int iTableIndex = TableDoc.GetTypeIndex(TYPE_ARC_TABLE);
	if (TableDoc.GetTypeIndexCount(iTableIndex) != 1)
		{
		GetProcessCtx()->Log(MSG_LOG_ERROR, ERR_EXPECTED_ARC_TABLE);
		return false;
		}

	const CString &sTableName = TableDoc.GetTypeIndexName(iTableIndex, 0);
	CDatum dTableDesc = TableDoc.GetTypeIndexData(iTableIndex, 0);

	//	Create the command

	CComplexArray *pPayload = new CComplexArray;
	pPayload->Insert(dTableDesc);

	SendMessageCommand(ADDR_AEON_COMMAND, MSG_AEON_CREATE_TABLE, GenerateAddress(ADDR_DATAMANCER_COMMAND), CDatum(pPayload), MESSAGE_TIMEOUT);

	return true;
	}
