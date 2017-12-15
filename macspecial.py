#!/usr/bin/python
from __future__ import print_function
import os
import subprocess
import warnings

full_cmd="clang  -x c++ -E  testboost.cpp"

def mac_fix_boost():
    import subprocess

    proc1  = subprocess.Popen(full_cmd,shell=True, stderr=subprocess.PIPE)
    output_bad = proc1.stderr.read()
    if "fatal error" in str(output_bad) or "command not found" in str(output_bad):
        print("********************************************************\n*  We could not found a cpp-Boost version on your Mac  *\n********************************************************\n")
        print("\t Follow the instruction here: http://fb09-pasig.umwelt.uni-giessen.de/cmf/wiki/CmfInstall \n \t or use the automated installation process:\n\n")
        print("Do you want to auto install Boost (a version of brew will installed before, which also install XCode Tool. You will get a premission request.)?")
        userchoice = str(input("[Y/n]: "))
        if userchoice == "Y" or userchoice == "y":
            run_mac_install()
    else:
        print("\t ✓ Nothing to do 🙂")

def run_mac_install():
    os.system('/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"')
    proc  = subprocess.Popen("brew",shell=True, stderr=subprocess.PIPE)
    output_bad = proc.stderr.read()
    if "command not found" in str(output_bad):
        warnings.warn("📛Sorry, but brew could not be installed. We abort the install process📛")
    else:
        os.system('brew install boost')
        proc1  = subprocess.Popen(full_cmd,shell=True, stderr=subprocess.PIPE)
        output_bad = proc1.stderr.read()
        if "fatal error" not in str(output_bad):
            print("\t 👊 Boost has been installed successfully 🍻")

        else:
            warnings.warn("Sorry, but there was an error during the installation of Boost. Please contact the authors of the CMF package")
