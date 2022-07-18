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
import dash_bootstrap_components as dbc
from dash import Input, Output, dcc, html




#randomRange = 0
max = 30
timee = deque(maxlen = max)
Y = deque(maxlen=max)
Y2 = deque(maxlen=max)
Y3 = deque(maxlen=max)
Y4 = deque(maxlen=max)
#ch = numpy.array([]) 



#df = pd.DataFrame({"Card data 1": Y,  "Time": timee })


app = dash.Dash(__name__)




# --------------------------------- Tab stylization------------------------------------------------#
app = dash.Dash(external_stylesheets=[dbc.themes.BOOTSTRAP])

tabs_styles = {
    'height': '44px',
    
}
tab_style = {
    #'borderBottom': '1px solid #d6d6d6',
    #'padding': '16px',
    'fontWeight': 'bold',
    'background-color': 'f1f1f1',

    'float': 'left'
            
}

tab_selected_style = {
    'borderTop': '1px solid #d6d6d6',
    'borderBottom': '1px solid #d6d6d6',
    'backgroundColor': '#119DFF',
    'color': 'white',
    'padding': '16px'
}
CONTENT_STYLE = {
    "margin-left": "4rem",
    "margin-right": "4rem",
    "padding": "4rem 1rem",
}
SIDEBAR_STYLE = {
    "position": "fixed",
    "top": 0,
    "left": 0,
    "bottom": 100,
    "width": "16rem",
    "padding": "2rem 1rem",
    "background-color": "#f8f9fa",
    'color' : 'dark'
}

sidebar = html.Div(
    [

        html.P(
            className="lead"
        ),
        dbc.Nav(
            [
                dbc.NavLink("Summary", href="/", active="exact"),
                dbc.NavLink("Graph", href="/graph", active="exact"),
                dbc.NavLink("Export", href="/export", active="exact"),
                dbc.NavLink("4-Wire AC", href="/4wire", active="exact"),
            ],
            vertical=True,
            pills=True,
        ),
    ],
    style=SIDEBAR_STYLE,
)

content = html.Div(id="page-content", style=CONTENT_STYLE)

# error says it can not fund thi ID need to figure out where to place it elsewhere
##app.layout = html.Div([dcc.Location(id="url"), sidebar, content])


@app.callback(Output("page-content", "children"), [Input("url", "pathname")])
def render_page_content(pathname):
    if pathname == "/":
        return html.P("This is the content of the home page!")
    elif pathname == "/graph":
        return html.P("This is the content of page 1. Yay!")
    elif pathname == "/export":
        return html.P("Oh cool, this is page 2!")
    elif pathname == "/4wire":
        return html.P("4-wire AC page and information")
    # If the user tries to reach a different page, return a 404 message
    return 




# going to try and see if this will work better as a way to add new values
# to the traces. 
# this will take in values that can then be used by the 


#-------------------------- app Layout--------------------------------------------------------------------#

