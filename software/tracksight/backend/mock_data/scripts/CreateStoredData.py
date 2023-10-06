import pandas as pd
import numpy as np

date_rng = pd.date_range(start='2023-09-14', end='2023-09-16', freq='D')

data = {
    'Signal1': np.random.randn(len(date_rng)),
    'Signal2': np.random.randn(len(date_rng)),
    'Signal3': np.random.randn(len(date_rng))
}

df = pd.DataFrame(data, index=date_rng)
df.index.name = 'date_time'
print(df)

file_path = '../data/sample_data.csv'

df.to_csv(file_path)
