// EndPointController.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <wchar.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include "windows.h"
#include "Mmdeviceapi.h"
#include "PolicyConfig.h"
#include "Propidl.h"
#include "Functiondiscoverykeys_devpkey.h"

HRESULT SetDefaultAudioPlaybackDevice(LPCWSTR devID)
{	
	IPolicyConfigVista *pPolicyConfig;
	ERole reserved = eConsole;

    HRESULT hr = CoCreateInstance(__uuidof(CPolicyConfigVistaClient), 
		NULL, CLSCTX_ALL, __uuidof(IPolicyConfigVista), (LPVOID *)&pPolicyConfig);
	if (SUCCEEDED(hr))
	{
		hr = pPolicyConfig->SetDefaultEndpoint(devID, reserved);
		pPolicyConfig->Release();
	}
	return hr;
}

// EndPointController.exe [NewDefaultDeviceID]
int _tmain(int argc, wchar_t* argv[])
{

	//get string entered as parameter
	std::wstring device_name;

	device_name = std::wstring(L"$$no parameter given$$$");

	if (argc == 2){
		device_name = argv[1];
	}else{
		std::cout << "Usage: EndPointController.exe \"<name of device to switch to>\"" << std::endl << std::endl;
		std::cout << "<name of device to switch to> can be a device as displayed by the EndPointController.exe if called with no parameter." << std::endl;
		std::cout << "It can also just be a substring of the name, but this string has to be unique in all devices connected to the computer" << std::endl;
		std::cout << "Non ASCII characters like the german umlauts can not be displayed correctly in the console. But the paramer string is allowed to contain them." << std::endl << std::endl;
		std::cout << "Example: EndPointController.exe \"<name of device to switch to>\"" << std::endl << std::endl;
	}

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IMMDeviceEnumerator *pEnum = NULL;
		// Create a multimedia device enumerator.
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,
			CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
		if (SUCCEEDED(hr))
		{
			IMMDeviceCollection *pDevices;
			// Enumerate the output devices.
			hr = pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
			if (SUCCEEDED(hr))
			{
				UINT count;
				pDevices->GetCount(&count);
				if (SUCCEEDED(hr))
				{
					for (int i = 0; i < count; i++)
					{
						IMMDevice *pDevice;
						hr = pDevices->Item(i, &pDevice);
						if (SUCCEEDED(hr))
						{
							LPWSTR wstrID = NULL;
							hr = pDevice->GetId(&wstrID);
							if (SUCCEEDED(hr))
							{
								IPropertyStore *pStore;
								hr = pDevice->OpenPropertyStore(STGM_READ, &pStore);
								if (SUCCEEDED(hr))
								{
									PROPVARIANT friendlyName;
									PropVariantInit(&friendlyName);
									hr = pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);
									if (SUCCEEDED(hr))
									{

										std::wstring *originalName = new std::wstring(L"$$no parameter given$$$");
											
										originalName = new std::wstring(friendlyName.pwszVal);

										/*
										printf("Requested device name %ws\n",device_name);
										printf("Compared device name %s\n",*originalName);
										*/

										//printf("Audio Device %d: %ws\n",i, friendlyName.pwszVal);

										std::wcout << "Audio Device " << i << ": " << friendlyName.pwszVal << std::endl;

										
										
										if(originalName->find(device_name) != std::string::npos){
											SetDefaultAudioPlaybackDevice(wstrID);

											printf("Switched to ID: %d\n",i);

										}else{

											
										}

										PropVariantClear(&friendlyName);
									}
									pStore->Release();
								}
							}
							pDevice->Release();
						}
					}
				}
				pDevices->Release();
			}
			pEnum->Release();
		}
	}

	/**
	std::ofstream myfile;
	myfile.open ("output.xml");
	myfile << "Writing this to a file.\n";
	myfile.close();
	**/

	return hr;
}