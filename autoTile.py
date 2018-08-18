import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import sys

xSprites = 32
ySprites = 32

def index(A):
    
    iTab = np.zeros((3,4*xSprites//2*ySprites//2)).astype('int')
    
    if (A[1] == '1'):
        x0 = xSprites
        y0 = 0
    elif (A[1] == '2'):
        x0 = 0
        y0 = ySprites
    elif (A[1] == '3'):
        x0 = xSprites
        y0 = ySprites
    elif (A[1] == '4'):
        x0 = 0
        y0 = 2*ySprites
    elif (A[1] == '5'):
        x0 = xSprites
        y0 = 2*ySprites
    else: return [0,0,0]
    
    if (A[0] == 'B'):
        x0 += xSprites//2
    elif (A[0] == 'C'):
        y0 += ySprites//2
    elif (A[0] == 'D'):
        x0 += xSprites//2
        y0 += ySprites//2
    
    for i in range(xSprites//2):
        for j in range(ySprites//2):
            for k in range(4):
                iTab[0][i*4*ySprites//2+j*4+k] = x0+i
                iTab[1][i*4*ySprites//2+j*4+k] = y0+j
                iTab[2][i*4*ySprites//2+j*4+k] = k
    
    return iTab

def indexNew(i,j):
    
    iTab = np.zeros((3,4*xSprites//2*ySprites//2)).astype('int')
    
    x0 = i*xSprites//2
    y0 = j*ySprites//2
    for i in range(xSprites//2):
        for j in range(ySprites//2):
            for k in range(4):
                iTab[0][i*4*ySprites//2+j*4+k] = x0+i
                iTab[1][i*4*ySprites//2+j*4+k] = y0+j
                iTab[2][i*4*ySprites//2+j*4+k] = k
    
    return iTab
    
def createAutoTileBis(img, newImg, iGlobal, jGlobal, nx, ny):


    auto = img[2*iGlobal*xSprites:(2*iGlobal+2)*xSprites,3*jGlobal*ySprites:(3*jGlobal+3)*ySprites,:]
    newAuto = np.ones((5*xSprites,3*ySprites,4))
    
    casesAuto = [['A2','B2','A3','B2','A3','B3','A1','B4','A5','B4'],['C2','D2','C3','D2','C3','D3','C3','D2','C3','D1'],['A4','B4','A5','B4','A5','B5','A5','B1','A1','B4'],['C2','D2','C3','D2','C3','D3','C3','D2','C3','D1'],['A4','B4','A5','B4','A5','B5','A5','B4','A5','B1'],['C4','D4','C5','D4','C5','D5','C1','D2','C1','D2']]
    
    x = np.array([np.arange(10),np.arange(10),np.arange(10),np.arange(10),np.arange(10),np.arange(10)])
    y = np.array([0*np.ones((10)),1*np.ones((10)),2*np.ones((10)),3*np.ones((10)),4*np.ones((10)),5*np.ones((10))])
    
    for i in range(10):
        for j in range(6):
            newAuto[indexNew(i,j).tolist()] = auto[index(casesAuto[j][i]).tolist()]
    
    nAutoTile = jGlobal*nx+iGlobal
    y0 = (nAutoTile//2)*3
    x0 = (nAutoTile%2)*5    
    newImg[x0*xSprites:(x0+5)*xSprites,y0*ySprites:(y0+3)*ySprites,:] = newAuto
    
    return np.swapaxes(newImg,0,1)
'''
def createAutoTile(img, newImg, i, j, nx, ny):

    auto = img[2*i*xSprites:(2*i+2)*xSprites,3*j*ySprites:(3*j+3)*ySprites,:]
    newAuto = np.ones((5*xSprites,5*ySprites,4))
    
    newAuto[:2*xSprites,:2*ySprites,:] = auto[:,ySprites:3*ySprites,:]
    newAuto[2*xSprites:int(2.5*xSprites),:int(0.5*ySprites),:] = auto[int(1*xSprites):int(1.5*xSprites),int(2*ySprites):int(2.5*ySprites),:]
    newAuto[int(2.5*xSprites):3*xSprites,:int(0.5*ySprites),:] = auto[int(0.5*xSprites):int(1*xSprites),int(2*ySprites):int(2.5*ySprites),:]
    newAuto[2*xSprites:int(2.5*xSprites),int(0.5*ySprites):ySprites,:] = auto[int(1*xSprites):int(1.5*xSprites),int(1.5*ySprites):int(2*ySprites),:]
    newAuto[int(2.5*xSprites):3*xSprites,int(0.5*ySprites):ySprites,:] = auto[int(0.5*xSprites):int(1*xSprites),int(1.5*ySprites):int(2*ySprites),:]
    
    newAuto[4*xSprites:int(4.5*xSprites),:int(0.5*ySprites),:] = auto[:int(0.5*xSprites),:int(0.5*ySprites),:]
    newAuto[int(4.5*xSprites):5*xSprites,:int(0.5*ySprites),:] = auto[int(0.5*xSprites):xSprites,:int(0.5*ySprites),:]
    newAuto[4*xSprites:int(4.5*xSprites),int(0.5*ySprites):ySprites,:] = auto[:int(0.5*xSprites),int(1.5*ySprites):2*ySprites,:]
    newAuto[int(4.5*xSprites):5*xSprites,int(0.5*ySprites):ySprites,:] = auto[int(1.5*xSprites):2*xSprites,int(1.5*ySprites):2*ySprites,:]
    
    newAuto[2*xSprites:int(2.5*xSprites),ySprites:int(1.5*ySprites),:] = auto[:int(0.5*xSprites),:int(0.5*ySprites),:]
    newAuto[int(2.5*xSprites):3*xSprites,ySprites:int(1.5*ySprites),:] = auto[int(0.5*xSprites):xSprites,ySprites:int(1.5*ySprites),:]
    newAuto[2*xSprites:int(2.5*xSprites),int(1.5*ySprites):2*ySprites,:] = auto[:int(0.5*xSprites),int(0.5*ySprites):ySprites,:]
    newAuto[int(2.5*xSprites):3*xSprites,int(1.5*ySprites):2*ySprites,:] = auto[int(0.5*xSprites):xSprites,int(2.5*ySprites):3*ySprites,:]
    
    newAuto[3*xSprites:int(3.5*xSprites),ySprites:int(1.5*ySprites),:] = auto[xSprites:int(1.5*xSprites),ySprites:int(1.5*ySprites),:]
    newAuto[int(3.5*xSprites):4*xSprites,ySprites:int(1.5*ySprites),:] = auto[int(0.5*xSprites):xSprites,:int(0.5*ySprites),:]
    newAuto[3*xSprites:int(3.5*xSprites),int(1.5*ySprites):2*ySprites,:] = auto[xSprites:int(1.5*xSprites),int(2.5*ySprites):3*ySprites,:]
    newAuto[int(3.5*xSprites):4*xSprites,int(1.5*ySprites):2*ySprites,:] = auto[int(0.5*xSprites):xSprites,int(0.5*ySprites):ySprites,:]
    
    newAuto[4*xSprites:int(4.5*xSprites),ySprites:int(1.5*ySprites),:] = auto[:int(0.5*xSprites),int(1.5*ySprites):2*ySprites,:]
    newAuto[int(4.5*xSprites):5*xSprites,ySprites:int(1.5*ySprites),:] = auto[int(1.5*xSprites):2*xSprites,int(1.5*ySprites):2*ySprites,:]
    newAuto[4*xSprites:int(4.5*xSprites),int(1.5*ySprites):2*ySprites,:] = auto[:int(0.5*xSprites),int(0.5*ySprites):ySprites,:]
    newAuto[int(4.5*xSprites):5*xSprites,int(1.5*ySprites):2*ySprites,:] = auto[int(0.5*xSprites):xSprites,int(0.5*ySprites):ySprites,:]
    
    newAuto[3*xSprites:4*xSprites,:ySprites,:] = auto[:xSprites,:ySprites,:]
    
    for b1 in range(2):
        for b2 in range(2):
            for b3 in range(2):
                for b4 in range(2):
                    if (b1 or b2 or b3 or b4):
                        n = b1+2*b2+4*b3+8*b4 - 1
                        x0 = int(n%5)
                        y0 = 2+int(n//5)
                        newAuto[x0*xSprites:(x0+1)*xSprites,y0*ySprites:(y0+1)*ySprites,:] = auto[int(0.5*xSprites):int(1.5*xSprites),int(1.5*ySprites):int(2.5*ySprites),:]
                        if (b1):
                            newAuto[x0*xSprites:int((x0+0.5)*xSprites),y0*ySprites:int((y0+0.5)*ySprites),:] = auto[xSprites:int(1.5*xSprites),:int(0.5*ySprites),:]
                        if (b2):
                            newAuto[int((x0+0.5)*xSprites):(x0+1)*xSprites,y0*ySprites:int((y0+0.5)*ySprites),:] = auto[int(1.5*xSprites):2*xSprites,:int(0.5*ySprites),:]
                        if (b3):
                            newAuto[x0*xSprites:int((x0+0.5)*xSprites),int((y0+0.5)*ySprites):(y0+1)*ySprites,:] = auto[xSprites:int(1.5*xSprites),int(0.5*ySprites):ySprites,:]
                        if (b4):
                            newAuto[int((x0+0.5)*xSprites):(x0+1)*xSprites,int((y0+0.5)*ySprites):(y0+1)*ySprites,:] = auto[int(1.5*xSprites):2*xSprites,int(0.5*ySprites):ySprites,:]
    
    nAutoTile = i*ny+j
    y0 = (nAutoTile//2)*5
    x0 = (nAutoTile%2)*5
    
    newImg[x0*xSprites:(x0+5)*xSprites,y0*ySprites:(y0+5)*ySprites,:] = newAuto
    
    return np.swapaxes(newImg,0,1)
    
 '''
    

if (len(sys.argv) > 1):
    fichier = sys.argv[1]
    array_img = mpimg.imread(fichier)
    if array_img.dtype != np.float32:
        array_img = array_img.astype('float')/255.
    
    nx = len(array_img[0])/xSprites
    ny = len(array_img)/ySprites
    nx = nx/2
    ny = ny/3
    array_img = np.swapaxes(array_img,0,1)
    
    nTot = nx*ny
    newImg = np.ones(((nTot//2+nTot%2)*3*xSprites,2*5*ySprites,4))
    for j in range(ny):
        print "Iteration", j+1, "sur", ny
        for i in range(nx):
            newImg = createAutoTileBis(array_img, np.swapaxes(newImg,0,1), i, j, nx, ny)
    
    string_fichier = fichier[:fichier.find('.')]
    extension = fichier[fichier.find('.'):]
    mpimg.imsave(string_fichier+"_t"+extension, newImg)
    
else:

    print "This program needs one argument, the name of the .png or .jpg file."
