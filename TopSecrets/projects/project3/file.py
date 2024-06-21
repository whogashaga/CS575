import pandas as pd
pd.plotting.register_matplotlib_converters()

import matplotlib.pyplot as plt
import seaborn as sns

# Path of the file to read
data_path = "./result.csv"

# Fill in the line below to read the file into a variable ign_data
data = pd.read_csv(data_path)

plt.figure(figsize=(14,6))
plt.title("Figure 1. Simulation's global states over time")

data1 = data.drop(columns=['Year', 'Month'])

palette = sns.cubehelix_palette(rot=3, dark=0.1, gamma=0.8)
p = sns.lineplot(data=data1)

p.set_xlabel("#Month")