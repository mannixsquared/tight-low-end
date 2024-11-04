include("LookAndFeel.js");

Content.makeFrontInterface(720, 430);

const TightFX = Synth.getEffect("TightFX");
const var Panel1 = Content.getComponent("Panel1");

const dbsource = Synth.getDisplayBufferSource("TightFX");

Panel1.setPaintRoutine(function(g)
{
	var w = this.getWidth();
	var h = this.getHeight();

	var d = dbsource.getDisplayBuffer(0);
	var path = d.createPath([0, h, w, -h],
                 		    [0, 1, 0, -1],
                 			0.0);
    
	g.setColour(Colours.lightgrey);
	g.drawLine(30, w, 0, 0, 1);
	
    for (i = 50; i < h; i += 50)
    {
		g.setColour(Colours.darkgrey);
		g.drawLine(30, w, i, i, 0.5);           			    
    }  			
                 			
	g.setColour(Colours.fromVec4([0.125, 0.8, 0.875, 0.5]));
	g.fillPath(path, [30, 0, w-30, h]);
	
	g.setColour(Colours.grey);
	g.drawLine(0, 0, 20, h-20, 1);
});

Panel1.setTimerCallback(function()
{
	this.repaintImmediately(); 
});

Panel1.startTimer(1000 / 30);
function onNoteOn()
{
	
}
 function onNoteOff()
{
	
}
 function onController()
{
	
}
 function onTimer()
{
	
}
 function onControl(number, value)
{
	
}
 