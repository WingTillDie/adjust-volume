
#if false
    x86_64-w64-mingw32-g++ -Wall "$0" -lole32 -static && ./a.exe "$@"
    exit
#endif
// Above code makes it run as script

#include <cstdio>
#include <Windows.h>
#include <MMDeviceAPI.h>
#include <EndpointVolume.h>

int main(int argc, char* argv[])
{
    if (argc > 2) return 1;

    // Initialize the COM library
    CoInitialize(NULL);  

    IMMDeviceEnumerator* pEnumerator;
    IMMDevice* pDevice;
    IAudioEndpointVolume* pEndpointVolume;

    // Create a device enumerator
    CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);

    // Get the default audio endpoint device
    pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

    // Activate the IAudioEndpointVolume interface
    pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pEndpointVolume);

    if ( argc == 1 ) {
        // Get the master volume level
        float currentVolume;
        pEndpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
        printf("Current sound volume: %.1f\n\n", currentVolume);
    }

    if ( argc == 2 ) {
        float volume = strtof(argv[1], NULL);
        // Verify valid input range 0~1
        if ( ! ( 0.f <= volume && volume <= 1.f ) )
            return 1;
        pEndpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
    }

    CoUninitialize();

    return 0;
}
