import matplotlib.pyplot as plt
import pandas as pd
file_name = "./g.csv"
df = pd.read_csv(file_name , sep="," , encoding="utf-8")
plt.plot(df['EPOCH'],df['E-Avergae'])
plt.xlabel('EPOCH')
plt.ylabel('E-Average')
plt.show() 