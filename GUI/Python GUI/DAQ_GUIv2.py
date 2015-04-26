# -*- coding: utf-8 -*-
"""

Filename: DAQ_GUIv2.py

Created on Tue Mar 03 11:52:07 2015

@author: James
"""

#lineplot.py
#matlab esque operations
import numpy as numpy
#plotting functionality
import pylab as pl, mpld3
#for checking file exists
import os.path


#set the sample rate (in Hz)
sampRate = 10

fileExists = False
filename = ""

def openInputFile():
    global  fileExists
    global filename
    
    filename = raw_input("Enter the filename you wish to load data from: ")
    if(os.path.exists(filename)):
        # load text from file accelGyro.txt
        data = numpy.loadtxt(filename)
        fileExists = True
        #print(data)
        return data
    else:
        fileExists = False
        return 0

#not sure if this method is needed, because the 
def setDataArrays(data):
    accelX = data[:,0]
    accelY = data[:,1]
    accelZ = data[:,2]
    gyroX = data[:,3]
    gyroY = data[:,4]
    gyroZ = data[:,5]
    dataArray = [accelX, accelY, accelZ, gyroX, gyroY, gyroZ]
    
    return dataArray
                
def graphData(data):  
    #enables graphing 
    mpld3.enable_notebook()
    
    #pulls from 2d data array the accel and gyro 1d arrays
    #so they can be filtered
    
    accelX = data[:,0]
    accelY = data[:,1]
    accelZ = data[:,2]
    gyroX = data[:,3]
    gyroY = data[:,4]
    gyroZ = data[:,5]
    
    
    #find num samples for accel (should be same # for all axes)
    numSamples = len(accelX)

    time = []
    
    #fill time array with values from 0 to numSamples
    for x in range(0, numSamples):
        time.append(x ) 
        
    #create figure for sub plotting    
    f1 = pl.figure(num=None, figsize=(50,50), dpi=80, facecolor='w'
    , edgecolor='k')    
    
    #change window titleS
    f1.canvas.set_window_title('Accelerometer Graphs for file: ' + filename)            

    #adjust margins of subplots
    pl.subplots_adjust(left=0.2, right=.8, wspace=0.1, hspace=.1)

    #plot unfiltered x-data    
    pl.subplot2grid((3,1),(0,0))
    pl.title('Acceleration in x vs time (Unfiltered)')
    
    #create a red dashed line    
    pl.plot(time, accelX, 'r--')

    #set x axis label to be time
    pl.xlabel("time (milliseconds)")

    #set x axis limits (beginnning, end)
    pl.xlim(0 , numSamples)
    
    #set y axis to be acceleration in x
    pl.ylabel("acceleration (m/s^2)")
    
    #set y axis limits (beginnning, end)
    pl.ylim(-1.5 , 1.5)
    
    
    #plot unfiltered y-data
    pl.subplot2grid((3,1),(0,1))
    pl.title('Acceleration in y vs time (Raw)')
    
    #create a green dashed line    
    pl.plot(time, accelY, 'g--')

    #set x axis label to be time
    pl.xlabel("time (milliseconds)")

    #set x axis limits (beginnning, end)
    pl.xlim(0 , numSamples)
    
    #set y axis to be acceleration in y
    pl.ylabel("acceleration (m/s^2)")
    
    #set y axis limits (beginnning, end)
    pl.ylim(-1.5 , 1.5)
    
    
    #plot unfiltered z-data
    pl.subplot2grid((3,1),(0,2))
    pl.title('Acceleration in z vs time (Raw)')
    
    #create a blue dashed line    
    pl.plot(time, accelZ, 'b--')

    #set x axis label to be time
    pl.xlabel("time (milliseconds)")

    #set x axis limits (beginnning, end)
    pl.xlim(0 , numSamples)
    
    #set y axis to be acceleration in z
    pl.ylabel("acceleration (m/s^2)")
    
    #set y axis limits (beginnning, end)
    pl.ylim(-1.5 , 1.5)
    
    
    #plot filtered x-data    
    pl.subplot2grid((3,1),(1,0))
    pl.title('Acceleration in x vs time (Raw)')
    
    #create a red dashed line    
    pl.plot(time, accelX, 'r--')

    #set x axis label to be time
    pl.xlabel("time (milliseconds)")

    #set x axis limits (beginnning, end)
    pl.xlim(0 , numSamples)
    
    #set y axis to be acceleration in x
    pl.ylabel("acceleration (m/s^2)")
    
    #set y axis limits (beginnning, end)
    pl.ylim(-1.5 , 1.5)
    
    #plot filtered y-data
    pl.subplot2grid((3,2),(1,1))
    pl.title('Acceleration in y vs time (Filtered)')
    
    #create a green dashed line    
    pl.plot(time, accelY, 'g--')

    #set x axis label to be time
    pl.xlabel("time (milliseconds)")

    #set x axis limits (beginnning, end)
    pl.xlim(0 , numSamples)
    
    #set y axis to be acceleration in y
    pl.ylabel("acceleration (m/s^2)")
    
    #set y axis limits (beginnning, end)
    pl.ylim(-1.5 , 1.5)
    
    
    #plot filtered z-data
    pl.subplot2grid((3,2),(1,2))
    pl.title('Acceleration in z vs time (Filtered)')
    
    #create a blue dashed line    
    pl.plot(time, accelZ, 'b--')

    #set x axis label to be time
    pl.xlabel("time (milliseconds)")

    #set x axis limits (beginnning, end)
    pl.xlim(0 , numSamples)
    
    #set y axis to be acceleration in z
    pl.ylabel("acceleration (m/s^2)")
    
    #set y axis limits (beginnning, end)
    pl.ylim(-1.5 , 1.5)
    
    
    #show the plot on screen
    pl.show()
    
#not too sure what this function is doing (moving average)
def movingAverage(dataArray, window_size):
    window= numpy.ones(int(window_size))/float(window_size)
    return numpy.convolve(dataArray, window, 'same')

#this function finds points that exceed the possible range of accelerometer
#values [-2,2] but should be made more general to apply to outlying gyro
#data as well
def removeOutliers(accelArray):    

    outliersRem = []
    for i in range(len(accelArray)):
        outliersRem.append(0)
    for x in range(0,len(accelArray)):
        if (accelArray[x] > 2.0) or (accelArray[x] < -2.0):
            outliersRem[x] = (accelArray[x+1] + accelArray[x-1])/2  
        else:
            outliersRem[x] = accelArray[x]
    return outliersRem
            

def lowPassFilter(accelArray):
    alpha = 0.5
    filtAccel = []
    for i in range(len(accelArray)):
        filtAccel.append(0)
    for x in range(0,len(accelArray)):
        prevAccel = accelArray[x-1]
        filtAccel[x] = accelArray[x]*alpha + prevAccel * (1.0 - alpha)
        return filtAccel        

def main(): 
    dataSet = openInputFile()
    
    if fileExists == True:        
        #filteredData = filterData(dataSet)        
        graphData(dataSet)
    else:
        print("You incorrectly entered the filename or it doesn't exist!")
    
main()

