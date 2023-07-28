#if false
    x86_64-w64-mingw32-g++ -Wall "$0" -lole32 -static && ./a.exe "$@"
    exit
#endif
// Above code makes it run as script

#include <cstdio>
#include <cstring>
#include <iostream>
#include <Windows.h>
#include <MMDeviceAPI.h>
#include <EndpointVolume.h>
#include <wrl/client.h> // Include the WRL headers for ComPtr
#include <stdexcept>

// Smart Pointer
using Microsoft::WRL::ComPtr; // Add a using statement for ComPtr

bool streq(const char* p_chr_0, const char* p_chr_1) {
    return strcmp(p_chr_0, p_chr_1) == 0;
}

void usage() {
    std::string usage =
R"(Usage: adjust-volume.exe [volume]
If no volume is specified, it will print the current sound volume.
If a volume value between 0 and 1 is provided, it will set the sound volume.
)";
    printf("%s", usage.c_str());
}

int main(int argc, char* argv[])
{
    enum class Program_mode {
        unknown,
        set_sound_volume,
        get_sound_volume
    };

    Program_mode program_mode = Program_mode::unknown;
    
    float volume; // For Program_mode::set_sound_volume

    if (argc == 1) {
        program_mode = Program_mode::get_sound_volume;
        // TODO segfault
    } else if (argc == 2) {
        if (
            streq(argv[1], "--help") ||
            streq(argv[1], "-h")
            ) {
            usage();
            return 0;
        } else {
            program_mode = Program_mode::set_sound_volume;
            volume = strtof(argv[1], NULL);

            // Verify valid input range 0~1
            if ( ! ( 0.f <= volume && volume <= 1.f ) ) {
                printf("Invalid volume value. Volume must be between 0 and 1.\n\n");
                usage();
                return 1;
            }
        }
    } else {
        puts("Wrong command line arguments");
        usage();
        return 1;
    }

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

    if ( program_mode == Program_mode::get_sound_volume ) {
        // Get the master volume level
        float currentVolume;
        pEndpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
        printf("Current sound volume: %.1f\n\n", currentVolume);
        usage();
    }

    if ( program_mode == Program_mode::set_sound_volume )
        pEndpointVolume->SetMasterVolumeLevelScalar(volume, NULL);

    CoUninitialize();

    return 0;
}
