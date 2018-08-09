#include "FileEx.h"
#include <algorithm>
#include <string.h>

#if (!defined _WINDOWS_) && (!defined _WIN32)
int CFileEx::m_snDepth = -1;
bool CFileEx::m_sbFile = true;	//TRUE for file and FALSE for folder
std::vector<std::string> CFileEx::m_svecFile;
#else
#include <shlobj.h>
#endif

/*******************************************************************************
* �������ƣ�	
* ����������	��ȡ�ļ��ָ�����
* ���������	
* ���������	
* �� �� ֵ��	����windows����'\\'��linux����'/'��
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2008-03-01	�ܷ�	      ����
*******************************************************************************/
char CFileEx::Separator()
{
#if (defined _WINDOWS_) || (defined _WIN32)
	return '\\';
#else
	return '/';
#endif
}

/*******************************************************************************
* �������ƣ�	
* ����������	��ȡ��ǰĿ¼��
* ���������	
* ���������	
* �� �� ֵ��	��ǰĿ¼ȫ·����������ĩβ�ġ�\\����/����
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2008-01-05	�ܷ�	      ����
*******************************************************************************/
std::string CFileEx::GetCurDirectory()
{
	std::string strRet;
#if (defined _WINDOWS_) || (defined _WIN32)
	char buff[256];
	GetCurrentDirectoryA(256, buff);
	strRet = buff;
#else
	strRet = get_current_dir_name();
#endif
	return strRet;
}

/*******************************************************************************
* �������ƣ�	
* ����������	��ȡ��ִ�г�������Ŀ¼��
* ���������	
* ���������	
* �� �� ֵ��	���ؿ�ִ�г�������Ŀ¼������ֵ������ĩβ�ġ�\\����/����
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2008-01-05	�ܷ�	      ����
*******************************************************************************/
std::string CFileEx::GetExeDirectory()
{
	std::string strRet;
	char buff[256] = {0};
#if (defined _WINDOWS_) || (defined _WIN32)
	GetModuleFileNameA(NULL, buff, 256);
#else
	readlink("/proc/self/exe", buff, 256); 
#endif
	strRet = buff;
	strRet = strRet.substr(0, strRet.rfind(Separator()));
	return strRet;
}

/*******************************************************************************
* �������ƣ�	
* ����������	���õ�ǰ����Ŀ¼��
* ���������	lpszFolder	-- �����õĹ���Ŀ¼��
* ���������	
* �� �� ֵ��	ִ�гɹ�����TRUE��ִ��ʧ�ܷ���FALSE��
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2008-02-26	�ܷ�	      ����
*******************************************************************************/
bool CFileEx::SetCurDirectory(const char * lpszFolder)
{
#ifdef _WIN32
	return !!SetCurrentDirectoryA(lpszFolder);
#else
	return (chdir(lpszFolder) == 0);
#endif
}

/*******************************************************************************
* �������ƣ�	
* ����������	����ָ���Ķ༶�ļ�Ŀ¼��
* ���������	
* ���������	
* �� �� ֵ��	��windows�����У����Ŀ¼�����ɹ���Ŀ¼�Ѵ��ڷ���true�����򷵻�false��
*				linux���������Ƿ���true��
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2008-01-05	�ܷ�	      ����
*******************************************************************************/
bool CFileEx::CreateFolder(const char * lpszFolder)
{
	if (0 == lpszFolder)
	{
		return false;
	}
	std::string strFolder = lpszFolder;
	if (strFolder.empty())
	{
		return false;
	}
	if (Separator() != strFolder[0] && std::string::npos == strFolder.find(':'))
	{
		std::string strCurDir = GetCurDirectory();
		strCurDir += Separator();
		strFolder.insert(strFolder.begin(), strCurDir.begin(), strCurDir.end());
	}
#if (defined _WINDOWS_) || (defined _WIN32)
	int nRet = SHCreateDirectoryExA(
		NULL, 
		strFolder.c_str(),
		NULL
		);
	return ERROR_SUCCESS == nRet || ERROR_ALREADY_EXISTS == nRet;
#else
	std::string strCmd = "mkdir -p \"" + strFolder + std::string("\"");
	system(strCmd.c_str());
	return true;
#endif
}


/*******************************************************************************
* �������ƣ�	
* ����������	����ָ���Ķ༶�ļ�Ŀ¼��
* ���������	
* ���������	
* �� �� ֵ��	��windows�����У����Ŀ¼�����ɹ���Ŀ¼�Ѵ��ڷ���true�����򷵻�false��
*				linux���������Ƿ���true��
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2014-014-02	������	      ����
*******************************************************************************/
bool CFileEx::CreateFolderEx(const char * lpszFolder)
{
	if (0 == lpszFolder)
	{
		return false;
	}
	std::string strFolder = lpszFolder;
	if (strFolder.empty())
	{
		return false;
	}

#if (defined _WINDOWS_) || (defined _WIN32)
	int nRet = SHCreateDirectoryExA(
		NULL, 
		strFolder.c_str(),
		NULL
		);
	return ERROR_SUCCESS == nRet || ERROR_ALREADY_EXISTS == nRet;
#else
	std::string strCmd = "mkdir -p \"" + strFolder + std::string("\"");
	system(strCmd.c_str());
	return true;
#endif
}

