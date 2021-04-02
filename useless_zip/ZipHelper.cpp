#include "stdafx.h"
#include "ZipHelper.h"

ZipHelper::ZipHelper()
{
	m_nZipCnt = 0;
}

ZipHelper::~ZipHelper()
{
}

BOOL ZipHelper::Zip_PackFiles(vector<CString> arr, CString mZipFileFullPath)
{
	for (int i = 0; i < arr.size(); ++i)
	{
		//参数错误 
		if ((arr[i] == "") || (mZipFileFullPath == ""))
		{
			//路径异常返回 
			return FALSE;
		}

		if (!IsFolderExist(arr[i]) && !IsFileExist(arr[i]))
		{
			//要被压缩的文件夹不存在 
			return FALSE;
		}

		CString tZipFilePath = mZipFileFullPath.Left(mZipFileFullPath.ReverseFind('\\') + 1);
		if (!IsFolderExist(tZipFilePath))
		{
			//ZIP文件存放的文件夹不存在创建它 
			wchar_t* temp = (wchar_t*)tZipFilePath.GetBuffer(tZipFilePath.GetLength());
			if (FALSE == CreatedMultipleDirectory(temp))
			{
				//创建目录失败 
				return FALSE;
			}
		}

		//获得文件夹的名字 
		if (arr[i].Right(1) == '\\')
		{
			this->m_FolderPath = arr[i].Left(arr[i].GetLength() - 1);
			m_FolderName = m_FolderPath.Right(m_FolderPath.GetLength() - m_FolderPath.ReverseFind('\\') - 1);
		}
		else
		{
			this->m_FolderPath = arr[i];
			m_FolderName = arr[i].Right(arr[i].GetLength() - arr[i].ReverseFind('\\') - 1);
		}

		/************************************************************************/

		//创建ZIP文件 
		if (m_nZipCnt == 0)
		{
			m_hz = CreateZip(mZipFileFullPath, 0);
		}

		if (m_hz == 0)
		{
			//创建Zip文件失败 
			return FALSE;
		}

		//递归文件夹,将获取的文件加入ZIP文件 
		m_nZipCnt++;
		BrowseFile(arr[i]);
		//关闭ZIP文件 
		if (m_nZipCnt == m_arrFileFullName.size())
		{
			CloseZip(m_hz);
			m_nZipCnt = 0;
			break;
		}
	}

	/************************************************************************/

	CFileFind tFFind;
	if (!tFFind.FindFile(mZipFileFullPath))
	{
		//压缩失败(未发现压缩后的文件) 
		return FALSE;
	}

	return TRUE;
}

/
// 函数说明: 解压缩文件夹 
// 参数说明: [in]： mUnPackPath     解压后文件存放的路径 
//                         mZipFileFullPath  ZIP文件的路径
/
BOOL ZipHelper::Zip_UnPackFiles(CString mZipFileFullPath, CString mUnPackPath)
{
	//参数错误 
	if ((mUnPackPath == "") || (mZipFileFullPath == ""))
	{
		//路径异常返回 
		return FALSE;
	}

	CFileFind tFFind;
	if (!tFFind.FindFile(mZipFileFullPath))
	{
		//压缩失败(未发现压缩文件) 
		return FALSE;
	}

	//如果解压缩的路径不存在 试图创建它 
	CString tZipFilePath = mUnPackPath;
	if (!IsFolderExist(tZipFilePath))
	{
		//解压后存放的文件夹不存在 创建它 
		wchar_t* temp = (wchar_t*)tZipFilePath.GetBuffer(tZipFilePath.GetLength());
		if (FALSE == CreatedMultipleDirectory(temp))
		{
			//创建目录失败 
			return FALSE;
		}
	}
	/************************************************************************/
	//打开ZIP文件 
	m_hz = OpenZip(mZipFileFullPath, 0);
	if (m_hz == 0)
	{
		//打开Zip文件失败 
		return FALSE;
	}

	m_zr = SetUnzipBaseDir(m_hz, mUnPackPath);
	if (m_zr != ZR_OK)
	{
		//打开Zip文件失败 
		CloseZip(m_hz);
		return FALSE;
	}

	m_zr = GetZipItem(m_hz, -1, &m_ze);
	if (m_zr != ZR_OK)
	{
		//获取Zip文件内容失败 
		CloseZip(m_hz);
		return FALSE;
	}

	int numitems = m_ze.index;
	for (int i = 0; i < numitems; i++)
	{
		m_zr = GetZipItem(m_hz, i, &m_ze);
		m_zr = UnzipItem(m_hz, i, m_ze.name);

		if (i == 100)
		{
			int x = 1;
		}
		if (m_zr != ZR_OK)
		{
			//获取Zip文件内容失败 
			CloseZip(m_hz);
			return FALSE;
		}
	}

	m_nZipCnt = 0;
	CloseZip(m_hz);
	return TRUE;
}

