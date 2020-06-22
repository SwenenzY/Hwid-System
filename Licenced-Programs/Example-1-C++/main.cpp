#pragma region Header

#pragma comment(lib, "urlmon.lib")
#define _WIN32_DCOM

#include <iostream> // STD
#include <urlmon.h> // URL
#include <sstream> // read
#include <comdef.h>
#include <Wbemidl.h>

#include "packages/nlohmann.json.3.7.3/build/native/include/nlohmann/json.hpp"

#pragma comment(lib, "wbemuuid.lib")
#define CRED_MAX_USERNAME_LENGTH            513
#define CRED_MAX_CREDENTIAL_BLOB_SIZE       512
#define CREDUI_MAX_USERNAME_LENGTH CRED_MAX_USERNAME_LENGTH
#define CREDUI_MAX_PASSWORD_LENGTH (CRED_MAX_CREDENTIAL_BLOB_SIZE / 2)
#pragma argsused

using namespace nlohmann;
using namespace std;


#pragma endregion

#pragma region Reference

/* Reference
 * Github;
 * https://github.com/nlohmann/json -> JSON Parser/Reader
 * https://github.com/SwenenzY/JsonExample -> Basicly Using JSON
 * https://github.com/SwenenzY/SimpleHttpRequest -> Request Example
 * https://github.com/SwenenzY/WMIPlus -> For CPU ID
 */

#pragma endregion

#pragma region Hooks & Voids

std::string GetLicense() {
    IStream* stream;
    HRESULT result = URLOpenBlockingStream(0, L"http://127.0.0.1/list", &stream, 0, 0); // Any Url
    if (result != 0)
    {
        return NULL;
    }
    char buffer[100];
    unsigned long bytesRead;
    std::stringstream ss;
    stream->Read(buffer, 100, &bytesRead);
    while (bytesRead > 0U)
    {
        ss.write(buffer, (long long)bytesRead);
        stream->Read(buffer, 100, &bytesRead);
    }
    std::string resultString = ss.str();
 //   std::cout << "Data : \n" << resultString << std::endl; //  {"Keys":["key1","key2"]}
    return resultString;
}

VARIANT GetCpu()
{
	VARIANT CPU;
	wchar_t pszName[CREDUI_MAX_USERNAME_LENGTH + 1] = L"user";
	wchar_t pszPwd[CREDUI_MAX_PASSWORD_LENGTH + 1] = L"password";
	BSTR strNetworkResource;
	bool localconn = true;
	strNetworkResource = localconn ? (bstr_t)L"\\\\.\\root\\CIMV2" : (bstr_t)L"\\\\remote--machine\\root\\CIMV2";
	COAUTHIDENTITY* userAcct = NULL;
	COAUTHIDENTITY authIdent;
	HRESULT hres;
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);

	if (localconn)
		hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	else
		hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_NONE, NULL);
	IWbemLocator* pLoc = NULL;
	hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
	IWbemServices* pSvc = NULL;

	if (localconn)
		hres = pLoc->ConnectServer(_bstr_t(strNetworkResource), NULL, NULL, 0, NULL, 0, 0, &pSvc);
	else
		hres = pLoc->ConnectServer(_bstr_t(strNetworkResource), _bstr_t(pszName), _bstr_t(pszPwd), NULL, NULL, NULL, NULL, &pSvc);

	cout << "Connected to root\\CIMV2 WMI namespace" << endl;

	if (localconn)
		hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	else
	{
		memset(&authIdent, 0, sizeof(COAUTHIDENTITY));
		authIdent.PasswordLength = wcslen(pszPwd);
		authIdent.Password = (USHORT*)pszPwd;
		authIdent.User = (USHORT*)pszName;
		authIdent.UserLength = wcslen(pszName);
		authIdent.Domain = 0;
		authIdent.DomainLength = 0;
		authIdent.Flags = SEC_WINNT_AUTH_IDENTITY_UNICODE;
		userAcct = &authIdent;
		hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_DEFAULT, RPC_C_AUTHZ_DEFAULT, COLE_DEFAULT_PRINCIPAL, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE, userAcct, EOAC_NONE);
	}

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery((bstr_t)L"WQL", (bstr_t)L"SELECT * FROM Win32_Processor",
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

	if (!localconn)
	{
		hres = CoSetProxyBlanket(pEnumerator, RPC_C_AUTHN_DEFAULT, RPC_C_AUTHZ_DEFAULT, COLE_DEFAULT_PRINCIPAL, RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE, userAcct, EOAC_NONE);
	}
	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if (0 == uReturn || FAILED(hr))
			break;
		VARIANT vtProp;
		hr = pclsObj->Get(L"ProcessorId", 0, &CPU, 0, 0);
		return CPU;
		pclsObj->Release();
		pclsObj = NULL;
	}
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	if (pclsObj != NULL)
		pclsObj->Release();
	CoUninitialize();
	cin.get();

}

string bstr_to_str(BSTR source) {
	_bstr_t wrapped_bstr = _bstr_t(source);
	int length = wrapped_bstr.length();
	char* char_array = new char[length];
	strcpy_s(char_array, length + 1, wrapped_bstr);
	return char_array;
}

#pragma endregion

int main() {

	VARIANT CpuID = GetCpu();
	string CpuIDStr = bstr_to_str(CpuID.bstrVal);
	std::cout << "CPU ID : " << CpuIDStr << endl; 
    std::string LicenseString = GetLicense();
	
    auto Json = json::parse(LicenseString);
    bool Status = false;

    for (const auto& element : Json["Keys"])
    {
        if (element == CpuIDStr) {
            Status = true;
            break;
        }
    }

    if (Status) {
        std::cout << "Hi" << std::endl;
    }

	system("pause");
}

