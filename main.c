#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define ERROR_SUCCESS 0
#define ERROR_ARGS 1
#define ERROR_SDL 2

struct audioData_t
{
    uint8_t *ptr;
    int length;
};
void AudioCallback(void *, Uint8 *, int);

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Enter file path!\n");
        return ERROR_ARGS;
    }
    const char *filename = argv[1];
    //init SDL audio
    if (SDL_Init(SDL_INIT_AUDIO))
    {
        fprintf(stderr, "Unable to initialize SDL: [%s]\n", SDL_GetError());
        return ERROR_SDL;
    }
    SDL_AudioSpec audioInfo;                                            //this struct contains info about WAV file- frequency, channels, format etc.
    uint8_t *audioBuffer;                                               //buffer for audio file
    uint32_t audioLength;                                               //audio length
    if (!SDL_LoadWAV(filename, &audioInfo, &audioBuffer, &audioLength)) //loading WAV file, allocating memory for buffer
    {
        fprintf(stderr, "Unable to open file %s: [%s]\n", filename, SDL_GetError());
        return ERROR_SDL;
    }
    else //file successfully loaded
    {
        printf("File %s loaded!\n", filename);
    }
    //audio info used in Callback function
    struct audioData_t audioData = {.ptr = audioBuffer, .length = audioLength};
    //setup pointer tocallback function
    audioInfo.callback = AudioCallback;
    audioInfo.userdata = &audioData; //passing this structure into callback function
    //device ID
    SDL_AudioDeviceID device;
    //NULL for system deafult audio device,0 not recording
    //audioInfo represents desired output format, NULL optained format (not required)
    //flag _ANY_CHANGE allow SDL to make some changes in order to play audio if desired format is not available.
    //for example SDL can change audio format float32 to int16 if device does not support float
    if (!(device = SDL_OpenAudioDevice(NULL, 0, &audioInfo, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE)))
    {
        fprintf(stderr, "Unable to open device [%s]\n", SDL_GetError());
        return ERROR_SDL;
    }
    //freq*length
    printf("playing %s ...\n", filename);
    SDL_PauseAudioDevice(device, 0); //unpause device-play sound
    while (audioData.length > 0)
        SDL_Delay(100);
    //close audio device
    printf("%s ended\n", filename);
    SDL_CloseAudioDevice(device);
    //dispose WAV file buffer
    SDL_FreeWAV(audioBuffer);
    //quit SDL when program ends
    atexit(SDL_Quit);
    return ERROR_SUCCESS;
}
void AudioCallback(void *userdata, Uint8 *stream, int len)
{
    if (!userdata || !stream)
        return;
    struct audioData_t *audioData = (struct audioData_t *)userdata;
    if (audioData->length == 0){
         memset(stream,0,len); //clear output stream, prevent undefined noises
        return;

    }
    //length of audio data to copy into buffer
    int length = (audioData->length < len ? audioData->length : len);
    memcpy(stream, audioData->ptr, length);
    audioData->ptr += length;
    audioData->length -= length;
}
