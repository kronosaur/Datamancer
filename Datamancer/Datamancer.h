//	Datamancer.h
//
//	Datamancer Server Implementation
//	Copyright (c) 2019 Kronosaur Productions, LLC. All Rights Reserved.

#pragma once

class CDatamancerEngine : public TSimpleEngine<CDatamancerEngine>
	{
	public:
		CDatamancerEngine (void);
		virtual ~CDatamancerEngine (void);

		static SMessageHandler m_MsgHandlerList[];
		static int m_iMsgHandlerListCount;

	protected:
		//	TSimpleEngine override
		virtual void OnBoot (void) override;
		virtual void OnMarkEx (void) override;
		virtual void OnStartRunning (void) override;
		virtual void OnStopRunning (void) override;

	private:
		//	Message handlers
		void MsgAeonOnStart (const SArchonMessage &Msg, const CHexeSecurityCtx *pSecurityCtx);
		void MsgStatus (const SArchonMessage &Msg, const CHexeSecurityCtx *pSecurityCtx);

		CCriticalSection m_cs;
		bool m_bAeonInitialized = false;
	};