app.layout = html.Div([

    html.Div(id='tabs-content'),
    dcc.Store(id='session', storage_type='session'),
    dcc.Location(id="url"), sidebar, content,

    # this creates a list of options

    dcc.Tabs(

        id = "tabs-with-classes",
        value = 'tb',

        children = [
                dcc.Tab (
                
                label = 'Summary',
                value = 't',
               # tab_id = 't1',
                style = tab_style, 
                selected_style = tab_selected_style,
                className = 'custom-tab',
                selected_className = 'custom-tab-selected',
                children = [

                    #currently working on adding input range feature to change the amount of data can be viewed

                    dcc.Input(id="realRange", type="number", placeholder=max, style={'marginRight':'10px'}),
                    html.Div(id="out"),

                    dcc.RadioItems(   options=[
                    {'label': 'Graph A', 'value': 'A'},
                    {'label': 'Graph B', 'value': 'B'},
                    {'label': 'Graph C', 'value': 'C'},
                    {'label': 'Graph D', 'value': 'D'},
                    ],
                    value = 'A' ),


                ]


            ),


# the tab bellow will hold the information for the graphs
            dcc.Tab (
                
                label = 'Graphs',
                value = 't1',
               # tab_id = 't1',
                style = tab_style, 
                selected_style = tab_selected_style,
                className = 'custom-tab',
                selected_className = 'custom-tab-selected',
                children = [

                    #current/y working on adding input range feature to change the amount of data can be viewed

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
                html.Div(id='4WireAC-Card'),

                

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

                                dcc.Tab (
                
                label = 'export',
                value = 't4',
               # tab_id = 't1',
                style = tab_style, 
                selected_style = tab_selected_style,
                className = 'custom-tab',
                selected_className = 'custom-tab-selected',
                children = [

                    #currently working on adding input range feature to change the amount of data can be viewed



                ]


            ),




                #]

            
        ]
    ),
    html.Button('test for Hiding tabs', id="hide_1_button"),
    html.Div(id='tabs-content-classes'),

    html.Div([        


#---------------------- two Graph ------------------------------
        dcc.Graph( 
            id='live-graph2',
            #animate = True
            
        ),

        dcc.Interval(
            id='graph-update2',
            interval= 1*10000,
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
    global randomRange
    if (tab == 't1'):
        randomRange = 5
    if (tab == 't2'):
        randomRange = 68

@app.callback(
    [Output('realRange', 'children')],
    Input('out', 'value'),
)
def update_output(value):
    max = value
    return max
  

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

    #X.append(X[-1]+1)
   # y = randint(0,4)
    
    #random y values to test that the graphs can update live.
    # xx Y appendded values
    Y.append(random.uniform(0,randomRange))
    Y2.append(random.uniform(0,randomRange))
    Y3.append(random.uniform(0,randomRange))
    Y4.append(random.uniform(0,randomRange))

 
   
   
# this is currently not in use but may be useful for organizaing data with API

    # data = {

    #     'time' : [],
    #     'full': [],
    #     'vis': [],
    #     'lux': [],
    #     'inf': []

    #  if you want to use the dictionarry- use Capital 'M'
    
    # xx DateTime Now -------------------
    now = datetime.now()
    timee.append(now)
    
    
    #-------------------------------------------------------------------------------------------

    #xx plot margins
    fig = make_subplots(rows=2, cols=2, vertical_spacing=0.1, shared_xaxes=True,

    subplot_titles=("Graph A","Graph B", "Graph C", "Grapb D")
    
    
    )
    
    fig['layout']['margin'] = {
        'l': 50, 'r': 20, 'b': 40, 't': 30
    }
    fig['layout']['legend'] = {'x': 1.5, 'y': 1, 'xanchor': 'right'}
    #timee.append(timee[datetime.now()]-datetime.now()
    

# xx scatter lists
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


# xx scatter traces
    fig.add_trace(
        go.Scatter(
        t,
        legendgroup = '1'
   


        ), 1, 1
    )
    fig.add_trace(
    go.Scatter(
        t2,
        legendgroup = '1'
    

    ), 1, 2

    )

    fig.add_trace(
    go.Scatter(
        t3,
        legendgroup = '2'
        

    ), 2, 1
    )
    fig.add_trace(

    go.Scatter(
        t,
        legendgroup = '3'
        

    ), 2, 2
    )

    #fig.update_xaxes( autorange = True)
    fig.update_layout(template = 'plotly_dark', autosize= True, ) # xaxis=dict(range=[min(X),max(X)]))
    
    
    #fig.update_xaxes(range=list(timee))
  #  global ch

   # ch = numpy.array([numpy.array(["Time ", "Temperature"])], [numpy.array([str(timee), str(Y)])])


#    #################################### global df 
#     df = pd.DataFrame({"Card data 1": Y,  "Time": timee })


    return fig


# xx Cvs callback and function
@app.callback(
    Output("download-dataframe-csv", "data"),
    Input("btn_csv", "n_clicks"),
    prevent_initial_call=True,
)
def func(n_clicks):

    #file = numpy.savetxt("4AC-ch1.txt", ch, fmt='%s')
    #a = open("4AC-ch1.txt", 'r')
   # print(a.read())
    #return dcc.send_file(file, '4AC-ch1.txt')
   
    return #dict(content =file, filename="data.txt")
   # return dcc.send_data_frame(df.to_csv, "mydf.csv")


################ error





# Main Call back

if __name__ == '__main__':
    app.run_server(debug= True , dev_tools_hot_reload = False) 
