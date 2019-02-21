# usb-monitor
usb-monitor is a opensorce project for monitor pc's cpu/mem/net/temp through usb port . 

## target image
![Image text](https://raw.githubusercontent.com/tonytsangzen/usb-monitor/master/doc/target.jpeg)

## System Require
#### 1.runtime
python libusb libhidapi
#### 2.python module
paramiko psutil

## HOW TO
#### run on windows
1.[install python 2.7 on windows](https://www.python.org/downloads/release/python-2715/)

2.Update pip 
```
python.exe -m pip install -U pip
```

3.Install python module
```
python.exe -m pip install paramiko psutil
```

4. Copy windows\hidapi\win32(x64)\hidapi.dll driver to c:\windows\system32

5.run service 
```
python.exe src\legency.py
```
