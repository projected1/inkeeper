#pragma once

namespace Utils
{
	// ------------------------------------------------------------------------
	inline void OpenUrl(const CString& _url)
	{
		BOOL rv = ::PathIsURL(_url);
		ATLENSURE_RETURN_VOID(rv);

		SHELLEXECUTEINFO sei = {0};
		sei.cbSize = sizeof(SHELLEXECUTEINFO);
		sei.lpVerb = _T("open");
		sei.lpFile = _url;

		rv = ::ShellExecuteEx(&sei);
		ATLASSERT(rv);
	}

	// ------------------------------------------------------------------------
	inline bool StartProcess(const CString& _imagePath, const CString& _args)
	{
		BOOL rv = ::PathFileExists(_imagePath);
		ATLENSURE_RETURN_VAL(rv, false);

		PROCESS_INFORMATION pi = {0};
		STARTUPINFO si = {0};
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;

		CString sCommandLine;
		sCommandLine.Format(_T("%s %s"), _imagePath, _args);

		rv = ::CreateProcess(
			NULL,						// Use command line
			sCommandLine.GetBuffer(),	// Command line
			NULL,						// Process handle not inheritable
			NULL,						// Thread handle not inheritable
			FALSE,						// Set handle inheritance to FALSE
			0,							// No creation flags
			NULL,						// Use parent's environment block
			NULL,						// Use parent's starting directory
			&si,						// Startup info
			&pi);						// Process info
		ATLENSURE_RETURN_VAL(rv, false);

		return true;
	}

	// ------------------------------------------------------------------------
	inline CString GetMachineGuid()
	{
		CRegKey rk;
		LONG rv = rk.Open(
			HKEY_LOCAL_MACHINE, 
			_T("Software\\Microsoft\\Cryptography"), 
			KEY_READ | KEY_WOW64_64KEY);
		ATLENSURE_RETURN_VAL(ERROR_SUCCESS == rv, _T(""));

		TCHAR szBuf[1024] = {0};
		DWORD dwBufSize = sizeof(szBuf); // Size in bytes
		rv = rk.QueryStringValue(_T("MachineGuid"), szBuf, &dwBufSize);
		ATLENSURE_RETURN_VAL(ERROR_SUCCESS == rv, _T(""));

		return szBuf;
	}

	// ------------------------------------------------------------------------
	inline CString LoadStringFromResource(UINT _resourceId)
	{
		CString s;
		BOOL rv = s.LoadString(_resourceId);
		ATLASSERT(rv);
		return s;
	}

	// ------------------------------------------------------------------------
	inline CStringA GetMd5Hash(const CStringA& _bytes)
	{
		ATLASSERT(!_bytes.IsEmpty());

		HCRYPTPROV hCryptProv = NULL;
		HCRYPTHASH hHash = NULL;

		// Get a handle to a cryptography provider context
		BOOL rv = ::CryptAcquireContext(
			&hCryptProv,
			NULL,
			NULL,
			PROV_RSA_AES,
			0);
		if (!rv)
		{
			// Check if key container exists and try creating one if it doesn't
			DWORD dwError = ::GetLastError();
			ATLENSURE_RETURN_VAL(NTE_BAD_KEYSET == dwError, "");

			rv = ::CryptAcquireContext(
				&hCryptProv,
				NULL,
				NULL,
				PROV_RSA_AES,
				CRYPT_NEWKEYSET);
			ATLENSURE_RETURN_VAL(rv, "");
		}

		// Acquire a hash object handle
		rv = ::CryptCreateHash(
			hCryptProv,
			CALG_MD5,
			0,
			0,
			&hHash);
		ATLENSURE_RETURN_VAL(rv, "");

		// Add data to the hash object
		rv = ::CryptHashData(
			hHash, 
			(const BYTE*)_bytes.GetString(), 
			_bytes.GetLength(), 
			0);
		ATLENSURE_RETURN_VAL(rv, "");

		// Get the hash size
		DWORD cbHashSize = 0;
		DWORD dwCount = sizeof(DWORD);
		rv = ::CryptGetHashParam(
			hHash, 
			HP_HASHSIZE, 
			(BYTE*)&cbHashSize,
			&dwCount, 
			0);
		ATLENSURE_RETURN_VAL(rv, "");

		// Get the hash value
		CStringA sHash;
		rv = ::CryptGetHashParam(
			hHash, 
			HP_HASHVAL, 
			(BYTE*)sHash.GetBufferSetLength(cbHashSize), 
			&cbHashSize, 
			0);
		ATLENSURE_RETURN_VAL(rv, "");

		// Cleanup
		::CryptDestroyHash(hHash);
		::CryptReleaseContext(hCryptProv, 0);

		return sHash;
	}

	// ------------------------------------------------------------------------
	inline CStringA BytesToHexString(const CStringA& _bytes)
	{
		ATLASSERT(!_bytes.IsEmpty());

		CStringA sHex;
		CHAR szBuf[3] = {0};
		BYTE* pszHash = (BYTE*)_bytes.GetString();
		for (int i = 0; i < _bytes.GetLength(); i++)
		{
			sprintf_s(szBuf, _countof(szBuf), "%02x", pszHash[i]);
			sHex += szBuf;
		}

		return sHex;
	}

	// ------------------------------------------------------------------------
	inline bool GetAppDir(CString& _path)
	{
		static enum { MAX_PATH_LEN = 4096 };
		CString sExeDir;
		DWORD dwLen = ::GetModuleFileName(
			NULL, 
			sExeDir.GetBufferSetLength(MAX_PATH_LEN),
			MAX_PATH_LEN);
		ATLENSURE_RETURN_VAL(dwLen, false);
		sExeDir.ReleaseBuffer();
		ATLPath::RemoveFileSpec(sExeDir.GetBuffer());
		_path = sExeDir;

		return true;
	}

	// ------------------------------------------------------------------------
	inline bool GetChromaticImagePath(CString& _path)
	{
		// Get program files x86 path
		static enum { MAX_PATH_LEN = 4096 };
		CString sLocalAppdataPath;
		BOOL rv = ::SHGetSpecialFolderPath(
			NULL, 
			sLocalAppdataPath.GetBufferSetLength(MAX_PATH_LEN), 
			CSIDL_LOCAL_APPDATA, 
			FALSE);
		ATLENSURE_RETURN_VAL(rv, false);
		sLocalAppdataPath.ReleaseBuffer();

		// Get Chrome EXE path
		CString sChromaticRelativePath(LoadStringFromResource(IDS_CHROMATIC_PATH));
		CString sChromaticAbsolutePath;
		sChromaticAbsolutePath.Format(_T("%s\\%s"), sLocalAppdataPath, sChromaticRelativePath);
		if (ATLPath::FileExists(sChromaticAbsolutePath))
		{
			_path = sChromaticAbsolutePath;
			return true;
		}

		return false;
	}
} // namespace Utils