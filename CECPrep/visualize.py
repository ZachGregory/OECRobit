import plotly.express as px
import pandas as pd


pd.options.display.max_rows = 999
df = pd.read_csv(r"C:\Users\zachg\Downloads\sketchyLidar\data.csv")
fig = px.scatter_3d(df, x='X', y='Y', z='Z',
                    color='Dist')
fig.show()