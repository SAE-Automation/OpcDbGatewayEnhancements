#include "stdafx.h"
#include "ChildStdoutReader.h"

ChildStdoutReader::ChildStdoutReader() :m_hChildStd_IN_Rd(NULL),
m_hChildStd_IN_Wr(NULL), m_hChildStd_OUT_Rd(NULL), m_hChildStd_OUT_Wr(NULL)

{
	// Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT. 
	if (!CreatePipe(&m_hChildStd_OUT_Rd, &m_hChildStd_OUT_Wr, &saAttr, 0))
		//ErrorExit(TEXT("StdoutRd CreatePipe"));
		ATLTRACE(_T("\n->StdoutRd CreatePipe.\n"));

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (!SetHandleInformation(m_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
		//ErrorExit(TEXT("Stdout SetHandleInformation"))
		ATLTRACE(_T("\n->Stdout SetHandleInformation\n"));
}

ChildStdoutReader::ChildStdoutReader(CProcessImageMemory *pProcessImageMemory):m_hChildStd_IN_Rd(NULL),
m_hChildStd_IN_Wr(NULL), m_hChildStd_OUT_Rd(NULL), m_hChildStd_OUT_Wr(NULL)
										
{
	_ASSERT(pProcessImageMemory != NULL);
	m_pProcessImageMemory = pProcessImageMemory;
	//read configuration file and define how to actualise MOs
	//in the configuration file will be also string to start mosquitto_sub

}


ChildStdoutReader::~ChildStdoutReader()
{
	//Terminate process and close process and thread handles
	TerminateChildProcess();
	//ToDo CloseHandle
	if (m_hChildStd_IN_Rd)
	{
		CloseHandle(m_hChildStd_IN_Rd);
		m_hChildStd_IN_Rd = NULL;
	}
	if (m_hChildStd_IN_Wr)
	{
		CloseHandle(m_hChildStd_IN_Wr);
		m_hChildStd_IN_Wr = NULL;
	}
	if (m_hChildStd_OUT_Rd)
	{
		CloseHandle(m_hChildStd_OUT_Rd);
		m_hChildStd_OUT_Rd = NULL;
	}
	if (m_hChildStd_OUT_Wr)
	{
		CloseHandle(m_hChildStd_OUT_Wr);
		m_hChildStd_OUT_Wr = NULL;
	}

	
}

BOOL ChildStdoutReader::CreateChildProcess(LPSTR szCmdline)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
	TCHAR szintCmdline[] = TEXT("mosquitto_sub.exe -h sae33 -t topic2");
	STARTUPINFO siStartInfo;
	BOOL bSuccess = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&m_piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdError = m_hChildStd_OUT_Wr;
	siStartInfo.hStdOutput = m_hChildStd_OUT_Wr;
	siStartInfo.hStdInput = m_hChildStd_IN_Rd;
	//siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	siStartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	siStartInfo.wShowWindow = SW_HIDE; // to provide that cmd window will not be displayied

	// Create the child process. 
	
	bSuccess = CreateProcess(NULL,
		szCmdline,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&m_piProcInfo);  // receives PROCESS_INFORMATION 

					   // If an error occurs, exit the application. 
	if (!bSuccess)
	{
		//ErrorExit(TEXT("CreateProcess"));
		ATLTRACE(_T("\n->CreateProcess\n"));
	}

	return bSuccess;
}

BOOL ChildStdoutReader::TerminateChildProcess()
{
	
	if (!m_piProcInfo.hProcess)
		return false;

	m_cCS.Enter();
	
	if (! TerminateProcess(m_piProcInfo.hProcess, 0) )
		return false;
	else
	{
		CloseHandle(m_piProcInfo.hProcess);
		m_piProcInfo.hProcess = NULL;

		if (m_piProcInfo.hThread)
		{
			CloseHandle(m_piProcInfo.hThread);
			m_piProcInfo.hThread = NULL;
		}

	}	
	m_cCS.Leave();
	return true;
}