/*******************************************************************************
* �������ƣ�	
* ����������	Ϊ����ָ�����ļ�������Ҫ���ļ�Ŀ¼��
* ���������	
* ���������	
* �� �� ֵ��	��windows�����У����Ŀ¼�����ɹ���Ŀ¼�Ѵ��ڷ���true�����򷵻�false��
*				linux���������Ƿ���true��
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2008-01-05	�ܷ�	      ����
*******************************************************************************/
bool CFileEx::CreateFolderForFile(const char * lpszFile)
{
	std::string strFolder = lpszFile;
	strFolder = strFolder.substr(0, strFolder.rfind(Separator()));
	return CreateFolder(strFolder.c_str());
}

/*******************************************************************************
* �������ƣ�	
* ����������	����ȫ·����ȡ�ļ�����
* ���������	
* ���������	
* �� �� ֵ��	���ػ�ȡ���ļ�����
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2008-01-05	�ܷ�	      ����
*******************************************************************************/
std::string CFileEx::Path2FileName(const char *lpszPath)
{
	std::string strRet = lpszPath;
	if (strRet.empty())
	{
		return strRet;
	}
	if (*strRet.rbegin() == Separator())
	{
		strRet.erase(strRet.length() - 1);
	}
	std::string::size_type pos = strRet.rfind(Separator());
	if (std::string::npos == pos)
	{
		return strRet;
	}
	return strRet.substr(pos + 1);
}

/*******************************************************************************
* �������ƣ�	
* ����������	��ȡ����ʣ��ռ䡣
* ���������	lpszPath -- ����Ŀ¼��
* ���������	
* �� �� ֵ��	���ش���ʣ��ռ�Ĵ�С����ȡʧ�ܷ���0����λMB��
* ����˵����	
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2007-11-17	�ܷ�	      ����
*******************************************************************************/
UINT CFileEx::GetFreeDiskSpace(const char* lpszPath)
{
#if (defined _WINDOWS_) || (defined _WIN32)
	//���Ӳ�̿ռ�
	ULARGE_INTEGER ulUserFree;
	ULARGE_INTEGER ulTotal;
	if(!GetDiskFreeSpaceExA(lpszPath, &ulUserFree, &ulTotal, NULL))
	{
		//���Ӳ��ʧ�ܣ�һ������Ϊ����·��������
		return 0;
	}
	else
	{
		UINT nMb = 1024 * 1024;
		UINT nUserFree = ulUserFree.HighPart * 4096 + ulUserFree.LowPart / nMb;
		UINT nTotal = ulTotal.HighPart * 4096 + ulTotal.LowPart / nMb;
		return nUserFree;
	}
	return 0;
#else
	struct statfs stVfs;
	memset( &stVfs, 0, sizeof( stVfs ) );
	if( -1 == statfs(lpszPath, &stVfs ) )
	{
		return 0;
	}
	return (UINT)(stVfs.f_bfree / 1024 / 1024 * stVfs.f_bsize);
#endif
}

/*******************************************************************************
* �������ƣ�	
* ����������	ɾ��ָ�����ļ��С�
* ���������	
* ���������	
* �� �� ֵ��	ִ�гɹ�����TRUE��ִ��ʧ�ܷ���FALSE��
* ����˵����	ѭ��ɾ���������е��ļ����ݡ�
* �޸�����		�޸���	      �޸�����
* ------------------------------------------------------------------------------
* 2008-01-05	�ܷ�	      ����
*******************************************************************************/
bool CFileEx::DelFolder(const char * lpszFolder)
{
#if (defined _WINDOWS_) || (defined _WIN32)
	std::string strPath = std::string(lpszFolder) + '\0';
	SHFILEOPSTRUCT fs;
	fs.hwnd = NULL;
	fs.wFunc = FO_DELETE;
	fs.pFrom = strPath.c_str();
	fs.pTo = NULL;
	fs.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
	fs.fAnyOperationsAborted = TRUE;
	return (0 == SHFileOperation(&fs)); 
#else
	std::string strCmd = std::string("rm -rfd \"") + lpszFolder + std::string("\"");
	system(strCmd.c_str());
	return true;
#endif
}

#if (!defined _WINDOWS_) && (!defined _WIN32)
int CFileEx::FileFunc(const char *file, const struct stat *sb, int flag)
{
	int nDepth = std::count(file, file + strlen(file), '/');
	if (-1 == m_snDepth)
	{
		m_snDepth = nDepth;
	}
	if (nDepth - m_snDepth == 1)
	{
		if ((m_sbFile && FTW_F == flag) || (!m_sbFile && FTW_D == flag))
		{
			m_svecFile.push_back(std::string(file));
		}
	}
	return 0;
}
#endif
