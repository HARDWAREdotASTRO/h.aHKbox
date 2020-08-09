I had a lot of trouble with exporting from CircuitMaker, but eventually I figured it out!

I strongly recommend you use the "Outputs" tab while editing a circuitboard file, rather than "Project -> Generate Outputs" as the latter option generates all outputs for every board in the project which can get confusing.

Issues I ran into:
1. "Board Cutout" and "Board Shape" have nothing to do with the actual board shape. They are just there so you know where you can put your components. To tell the manufacturer the shape of the board you need to use lines to draw the board outline on the "Keep-out Layer" Layer. 
	-> If there is no pre-existing keepout layer, you can open the "View Configurations" Layer window by clicking on the small colored rectangle to the left of the layer tabs at the bottom of the circuit board window. This should list all of the layers and their colors. From here you want to look at the Mechanical Layers(M) table on the top right. Uncheck the "Only show enabled mechanical Layers" box if it is checked. Enable one of the unused Mechanical Layers using the "Enable" check box. Slowly click twice on the name of the layer so you're able to edit the name. Rename it to "Outline". Check the "Show" box for this layer, and then click "OK" at the bottom right of the window. Now you should have the "Outline" Layer listed in the tabs at the bottom, and you can draw your board outline.

2. Wonky Top Overlay drawings resulted in the Gerber Viewer when I uploaded the files wihtout any explanation. I had followed the same process that worked not 20 minutes prior to export my files, so I had no idea what caused it. After a lot of creative googling I found one forumn post where someone suggested use "Relative Origin" instead of "Absolute Origin" in the file output configurations. This did indeed fix my issue, but you NEED to make sure you use relative origin for both the gerber output and the nc drill output. If they arn't the same, the holes may end up offset!




OUTPUT STEPS:
1. Save your files and commit them.
2. With your board file open, goto the "Outputs" tab and click the "Gerber" button.
		- if you are prompted to save, click yes, click ok
3. In the pop up you can change your setting as needed. The mains ones I messed with:
		General -> Units = millimeters
		General -> Format = 4:3
		Layers -> All Layers -> Plot = checked (everything else unchecked
					-----> if you used a mechanical layer instead of the "Keep-out layer", make sure its "Plot" checkbox is checked in the right side of this layer tab. Otherwise it won't be part of the outputs.
		Drill Drawings -> Plot all used layer pairs = both boxes checked
		Apertures -> Embedded apertures (RS274X) = checked
		Advanced -> Position on File = Reference to relative origin
		Everything else left default
4. Click OK and save to desired location

5. In the Outputs tab, click "NC Drill Files" and save/commit if prompted.
6. Settings I chose are as follows
		Units = Millimeters (same as gerber)
		Format = 4:3 (same as gerber)
		Coordinate Positions = Reference to relative origin (same as gerber)
		Everything else left default
7. Click OK and save to desired location (preferably near gerber output)
8. Next, the gerber and NC Drill files need to be combined into one zip file. 
	- Using your desired program, Extract the contents of both the gerber output and the nc drill output.
	- Create a new zip file and add both outputs to it.
		- You will be prompted to overwrite the StatusReport.txt, to which you can say yes.
9. The resulting zip is the file you will need to upload to the manufacturer.
10. I suggest you use an online Gerber Viewer to make sure the files load in the way you expect them to. 
		https://www.pcbway.com/project/OnlineGerberViewer.html   is what I used as we were ording from PCBWay anyway. If you're ordering from a different manufacturer and they have a their own viewer, I suggest you use theirs as it will likely be more accurate to their process.

If you have any questions you can contact me at adam.stammer@go.winona.edu or adam.r.stammer@gmail.com