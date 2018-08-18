import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import sys


argc = len(sys.argv)
if argc>1:
	fichier = sys.argv[1]
	if (argc>2):
		lim = sys.argv[2]
	else:
		lim = 512
	array_img = mpimg.imread(fichier)
	if array_img.dtype != np.float32:
		array_img = array_img.astype('float')/255.
	print np.mean(array_img)
	if (len(array_img[0]) > lim):
		n_copy = len(array_img[0])//lim
		if (len(array_img[0])//lim < len(array_img[0])*1./lim):
			n_copy += 1
		new_img = np.ones((len(array_img)*n_copy,lim,4))
		for i in range(n_copy-1):
			new_img[i*len(array_img):(i+1)*len(array_img),:,:] = array_img[:,i*lim:(i+1)*lim,:]
		for i in range(len(array_img)):
			for j in range(len(array_img[0])-lim*(n_copy-1)):
				new_img[(n_copy-1)*len(array_img)+i,j,:] = array_img[i,(n_copy-1)*lim+j,:]

		
		string_fichier = fichier[:fichier.find('.')]
		extension = fichier[fichier.find('.'):]
		mpimg.imsave(string_fichier+"_"+str(lim)+extension, new_img)
	else:
		print "Image Ok with the limit, nothing to do."

else:
	print "This programs needs one argument: name of image file to cut."
	
	
