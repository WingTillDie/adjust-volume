#if false
    x86_64-w64-mingw32-g++ $0 -lole32 && ./a.exe
    exit
#endif

#include <cstdio>
#include <Windows.h>
#include <MMDeviceAPI.h>
#include <EndpointVolume.h>

int main()
{
    // Initialize the COM library
    HRESULT hr = CoInitialize(NULL);  
    if (FAILED(hr))
    {
        return hr;
    }

    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pEndpointVolume = NULL;

    // Create a device enumerator
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr))
    {
        CoUninitialize();  // Uninitialize the COM library
        return hr;
    }

    // Get the default audio endpoint device
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr))
    {
        pEnumerator->Release();
        CoUninitialize();  // Uninitialize the COM library
        return hr;
    }

    // Activate the IAudioEndpointVolume interface
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);
    if (FAILED(hr))
    {
        pDevice->Release();
        pEnumerator->Release();
        CoUninitialize();  // Uninitialize the COM library
        return hr;
    }

    // Get the master volume level
    float currentVolume = 0;
    hr = pEndpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
    if (FAILED(hr))
    {
        pEndpointVolume->Release();
        pDevice->Release();
        pEnumerator->Release();
        CoUninitialize();  // Uninitialize the COM library
        return hr;
    }

    auto set_volume = [&](float volume){
        hr = pEndpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
    };

    if (currentVolume != 0.f) {
        // Minimize-Volume
        set_volume(0.f);
    } else {
        // Maximize-Volume
        set_volume(1.f);
    }

    // Release the interfaces
    pEndpointVolume->Release();
    pDevice->Release();
    pEnumerator->Release();

    CoUninitialize();  // Uninitialize the COM library

    return 0;
}