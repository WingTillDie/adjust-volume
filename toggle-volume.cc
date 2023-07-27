#if false
    x86_64-w64-mingw32-g++ $0 -lole32 && ./a.exe
    exit
#endif

#include <cstdio>
#include <Windows.h>
#include <MMDeviceAPI.h>
#include <EndpointVolume.h>

// Function to release interfaces
void release_interfaces(IMMDeviceEnumerator* pEnumerator, IMMDevice* pDevice, IAudioEndpointVolume* pEndpointVolume) {
    if (pEndpointVolume) pEndpointVolume->Release();
    if (pDevice) pDevice->Release();
    if (pEnumerator) pEnumerator->Release();
    CoUninitialize();
}

// Main function
int main()
{
    // Initialize the COM library
    HRESULT hr = CoInitialize(NULL);  
    if (FAILED(hr)) return hr;

    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioEndpointVolume* pEndpointVolume = NULL;

    // Create a device enumerator
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
    if (FAILED(hr)) {
        release_interfaces(pEnumerator, pDevice, pEndpointVolume);
        return hr;
    }

    // Get the default audio endpoint device
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr)) {
        release_interfaces(pEnumerator, pDevice, pEndpointVolume);
        return hr;
    }

    // Activate the IAudioEndpointVolume interface
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);
    if (FAILED(hr)) {
        release_interfaces(pEnumerator, pDevice, pEndpointVolume);
        return hr;
    }

    // Get the master volume level
    float currentVolume = 0;
    hr = pEndpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
    if (FAILED(hr)) {
        release_interfaces(pEnumerator, pDevice, pEndpointVolume);
        return hr;
    }

    // TODO assignment of read-only variable 'hr'
    auto set_volume = [hr, pEndpointVolume](float volume) mutable -> HRESULT  {
        hr = pEndpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
        return hr;
    };

    if (currentVolume != 0.f) {
        // Minimize-Volume
        hr = set_volume(0.f);
    } else {
        // Maximize-Volume
        hr = set_volume(1.f);
    }

    if (FAILED(hr)) {
        release_interfaces(pEnumerator, pDevice, pEndpointVolume);
        return hr;
    }

    // Release the interfaces
    release_interfaces(pEnumerator, pDevice, pEndpointVolume);
    return 0;
}