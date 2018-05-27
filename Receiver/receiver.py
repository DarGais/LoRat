##############IMPORT#################################

import subprocess as sp
import numpy as np
from PIL import Image
import os
import re
import serial
import syslog
import time
import json
import io
import StringIO

#####################################################

def clean(L):
    infile = L
    outfile = "cleaned_file.txt"

    delete_list = ["initializing", "Setting freq", "No ACK", "\r\n"]
    fin = open(infile)
    fout = open(outfile, "w+")
    sep = "end"
    for line in fin:
        for word in delete_list:
            line = line.replace(word, "")
        if sep in line:
            line = line.split(sep,1)[0]
        if line.strip() != "":
            fout.write(line)  # want to remove 0d0a
    fin.close()
    fout.close()

    return outfile

def toImg(L,imgname):
    with open("cleaned_file.txt", mode='rb') as file: # b is important -> binary
        fileContent = file.read()

    image_data = fileContent
    image = Image.open(io.BytesIO(image_data))
    image.save(imgname,"JPEG")
    image.show()

#---------------(main)--------------------------------------------------

def main():

    recFile = "rec.txt"

    with open('../Project/config2.json','r') as f:
        config = json.load(f)

    port = config['PATH']['PORT']
    imgname = config['PATH']['IMG']

    ser = serial.Serial(port,9600,timeout=1)


    receive = ""
    read = ""

    while True:
        read = ser.read(1000)
        print(read)
        if read != "No ACK":
            receive += read      

        if "end" in read:
            break

        time.sleep(6)

    #print "out of while loop"


    with open(recFile,"w+") as txt:
        txt.write(receive)

    toImg(clean(recFile),imgname)
	
    

if __name__ == "__main__":
	main()