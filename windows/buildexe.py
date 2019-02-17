from distutils.core import setup
import py2exe
import sys

sys.path.append("../host/src/")
sys.path.append("C:/Python27/Lib/site-packages")
setup(
        console=["../host/src/legency.py"],
		options = {'py2exe': {'compressed': True}}
) 
