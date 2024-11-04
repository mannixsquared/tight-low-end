const var laf = Engine.createGlobalScriptLookAndFeel();

laf.loadImage("{PROJECT_FOLDER}subtle_knob.png", "Knob");

laf.registerFunction("drawToggleButton", function(g, obj)
{
    if (obj.value) {
        g.setColour(0xFF20CCDF);
    } else {
        g.setColour(Colours.withAlpha(0xFFE0DCC7, 0.3));
    }
        
    g.fillEllipse([10, obj.area[3]/2-6, 10, 10]);
        
    g.setColour(Colours.withAlpha(0xFFE0DCC7, obj.value ? 1.0 : 0.3));
    g.setFont("Arial Bold", 12.0);
    g.drawAlignedText(obj.text, obj.area, "centred");
});

laf.registerFunction("drawRotarySlider", function(g, obj)
{
	var frame_offset = Math.round(63 * obj.valueNormalized);
	g.drawImage("Knob", [0, 0, 64, 64], 0, frame_offset * 64);
		
	g.setColour(0xFF20CCDF);
	g.setFont("Avenir Next Bold", 14);
	g.drawAlignedText(obj.text.toUpperCase(), [73, 14, obj.area[2] - 73, 14], "topRight");
	
	g.setColour(0xFFE0DCC7);
	g.setFont("Avenir Next", 14);
	var v = Engine.doubleToString(obj.value, 1) + obj.suffix;
	g.drawAlignedText(v, [73, 28, obj.area[2] - 73, 14], "topRight");
});