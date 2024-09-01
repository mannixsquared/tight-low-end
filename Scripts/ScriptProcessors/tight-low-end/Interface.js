Content.makeFrontInterface(600, 430);

Content.getComponent("Tightness").setControlCallback(onTightnessControl);
Content.getComponent("CutOff").setControlCallback(onCutoffControl);
Content.getComponent("Gain").setControlCallback(onGainControl);
Content.getComponent("Release").setControlCallback(onReleaseControl);

const TightFX = Synth.getEffect("TightFX");
const TightnessValue = Content.getComponent("TightnessValue");
const CutOffValue = Content.getComponent("CutoffValue");
const GainValue = Content.getComponent("GainValue");
const ReleaseValue = Content.getComponent("ReleaseValue");
const var Panel1 = Content.getComponent("Panel1");

const dbsource = Synth.getDisplayBufferSource("TightFX");

inline function onTightnessControl(component, value)
{
	local v = Engine.doubleToString(value, 2);
	TightFX.setAttribute(1, -value);
	TightnessValue.set("text", v);
};


inline function onCutoffControl(component, value)
{
	local v = Engine.doubleToString(value, 1) + " Hz";
	TightFX.setAttribute(0, value);
	CutOffValue.set("text", v);
};

inline function onGainControl(component, value)
{
	local v = Engine.doubleToString(value, 1) + " dB";
	TightFX.setAttribute(2, value);
	GainValue.set("text", v);
};


inline function onReleaseControl(component, value)
{
	local v = Engine.doubleToString(value, 1) + " ms";
	TightFX.setAttribute(3, value);
	ReleaseValue.set("text", v);
};

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
 