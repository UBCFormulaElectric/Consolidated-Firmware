import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import plotly.graph_objects as go
from tkinter import *


root = Tk()

def retrive_input():
    inputVal = entry_ID.get() #reading user input id to graph
    print(inputVal)
    print(type(inputVal))


label_ID = Label(root, text='ID to graph')
entry_ID = Entry(root)

label_ID.grid(row=0)
entry_ID.grid(row=1)

button_retrieve_ID = Button(root, text='new graph', command=retrive_input)
button_retrieve_ID.grid(row=2)

root.mainloop()


data = pd.read_csv('0000129.TXT', sep=';', header =16, quoting=3, error_bad_lines=False)

oneid = data.loc[data['ID'] == 101]

oneid.Data = oneid.Data.apply(lambda x: int(x, 16))

oneid.Timestamp = pd.to_datetime(oneid.Timestamp, format='%Y%m%d%H%M%S%f')

fig = go.Figure([go.Scatter(x=oneid['Timestamp'], y=oneid['Data'])])
fig.show()

print(data)
print(data.columns)
print(oneid)
print(oneid.Timestamp)
