import subprocess

subprocess.call('stty -F /dev/ttyACM0 1200',  shell=True)