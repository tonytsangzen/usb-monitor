#!/usr/bin/python
import hid
import psutil
import font
import screen
import style
import sysinfo
import json
import sys
import getopt
import getpass

hostname = ''; username = ''; passwd = ''
#passopt
try:
    opts, args = getopt.getopt(sys.argv[1:],"hr:u:p:",["remote=", "user=", "passwd="])
except getopt.GetoptError:
    print 'legency.py -r hostname -u username -p password'
    sys.exit(-1)

for opt, arg in opts:
    if opt == '-h':
        print 'legency.py -r hostname -u username -p password'
        sys.exit()
    elif opt in ("-r", "--remote"):
        hostname = arg
    elif opt in ("-u", "--user"):
        username = arg
    elif opt in ("-p", "--passwd"):
        passwd = arg

#get username add passwd
if hostname:
    if not username:
        username = raw_input("Remote User Name:")
    if not passwd:
        passwd =  getpass.getpass("password:")
    try:
        sysinfo.connect(hostname, username, passwd)
    except Exception,e:
        print(e)
        exit(-1)
    print('Sucess connect to host:' + hostname)

#connect to usb hid device
dev = hid.hid(0x0483, 0x5750)
scr = screen.display(128,32)

while True:
    #get sytem info
    try:
        info = json.loads(sysinfo.info())
        style.display_percent_bar(scr, 0 , 'CPU:', info['cpu'])
        style.display_percent_bar(scr, 1 , 'MEM:', info['mem'])
        style.display_percent_bar(scr, 2 , 'DSK:', info['disk'])
        #scr.write(info['time'] + '\n')
    except Exception,e:
        print e
        scr.clear()
        scr.write(str(e))
    #display
    dev.write(scr.data())

