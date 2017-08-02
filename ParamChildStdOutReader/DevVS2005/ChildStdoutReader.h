#pragma once

#include "ProcessImageMemory.h"

#define BUFSIZE 4096

class ChildStdoutReader
{
public:
	ChildStdoutReader(CProcessImageMemory *);
	ChildStdoutReader();
	~ChildStdoutReader();
	BOOL CreateChildProcess(LPSTR szCmdline);
	inline BOOL ReadFromPipe(){ return ReadFile(m_hChildStd_OUT_Rd, &chBuf, BUFSIZE, &dwRead, NULL); };
	BOOL TerminateChildProcess();
	CHAR chBuf[BUFSIZE];
	DWORD dwRead;
	CCS	m_cCS;
	//ParseJSON
	//Actualise MOs

protected:
	// Pamatove operandy
	CProcessImageMemory* m_pProcessImageMemory;

private:
	HANDLE m_hChildStd_IN_Rd;
	HANDLE m_hChildStd_IN_Wr;
	HANDLE m_hChildStd_OUT_Rd;
	HANDLE m_hChildStd_OUT_Wr;
	PROCESS_INFORMATION m_piProcInfo;
	CProcessValue m_pvDataFromChildStdout;
	SECURITY_ATTRIBUTES saAttr;

};

