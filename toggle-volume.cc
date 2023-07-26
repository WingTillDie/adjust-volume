
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

//
using Microsoft::WRL::ComPtr; // Add a using statement for ComPtr

// Main function
int main()
{
    try {
        // Initialize the COM library
        HRESULT hr = CoInitialize(NULL);  
        if (FAILED(hr))
            throw std::runtime_error("Failed to initialize the COM library.");

        ComPtr<IMMDeviceEnumerator> pEnumerator;
        ComPtr<IMMDevice> pDevice;
        ComPtr<IAudioEndpointVolume> pEndpointVolume;

        // Create a device enumerator
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
        if (FAILED(hr))
            throw std::runtime_error("Failed to create a device enumerator.");

        // Get the default audio endpoint device
        hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
        if (FAILED(hr))
            throw std::runtime_error("Failed to get the default audio endpoint device.");

        // Activate the IAudioEndpointVolume interface
        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);
        if (FAILED(hr))
            throw std::runtime_error("Failed to activate the IAudioEndpointVolume interface.");

        // Get the master volume level
        float currentVolume = 0;
        hr = pEndpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
        if (FAILED(hr))
            throw std::runtime_error("Failed to get the master volume level.");

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

        if (FAILED(hr))
            throw std::runtime_error("Failed to set the master volume level.");
        
        return 0;
    } catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl;
        CoUninitialize();
        return 1;
    }
}
