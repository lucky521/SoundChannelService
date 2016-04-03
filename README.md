Windows Sound Channel Service
====
Keep the audio channel open between real audio-in device and vitual driver.


## install service
Command Prompt>  SoundChannelService.exe  -install

## uninstall service
Command Prompt>  SoundChannelService.exe  -remove

## Check 
HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\services\ + luSoundChannelService

Windows Task Manager -> Services  > luSoundChannelService


## This project can be a tutorial of writing a service

- Define the settings of the service
- Add the ServiceBase.h and ServiceBase.cpp files to provide a base class for a service that will exist as part of a service application.
- Add the SampleService.h and SampleService.cpp files to provide a sample service class that derives from the service base class CServiceBase.
- Add the ServiceInstaller.h and ServiceInstaller.cpp files to declare and implement functions that install and uninstall the service.


