
#if false
    x86_64-w64-mingw32-g++ $0 -lole32 -static && ./a.exe
    exit
#endif
// Above code makes it run as script

#include <cstdio>
#include <iostream>
#include <Windows.h>
#include <MMDeviceAPI.h>
#include <EndpointVolume.h>
#include <wrl/client.h> // Include the WRL headers for ComPtr
#include <stdexcept>
using Microsoft::WRL::ComPtr; // Add a using statement for ComPtr
int main()
{
    // Initialize the COM library
    CoInitialize(NULL);  

    ComPtr<IMMDeviceEnumerator> pEnumerator;
    ComPtr<IMMDevice> pDevice;
    ComPtr<IAudioEndpointVolume> pEndpointVolume;

    // Create a device enumerator
    CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);

    // Get the default audio endpoint device
    pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

    // Activate the IAudioEndpointVolume interface
    pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);

    // Get the master volume level
    float currentVolume = 0;
    pEndpointVolume->GetMasterVolumeLevelScalar(&currentVolume);

    auto set_volume = [hr, pEndpointVolume](float volume) mutable -> HRESULT  {
        pEndpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
        return hr;
    };

    if (currentVolume != 0.f) {
        // Minimize-Volume
        set_volume(0.f);
    } else {
        // Maximize-Volume
        set_volume(1.f);
    }

    CoUninitialize();
    
    return 0;
}
