# -*- coding: utf-8 -*-
"""
Created on Thu Apr  1 10:55:15 2021

@author: Keith
"""

import pandas as pd
import plotly
import plotly.graph_objs as go

print("Getting Data...", end=" ")

df = pd.read_csv("Data/3_30_S0.CSV")

time = df['time'] #Add to the top of the data file time,ultrasonic,pressure
US = df['ultrasonic']
p = df['pressure']

hour = time/(3600*1000) #convert from ms to hours
height = 6.5-US/30.48 #convert US to gage height
#TODO pressure to gage height
print("Task Failed Successfully!")

print("Cleaning data...\nRemoving row(s):", end=" ")
i = 3
part = 0.05 #uses regression to determine if the next point is valid, this is the variance (example 0.1 = 10%)
while i<len(height): #clean the data
    if i==3:
        projection = height[3]
    else:
        projection=m*time[i]+b
    if height[i]>projection*(1+part) or height[i]<projection*(1-part):
        height.pop(i)
        
        print(i, end=" ")
        k = i+1
        while True:
            prevH1 = height[k]
            prevH2 = height[i-1]
            prevT1 = time[k]
            prevT2 = time[i-1]
            m=(prevH2-prevH1)/(prevT2-prevT1)
            b=prevH1-m*prevT1
            projection = m*time[k]+b
            if height[k]>projection*(1+part) or height[k]<projection*(1-part):
                height.pop(k)
            else:
                k=k+1
                i=k
                break
    else:
        prevH1 = height[i]
        prevH2 = height[i-1]
        prevT1 = time[i]
        prevT2 = time[i-1]
        m=(prevH2-prevH1)/(prevT2-prevT1)
        b=prevH1-m*prevT1
        i=i+1
print("\nFinished Mostly Cleaning Data") 

print("Making Graph...", end=" ")

trace0 = go.Scatter(
    x = hour,
    y = height,
    mode = 'lines',
    name = 'UltraSonic'
)

trace1 = go.Scatter(
    x = hour,
    y = p,
    mode = 'lines',
    name = 'Pressure'
)

data = [trace0] #change to [trace0,trace1] to add pressure
layout = dict(title = 'Rainfall',
              xaxis = dict(title = 'Time (Hours)'),
              yaxis = dict(title = 'Water Height (Feet)'),
              )

fig = dict(data=data, layout=layout)

plotly.offline.plot(fig,filename='Water_Height.html')

print("Finished")