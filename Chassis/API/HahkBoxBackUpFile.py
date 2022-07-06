import dash
from dash.dependencies import Output, Input
from dash import Dash, Input, Output
#import dash_core_components as dcc
from dash import dcc
import dash_html_components as html

#import plotly.express as px
#import plotly.io as pio
import plotly
import dash_daq as daq
#import numpy as np
#import array as arr
import random
import plotly.graph_objs as go
import pandas as pd
import numpy
from datetime import datetime
from plotly.subplots import make_subplots
##from random import seed
from random import randint
from collections import deque



randomRange = 0
max = 30
X = deque(maxlen=20)
X.append(1)
timee = deque(maxlen = max)
Y = deque(maxlen=max)
Y2 = deque(maxlen=max)
Y3 = deque(maxlen=max)
Y4 = deque(maxlen=max)
ch1 = numpy.array([["Time ", "Temperature"]])



#df = pd.DataFrame({"Card data 1": Y,  "Time": timee })


app = dash.Dash(__name__)




# --------------------------------- Tab stylization------------------------------------------------#

tabs_styles = {
    'height': '44px'
}
tab_style = {
    'borderBottom': '1px solid #d6d6d6',
    'padding': '16px',
    'fontWeight': 'bold'
}

tab_selected_style = {
    'borderTop': '1px solid #d6d6d6',
    'borderBottom': '1px solid #d6d6d6',
    'backgroundColor': '#119DFF',
    'color': 'white',
    'padding': '16px'
}


#-------------------------- app Layout--------------------------------------------------------------------#

app.layout = html.Div([

    html.Div(id='tabs-content'),
    dcc.Store(id='session', storage_type='session'),

    dcc.Tabs(

        id = "tabs-with-classes",
        value = 'tab2',

        children = [

            dcc.Tab (
                
                label = 'Tab 1',
                value = 't1',
               # tab_id = 't1',
                style = tab_style, 
                selected_style = tab_selected_style,
                className = 'custom-tab',
                selected_className = 'custom-tab-selected',
                children = [

                    #current/y working on adding input range feature to change the amount of data can be viewed

                    dcc.Input(id="range", type="number", placeholder="", style={'marginRight':'10px'}),
                    html.Div(id="output"),

                    daq.Indicator(
                        id='my-daq-indicator',
                        value=True,

                        color="#00Bc96"
                        ), 
                        #randomRange : 50

                ]


            ),
            dcc.Tab (

                label = '4 wire ac',
               # tab_id= 't2',
                value = 't2',
                className = 'custom-tab',
                style=tab_style, 
                selected_style=tab_selected_style,
                selected_className = 'custom-tab-selected',
                children = [
                html.Div(id='live-update-text'),

                

                    dcc.Dropdown(
                        options=[
                            {'label': '2 wire', 'value': 'NYC'},
                            {'label': 'PID', 'value': 'MTL'},
                            {'label': '4 wire DC', 'value': 'SF'},
                            {'label': '4 wire AC', 'value': 'me'}

                        ],
                        value='me'
                    ),  

                    #'randomRange' : 5,


                    dcc.Checklist(
                        id = 'box',
                        options=[
                            {'label': 'Resistance', 'value': 'R'},
                            {'label': 'Voltage', 'value': 'V'},

                        ],
                        
                        value= ['R']
                        
                    ),
                    html.Button("Download CSV", id="btn_csv"),
                    dcc.Download(id="download-dataframe-csv"),

                


                ]),




                #]

            
        ]
    ),
    html.Button('test for Hiding tabs', id="hide_1_button"),
    html.Div(id='tabs-content-classes'),

    html.Div([        

#---------------------- One Graph ----------------------------------------------------------#
         

# -----------------------------------------------------------------

#---------------------- two Graph ------------------------------
        dcc.Graph( 
            id='live-graph2',
            #animate = True
            
        ),

        dcc.Interval(
            id='graph-update2',
            interval= 1*1000,
            n_intervals = 0
        ),


# -----------------------------------------------------------------

])

])

