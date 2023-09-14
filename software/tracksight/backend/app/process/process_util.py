import pandas as pd

"""
Get a list of ids from the stored data
returns in JSON format with the datetime index
"""
def get_signal(id):
    df = pd.read_csv("../../mock_data/data/sample_data.csv")
    df = df.set_index('date_time')
    print(df)
    return df[id].to_json()

print(get_signal("Signal1"))