/
// 函数说明: 遍历文件夹 
// 参数说明: [in]：strFile 遍历的文件夹(此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
/
void ZipHelper::BrowseFile(CString & strFile)
{
	CFileFind ff;
	CString szDir = strFile;

	WIN32_FIND_DATAA FindFileData;
	FindFirstFile(strFile, &FindFileData);
	if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//Folder
	{
		if (szDir.Right(1) != "\\")
			szDir += "\\";
		szDir += "*.*";
	}
	else//File
	{
		int xx = 21;
	}

	BOOL res = ff.FindFile(szDir);
	while (res)
	{
		res = ff.FindNextFile();
		if (ff.IsDirectory() && !ff.IsDots())
		{
			//如果是一个子目录，用递归继续往深一层找 
			CString strPath = ff.GetFilePath();

			CString subPath;
			GetRelativePath(strPath, subPath);
			//将文件添加到ZIP文件 
			ZipAddFolder(m_hz, subPath);
			BrowseFile(strPath);
		}
		else if (!ff.IsDirectory() && !ff.IsDots())
		{
			//显示当前访问的文件(完整路径) 
			CString strPath = ff.GetFilePath();
			CString subPath;

			GetRelativePath(strPath, subPath);
			//将文件添加到ZIP文件 
			ZipAdd(m_hz, subPath, strPath);
		}
	}
	//关闭 
	ff.Close();
}

/
// 函数说明: 获取相对路径 
// 参数说明: [in]：pFullPath 当前文件的完整路径 [out] : 解析后的相对路径 
/
void ZipHelper::GetRelativePath(CString & pFullPath, CString & pSubString)
{
	pSubString = pFullPath.Right(pFullPath.GetLength() - GetAppPath().GetLength());
}

/
// 函数说明: 创建多级目录 
// 参数说明: [in]： 路径字符串 
// 返回值: BOOL 成功True 失败False 
/
BOOL ZipHelper::CreatedMultipleDirectory(wchar_t* direct)
{
	std::wstring Directoryname = direct;

	if (Directoryname[Directoryname.length() - 1] != '\\')
	{
		Directoryname.append(1, '\\');
	}
	std::vector< std::wstring> vpath;
	std::wstring strtemp;
	BOOL  bSuccess = FALSE;
	for (int i = 0; i < Directoryname.length(); i++)
	{
		if (Directoryname[i] != '\\')
		{
			strtemp.append(1, Directoryname[i]);
		}
		else
		{
			vpath.push_back(strtemp);
			strtemp.append(1, '\\');
		}
	}
	vector< std::wstring>::const_iterator vIter = vpath.begin();
	for (vIter; vIter != vpath.end(); vIter++)
	{
		bSuccess = CreateDirectory((LPCTSTR)vIter->c_str(), NULL) ? TRUE : FALSE;
	}

	return bSuccess;
}

/
// 函数说明: 检查指定的文件夹是否存在 
// 参数说明: [in]：strPath 检查的文件夹 (此方法会主动向路径末尾添加*.*) 
// 返回值:BOOL类型,存在返回TRUE,否则为FALSE 
/
BOOL ZipHelper::IsFolderExist(CString & strPath)
{
	CString sCheckPath = strPath;

	if (sCheckPath.Right(1) != '\\')
		sCheckPath += '\\';

	sCheckPath += "*.*";

	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(sCheckPath, &wfd);

	if ((hFind != INVALID_HANDLE_VALUE) &&
		(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE))
	{
		//如果存在并类型是文件夹 
		rValue = TRUE;
	}

	FindClose(hFind);
	return rValue;
}
BOOL ZipHelper::IsFileExist(CString& strPath)
{
	DWORD dwAttrib = GetFileAttributes(strPath);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}
void ZipHelper::PressFolder(CString strTgt, CString strSrcFolder, vector<CString> arrSrcFloder)
{
	m_arrFileFullName.clear();

	if (!strSrcFolder.IsEmpty())
	{
		m_arrFileFullName.push_back(strSrcFolder);
	}
	if (!arrSrcFloder.empty())
	{
		for (auto it : arrSrcFloder)
		{
			m_arrFileFullName.push_back(it);
		}
	}

	//目的文件夹
	string patm_hzipFile = strTgt;
	Zip_PackFiles(m_arrFileFullName, patm_hzipFile.c_str());
}


void ZipHelper::UnPressFolder(CString strTgt)
{
	if (strTgt.IsEmpty())
		return;

	string patm_hzipFile = strTgt;
	string mUnPackPath = GetAppPath();

	Zip_UnPackFiles(patm_hzipFile.c_str(), mUnPackPath.c_str());
}