@app.callback(
[Output('tabs-content', 'children')],
Input('tabs-with-classes', 'value'),


prevent_initial_call=True
# ^^^^^^ look up what the above function does ^^^^^^^
)
def render_content(tab):
    if (tab == 't1'):
        randomRange = 5
    if (tab == 't2'):
        randomRange = 68



@app.callback(dash.dependencies.Output('live-update-text', 'children'),
              dash.dependencies.Input('graph-update', 'n_intervals'))
              
def update_metrics(n):
    t = timee.pop()
    timee.append(t)
    ti = t

    v = Y.pop()
    Y.append(v)
    val = v
    style = {'padding': '5px', 'fontSize': '26px'}
    return [
        html.Span('Value: {}'.format(v), style=style),
        html.Span('Value: {}'.format(t), style=style),

    ]

# -------------------------------- Graph Update TWO--------------------------




@app.callback(
    Output('live-graph2', 'figure'),
    Input('graph-update2', 'n_intervals'),


    #Input('box', 'value')
    )

#value' goes in the update function
def update_graph_scatter2(n):
    randomRange = 1
#--------------------------------------------------------------------------------------------------------------------
#   this portion of code will probably be the only place needed to add the information from the API


    style = {'padding': '10px'}

    X.append(X[-1]+1)
   # y = randint(0,4)
    
    #random y values to test that the graphs can update live.
    Y.append(random.uniform(0,randomRange))
    Y2.append(random.uniform(0,randomRange))
    Y3.append(random.uniform(0,randomRange))
    Y4.append(random.uniform(0,randomRange))
    ch1 = numpy.array([["Time ", "Temperature"], [timee ,'\n', Y, '\n']])

 
   
   
# this is currently not in use but may be useful for organizaing data with API

    # data = {

    #     'time' : [],
    #     'full': [],
    #     'vis': [],
    #     'lux': [],
    #     'inf': []

    # }
    

    now = datetime.now()
    timee.append(now)
    
    
    #-------------------------------------------------------------------------------------------

    
    fig = make_subplots(rows=2, cols=2, vertical_spacing=0.1,

    subplot_titles=("Channel 1","Channel 2", "Channel 3", "Channel 4")
    
    
    )
    
    fig['layout']['margin'] = {
        'l': 50, 'r': 20, 'b': 40, 't': 30
    }
    fig['layout']['legend'] = {'x': 1.5, 'y': 1, 'xanchor': 'left'}
    #timee.append(timee[datetime.now()]-datetime.now()
    


    t = plotly.graph_objs.Scatter(

        x = list(timee),
        y  = list(Y),



    )
    t2 = plotly.graph_objs.Scatter(

        x = list(timee),
        y  = list(Y2),



    )

    t3 = plotly.graph_objs.Scatter(

        x = list(timee),
        y  = list(Y3),



    )

    t4 = plotly.graph_objs.Scatter(

        x = list(timee),
        y  = list(Y4),



    ),

#--- this portion of the code creates the colored line for each graph based on the values it recives from the 
#--- previous value, this traces a line between each scatter points for each graph

    fig.add_trace(
        go.Scatter(
        t,
   


        ), 1, 1
    )
    fig.add_trace(
    go.Scatter(
        t2,
    

    ), 1, 2

    )

    fig.add_trace(
    go.Scatter(
        t3,
        

    ), 2, 1
    )
    fig.add_trace(

    go.Scatter(
        t,
        

    ), 2, 2
    )

    #fig.update_xaxes( autorange = True)
    fig.update_layout(template = 'plotly_dark' ) # xaxis=dict(range=[min(X),max(X)]))
    
    
    #fig.update_xaxes(range=list(timee))

    
    global df 
    df = pd.DataFrame({"Card data 1": Y,  "Time": timee })


    return fig



@app.callback(
    Output("download-dataframe-csv", "data"),
    Input("btn_csv", "n_clicks"),
    prevent_initial_call=True,
)
def func(n_clicks):

    file = numpy.savetxt("4AC-ch1.txt", ch1, delimiter=',', fmt ="%s")

    print(file)
    return dcc.send_file(file, '4AC-ch1.txt')
   # return dcc.send_data_frame(df.to_csv, "mydf.csv")






if __name__ == '__main__':
    app.run_server(debug= True   )#, dev_tools_hot_reload = False) 
