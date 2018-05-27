##############IMPORT#################################

import subprocess as sp
import numpy as np
from PIL import Image
#import Image
import os
import re
import serial
import syslog
import time
import json
import sys
import datetime

#####################################################

def imageManage(image): 
    image = image.reshape((720,1280,3))
    im = Image.fromarray(image)
    w, h = im.size
    im_resize = im.resize((w/4, h/4), Image.ANTIALIAS)
    return im_resize

def getImFromVid(vid):
    command = [ 'ffmpeg',
            '-i', vid,  #indicates the input file
            '-f', 'image2pipe',
            '-pix_fmt', 'rgb24',
            '-vcodec', 'rawvideo', '-']
    pipe = sp.Popen(command, stdout = sp.PIPE, stderr=sp.PIPE, bufsize=10**8)

    # read 420*360*3 bytes (= 1 frame)
    raw_image = pipe.stdout.read(1280*720*3)
    # transform the byte read into a numpy array
    image =  np.fromstring(raw_image, dtype='uint8')
    # throw away the data in the pipe's buffer.
    pipe.stdout.flush()
    return image

def splitImage(path, im, h, w, k):
    im_w, im_h = im.size
    for i in range(0,im_h, h):
        for j in range(0,im_w,w):
            box = (j, i, j+w, i+h)
            a = im.crop(box)
            a.save(os.path.join(path,"IMG-%s.jpg" % k))
            k += 1
    return k

#---------------(main)--------------------------------------------------

def main():
	with open('../Project/config.json','r') as f:
		config = json.load(f)

	port = config['PATH']['PORT']
	vid_name = config['PATH']['VIDEO_NAME']
	split_path = config['PATH']['SPLIT_DEST_PATH']
	image = imageManage(getImFromVid(vid_name))
	imagename = config['PATH']['IMG']
	image.save(imagename,"JPEG",optimize=True,quality=20) #reduce size

	k = splitImage(split_path, image,18,32,0)
	n = int(k)-1
	rawdata = ""

	for i in range(n):
		print(i)
		test_im = open(split_path+'/IMG-%s.jpg' % i, 'rb')  #maybe bug
		im_data = test_im.read()
		test_im.close()

		txt = list(im_data)
		n = len(txt)
		print n

		ser = serial.Serial(port,9600,timeout=5)

		
		count = 1
		ext = 0
		time1=0
		while True:

		 	rec = ser.read(1000)
		 	print ("Received : ")
		 	print(rec)
			if "get" in rec:
				tmp = ""
				for i in range (32):
					if not txt:
						ext = 1
						break
					tmp += txt.pop(0)
				print ("Python value sent: ")
				ser.write(tmp)
				print tmp

				if time1 ==0:
					time1 = datetime.datetime.utcnow()
				time2 = datetime.datetime.utcnow()
				print ("Time")
				print(time2-time1)			
			
			if ext == 1:
				ser.write("end")
				print ("end")
				
				time2 = datetime.datetime.utcnow()
				total_time = time2-time1
				print ("TimeEnd")
				print(total_time)
				break       
    

if __name__ == "__main__":
	main()