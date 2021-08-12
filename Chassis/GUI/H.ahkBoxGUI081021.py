import dash
from dash.dependencies import Output, Input
import dash_core_components as dcc
import dash_html_components as html
import plotly.express as px
import plotly.io as pio
import plotly
import dash_daq as daq
import numpy as np
import array as arr
import random
import plotly.graph_objs as go
import pandas as pd
from datetime import datetime
from plotly.subplots import make_subplots
from random import seed
from random import randint
from collections import deque


max = 20
# x list is not used only used as a test value
X = deque(maxlen=20)
X.append(1)

time = deque(maxlen = max)
Y = deque(maxlen=max)
Y2 = deque(maxlen=max)
Y3 = deque(maxlen=max)
Y4 = deque(maxlen=max)

#df = pd.DataFrame({"Card data 1": Y,  "Time": time })


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
    
    dcc.Store(id='session', storage_type='session'),

    dcc.Tabs(

        id = "tabs-with-classes",
        value = 'tab2',

        children = [

            dcc.Tab (
                
                label = 'Tab 1',
                value = 'home',
                id = 't2',
                style=tab_style, 
                selected_style=tab_selected_style,
                className = 'custom-tab',
                selected_className = 'custom-tab-selected',
                children = [

                    # test component can be implemented into something else later on 
                    #if throught to be useful otherwise this was orginally as test for placing
                    # other components in tabs
                    daq.Indicator(
                        id='my-daq-indicator',
                        value=True,

                        color="#00Bc96"
                        ) 
                ]


            ),
            
            dcc.Tab (

                label = '4 wire ac',
                id= 't',
                value = 'Full',
                className = 'custom-tab',
                style=tab_style, 
                selected_style=tab_selected_style,
                selected_className = 'custome-tab-selected',
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

#---------------------- LIVE GRAPH----------------------------------------------------------#
         
        dcc.Graph( 
            id='live-graph2',
            #animate = True
            
        ),

        dcc.Interval(
            id='graph-update2',
            interval= 1*1000,
            n_intervals = 0
        ),


# -----------------------------------------------------------------------------------------#

])

])


#-------------------------------------test button ----------------------------------------#
# only a test button to test deleting and hiding tabs
# this is not need
#---------------------delete -----------------------#


@app.callback(
[Output('t2', 'style'),Output('t2', 'selected_style')],
Input('hide_1_button', 'n_clicks'),
prevent_initial_call=True,
)
def hide_tab1(_):
    return {"display": "none"}, {"display": "none"}

#--------------------------------------------------#
#-----------------------------------------------------------------------------------------#



#-------------------------------------text value update-----------------------------------#

@app.callback(dash.dependencies.Output('live-update-text', 'children'),
              dash.dependencies.Input('graph-update', 'n_intervals'))
def update_metrics(n):
    """
    # I don't like this portion of code but at the time this 
    # seemed like the easiest way to get just the lastest value added
    # to the queue, going to try and find an easier and simplier way
    # so its not removing and readding the value to the queue.
    
    t variable is : TIME
    v variables is: Y (random) Value
    """
    
    t = time.pop()
    time.append(t)
    ti = t

    v = Y.pop()
    Y.append(v)
    val = v
    style = {'padding': '5px', 'fontSize': '26px'}
    return [
        html.Span('Value: {}'.format(v), style=style),
        html.Span('Value: {}'.format(t), style=style),

    ]

#-------------------------------------------------------------------------------
"""
graph is split into 4 subplots
the data being added is only random generated numbers
the x axis used real datetime
each graph as four seprate traces
more traced can be added if added under the subplot of graph

"""

# -------------------------------- LIVE GRAPH Callback--------------------------



@app.callback(
    Output('live-graph2', 'figure'),
    Input('graph-update2', 'n_intervals')


    #Input('box', 'value')
    )

#value' goes in the update function
def update_graph_scatter2(n):


    style = {'padding': '10px'}

    X.append(X[-1]+1)
    y = randint(0,4)
    
    #just test data
    Y.append(random.uniform(0,4))
    Y2.append(random.uniform(0,50))
    Y3.append(random.uniform(0,10))
    Y4.append(random.uniform(0,24))

   
   

#     data = {

#         'time' : [],
#         'full': [],
#         'vis': [],
#         'lux': [],
#         'inf': []

#     }

     #x axis time data
    now = datetime.now()
    time.append(now)

    #creates the subplots, size spacing ect.
    fig = make_subplots(rows=2, cols=2, vertical_spacing=0.1)
    fig['layout']['margin'] = {
        'l': 30, 'r': 10, 'b': 30, 't': 10
    }
    fig['layout']['legend'] = {'x': 0, 'y': 1, 'xanchor': 'left'}
    #time.append(time[datetime.now()]-datetime.now()
    
    """
    These are traces for creating the lines
    there are currently 4 one for each graph
    more can be added and possibly added to appear at the same time 
    as the other traces
    
    these extra lines traces are intented to be synced up with buttons and dropdown box component values
    
    """


    t = plotly.graph_objs.Scatter(

        x = list(time),
        y  = list(Y),



    )
    t2 = plotly.graph_objs.Scatter(

        x = list(time),
        y  = list(Y2),



    )

    t3 = plotly.graph_objs.Scatter(

        x = list(time),
        y  = list(Y3),



    )

    t4 = plotly.graph_objs.Scatter(

        x = list(time),
        y  = list(Y4),



    ),

    fig.add_trace(
        go.Scatter(
        t,
   

#        
        ), 1, 1
        # ^^^ subplot location ^^^^
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
        t4,
        

    ), 2, 2
    )

    #fig.update_xaxes( autorange = True)
    
    #these is a premade theme by plotly can easily remove this theme or change it.
    fig.update_layout(template = 'plotly_dark' ) # xaxis=dict(range=[min(X),max(X)]))
    
    
    #fig.update_xaxes(range=list(time))

    

    return fig



#-----------------------------------------------------------------------------------------

"""
downloads as intended however data is localized in only the graph function. 
need to figure out how to make data available to the download button
this will possible need to be changed into a text file instead of csv
the data file name will have to change.

when it is added and used in the h.ahk box its better to have it display a date and time in the name
and the name of the card the file is from. this will make it easier to log data and save it for 
viewing at a later period of time. 

may need another option to allow for viewing of previous logged data files to be uploaded and 
viewed as a graph.

this file should save something like this may need to be changed

    Value (temp technically)       TIME
    1                               date/time
    2                               date/time
    3                               date/time

"""


#-------------------------Download cvs button---------------------------------------------


df = pd.DataFrame({"Card data 1": Y,  "Time": time })
@app.callback(
    Output("download-dataframe-csv", "data"),
    Input("btn_csv", "n_clicks"),
    prevent_initial_call=True,
)
def func(n_clicks):
    return dcc.send_data_frame(df.to_csv, "CurrentData.csv")

#----------------------------------------------------------------------------------------








if __name__ == '__main__':
    app.run_server(debug= True) 
