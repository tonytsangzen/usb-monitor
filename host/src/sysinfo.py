import psutil as pu
import json
import paramiko
import os
import threading
import time
import datetime

SSHClient = None
script = '\
import psutil as pu\n\
import json\n\
import time\n\
import datetime\n\
info={}\n\
info["mem"] = pu.virtual_memory().percent\n\
info["cpu"] = pu.cpu_percent(1)\n\
info["disk"] = pu.disk_usage("/").percent\n\
info["time"] = datetime.datetime.fromtimestamp(time.time()).strftime("%Y-%m-%d   %H:%M:%S")\n\
print(json.dumps(info))\n\
'

def info():
    info = {}
    if not SSHClient:
        info['mem'] = pu.virtual_memory().percent
        info['cpu'] = pu.cpu_percent(1)
        info['disk'] = pu.disk_usage('/').percent
        info['time'] = datetime.datetime.fromtimestamp(time.time()).strftime("%Y-%m-%d   %H:%M:%S")
        return (json.dumps(info) + '\n')
    else:
        try:
            stdin,stdout,stderr = SSHClient.exec_command('echo \'' + script + '\'|python')
            result = stdout.read()
        except Exception,e:
            raise Exception,e
        return result

def connect(host, user, passwd):
    global SSHClient
    try:
        SSHClient = paramiko.SSHClient()
        SSHClient.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        SSHClient.connect(hostname=host, port=22, username=user, password=passwd)
    except Exception,e:
        raise Exception,e

