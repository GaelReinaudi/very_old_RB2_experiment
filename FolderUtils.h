
///////////////////////////////////////////////////////////////////////////////////////////////////
/////// Cette première partie de fichier viens de 
/////// Code-Project.com  : http://www.codeproject.com/file/textfile.asp
/////// il permet juste de gerer facilement l'ecriture et la lecture dans un fichier .txt
/////// qu'on utilise pour sauver les Settings.
///////  cf : Doc&Astuce : "The Code Project - CTextFile A handy helper - Files and Folders.htm"
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTFILE_H_
#define _TEXTFILE_H_

class CTextFile
{
public:

	// ctor( s )
	CTextFile( const CString& ext = _T( "" ), const CString& eol = _T( "\r\n" ) );

	// dtor
	virtual ~CTextFile();

	// File operations
	BOOL	ReadTextFile( CString& filename, CStringArray& contents );
	BOOL	ReadTextFile( CString& filename, CString& contents );

	BOOL	WriteTextFile( CString& filename, const CStringArray& contents );
	BOOL	WriteTextFile( CString& filename, const CString& contents );

	BOOL	AppendFile( CString& filename, const CString& contents );
	BOOL	AppendFile( CString& filename, const CStringArray& contents );

	// Window operations
	BOOL	Load( CString& filename, CEdit* edit );
	BOOL	Load( CString& filename, CListBox* list );
	BOOL	Save( CString& filename, CEdit* edit );
	BOOL	Save( CString& filename, CListBox* list );

	// Error handling
	CString GetErrorMessage();

protected:

	virtual BOOL GetFilename( BOOL save, CString& filename );
	CString GetExtension();

private:

	CString m_error;
	CString m_extension;
	CString m_eol;

	void	ClearError();
	BOOL	ValidParam( CWnd* wnd );

};

#endif // _TEXTFILE_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
///////  Cette deuxième partie de fichier vient de 
///////  Code Project : http://www.codeproject.com/file/FolderUtils.asp
///////  Il facilite la création et la manipulation de dossiers
///////  cf : Doc&Astuce : "The Code Project - Folder utilities - Files and Folders.htm"
///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _FOLDER_UTILS
#define _FOLDER_UTILS

#include <windows.h>
#include <stdio.h>

class CFolderUtils
{
public:
	static BOOL	CreateFolder(LPCTSTR pFolder)
	{
		TCHAR szFolder[MAX_PATH];
		_tcscpy(szFolder, pFolder);
		TCHAR *pStart = szFolder;
		TCHAR *pEnd = pStart + _tcslen(szFolder);
		TCHAR *p = pEnd;

		// Try 10 times to create the directory
		for (int i=0;i<10;i++)
		{
			BOOL bOK = CreateDirectory(szFolder, NULL);
			DWORD dwLastError = GetLastError();
			if (!bOK && dwLastError == ERROR_PATH_NOT_FOUND)
			{
				while (*p != '\\')
				{
					if (p == pStart)
						return FALSE;
					p--;
				}
				*p = NULL;
			}
			else if (bOK || (ERROR_ALREADY_EXISTS == dwLastError))
			{
				if (p == pEnd)
					return TRUE;

				*p = '\\';
				while (*p)
					p++;
			}
			else
				break;
		}

		return FALSE;
	}

	static DWORD GetSpace(LPCTSTR pFolder)
	{
		TCHAR szFullPathFileName[MAX_PATH];
		TCHAR szFilename[MAX_PATH];
		WIN32_FIND_DATA FileData = {0};
		BOOL bFinished = FALSE; 
		DWORD dwSize = 0;

		_stprintf(szFullPathFileName, TEXT("%s\\*.*"), pFolder);
		HANDLE hSearch = FindFirstFile(szFullPathFileName, &FileData); 
		if (hSearch == INVALID_HANDLE_VALUE) 
			return 0;

		while (!bFinished)
		{
			_stprintf(szFilename, TEXT("%s\\%s"),pFolder,FileData.cFileName);
			if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (_tcscmp(FileData.cFileName,TEXT(".")) && _tcscmp(FileData.cFileName,TEXT("..")))
					dwSize += GetSpace(szFilename);
			}
			else
				dwSize += FileData.nFileSizeLow;

			if (!FindNextFile(hSearch, &FileData)) 
			{
				if (GetLastError() == ERROR_NO_MORE_FILES) 
					bFinished = TRUE;
			} 
		}

		FindClose(hSearch);
		return dwSize;
	}

	static BOOL RemoveFolder(LPCTSTR pFolder)
	{
		TCHAR szFullPathFileName[MAX_PATH];
		TCHAR szFilename[MAX_PATH];

		if (!::RemoveDirectory(pFolder))
		{
			DWORD err = GetLastError();
			if (err != ERROR_DIR_NOT_EMPTY)
				return FALSE;
		}
		
		// remove sub folders and files.

		WIN32_FIND_DATA FileData = {0};
		BOOL bFinished = FALSE; 
		DWORD dwSize = 0;

		_stprintf(szFullPathFileName, TEXT("%s\\*.*"), pFolder);
		HANDLE hSearch = FindFirstFile(szFullPathFileName, &FileData); 
		if (hSearch == INVALID_HANDLE_VALUE) 
			return 0;

		while (!bFinished)
		{
			_stprintf(szFilename, TEXT("%s\\%s"),pFolder,FileData.cFileName);
			if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (_tcscmp(FileData.cFileName,TEXT(".")) && _tcscmp(FileData.cFileName,TEXT("..")))
				{
					RemoveFolder(szFilename);
					RemoveDirectory(szFilename);
				}
			}
			else
			{
				if (FileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					SetFileAttributes(szFilename, FILE_ATTRIBUTE_NORMAL);

				if (!::DeleteFile(szFilename))
				{
					FindClose(hSearch);
					return FALSE;
				}
			}
			if (!FindNextFile(hSearch, &FileData)) 
			{
				if (GetLastError() == ERROR_NO_MORE_FILES) 
					bFinished = TRUE;
			} 
		}
		FindClose(hSearch);

		// Here the directory is empty.
		::RemoveDirectory(pFolder);
		return TRUE;
	}

	static BOOL CopyFolder(LPCTSTR pFromFolder, LPCTSTR pToFolder)
	{
		SHFILEOPSTRUCT fo = {0};

		fo.wFunc = FO_COPY;
		fo.pFrom = pFromFolder;
		fo.pTo = pToFolder;
		fo.fFlags = FOF_SILENT | FOF_NOERRORUI;

		int rc = SHFileOperation(&fo);
		return (rc == 0);
	}
};
#endif