Processor 
Type SynthChain ID bass_tightener Bypassed Gain 	      �?Balance 	        VoiceLimit 	      P@KillFadeTime 	      4@IconColour 0 packageName  BuildVersion �  EditorStates BodyShown 0 Visible 1 Solo 0 Folded 1  ChildProcessors  Processor Type MidiProcessorChain ID Midi Processor Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor Type ScriptProcessor ID Interface Bypassed Script �Content.makeFrontInterface(600, 430);

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
  EditorStates BodyShown 1 Visible 1 Solo 0 contentShown 1 onInitOpen 1  ChildProcessors   Content  Control type ScriptSlider id Tightness value 	   @334@ Control type ScriptSlider id Gain value 	    �G@ Control type ScriptSlider id CutOff value 	     �`@ Control type ScriptSlider id 	Release value 	   ��]@ UIData  ContentProperties DeviceType 	Desktop Component type ScriptPanel id MainPanel x 	        y 	        width 	     ��@height 	     �z@bgColour 	1514537 itemColour 4278387459 itemColour2 4278387459 textColour 4281019179 locked 1 Component type ScriptLabel id 	Version x 	      |@y 	      $@text V1.0 alignment right fontStyle Bold parentComponent MainPanel fontName Avenir Next fontSize 	      0@textColour 4280339679  Component type ScriptLabel id 	Company x 	     �z@y 	      >@alignment right text © Mannix Squared, 2024 textColour 4292926663 parentComponent MainPanel fontName Avenir Next fontSize 	      0@width 	     �b@ Component type ScriptLabel id Title x 	     �S@y 	      3@width 	     �f@height 	      D@parentComponent MainPanel editable 0 text TIGHT LOW END fontName Avenir Next fontStyle 	Regular fontSize 	      :@alignment left textColour 4292926663  Component type ScriptImage id Logo x 	      4@y 	      $@width 	     �N@height 	      N@parentComponent MainPanel fileName {PROJECT_FOLDER}logo.png  Component type ScriptSlider id Tightness x 	      =@y 	     pu@middlePosition 	      2@stepSize 	   ����?max 	      Y@suffix  dB isPluginParameter 1 height 	      P@bgColour 0 itemColour 0 itemColour2 0 width 	      P@numStrips 	      P@filmstripImage !{PROJECT_FOLDER}subtle_knob.png mode 	Decibel showTextBox  Component type ScriptLabel id TightnessLabel x 	     @V@y 	     0v@text TIGHTNESS fontStyle Bold alignment right width 	     �R@height 	      4@textColour 4280339679 fontName Avenir Next fontSize 	      ,@editable 0  Component type ScriptLabel id TightnessValue x 	     �W@y 	      w@alignment right text 20.20 width 	     �Q@height 	      4@textColour 4292926663 itemColour2 0 editable 0 fontName Avenir Next fontSize 	      ,@ Component 	type ScriptPanel id Panel1 x 	     �r@y 	      Y@width 	     �p@height 	     �k@borderRadius 	        borderSize 	        bufferToImage 1  Component type ScriptSlider id Gain x 	     �s@y 	     Pu@middlePosition 	      #@max 	      2@suffix  dB isPluginParameter 1 height 	      P@bgColour 0 itemColour 0 itemColour2 0 width 	      P@numStrips 	      P@filmstripImage !{PROJECT_FOLDER}subtle_knob.png mode 	Decibel min 	      �?defaultValue 	      �?showTextBox  Component type ScriptLabel id GainLabel x 	     0v@y 	     v@text BOOST fontStyle Bold alignment right width 	     �R@height 	      4@textColour 4280339679 fontName Avenir Next fontSize 	      ,@editable 0  Component type ScriptLabel id GainValue x 	     �v@y 	      w@alignment right text 6.6 dB width 	     �Q@height 	      4@textColour 4292926663 itemColour2 0 editable 0 fontName Avenir Next fontSize 	      ,@ Component type ScriptSlider id CutOff x 	     �e@y 	     Pu@middlePosition 	     �V@stepSize 	   ����?max 	     @@suffix  dB isPluginParameter 1 mode Frequency height 	      P@bgColour 0 itemColour 0 itemColour2 0 width 	      P@numStrips 	      P@filmstripImage !{PROJECT_FOLDER}subtle_knob.png text 	Cut Off defaultValue 	     �V@min 	      4@showTextBox  Component type ScriptLabel id CutoffLabel x 	     �k@y 	     v@text 	CUT OFF fontStyle Bold alignment right width 	     �R@height 	      4@textColour 4280339679 fontName Avenir Next fontSize 	      ,@editable 0  Component type ScriptLabel id CutoffValue x 	     `l@y 	      w@alignment right text 
133.5 Hz width 	     �Q@height 	      4@textColour 4292926663 itemColour2 0 editable 0 fontName Avenir Next fontSize 	      ,@ Component type ScriptSlider id 	Release x 	     }@y 	     Pu@middlePosition 	     �b@max 	     �r@suffix ms isPluginParameter 1 height 	      P@bgColour 0 itemColour 0 itemColour2 0 width 	      P@numStrips 	      P@filmstripImage !{PROJECT_FOLDER}subtle_knob.png mode Time defaultValue 	      I@showTextBox  Component type ScriptLabel id ReleaseLabel x 	     �@y 	     v@text 	RELEASE fontStyle Bold alignment right width 	     �R@height 	      4@textColour 4280339679 fontName Avenir Next fontSize 	      ,@editable 0  Component type ScriptLabel id ReleaseValue x 	     8�@y 	      w@alignment right text 
116.0 ms width 	     �Q@height 	      4@textColour 4292926663 itemColour2 0 editable 0 fontName Avenir Next fontSize 	      ,@ Component type ScriptFloatingTile id FloatingTile1 x 	      >@y 	      Y@width 	     �k@height 	     �k@ContentType AudioAnalyser bgColour 0 itemColour 	�� �    itemColour2 4282729797 textColour 0 Data L{
  "ProcessorId": "Analyser1",
  "Index": 0,
  "FollowWorkspace": false
}  Processor Type ModulatorChain ID GainModulation Bypassed Intensity 	      �?EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type ModulatorChain ID PitchModulation Bypassed Intensity 	        EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type EffectChain ID FX Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor Type 
ScriptFX ID 	TightFX Bypassed NumDisplayBuffers    Script � function prepareToPlay(sampleRate, blockSize)
{
	
}
 function processBlock(channels)
{
	
}
 function onControl(number, value)
{
	
}
  EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Content   Networks  Network ID bass_tightener Version 0.0.0  UIData  ContentProperties DeviceType 	Desktop  Processor Type 
Analyser ID Analyser1 Bypassed BufferSize 	      �@PreviewType 	       @EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Processor Type AudioLooper ID Audio Loop Player1 Bypassed Gain 	   @���?Balance 	        VoiceLimit 	      p@KillFadeTime 	      4@IconColour 0 SyncMode 	      �?PitchTracking 	        LoopEnabled 	      �?RootNote 	      P@SampleStartMod 	        Reversed 	        FileName c/Users/johnmannix/Library/Mobile Documents/com~apple~CloudDocs/Music/A Mugshot in your Eyes 2.wav min     max     loopStart     loopEnd     EditorStates BodyShown 1 Visible 1 Solo 0 Folded 1  ChildProcessors  Processor Type MidiProcessorChain ID Midi Processor Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type ModulatorChain ID GainModulation Bypassed Intensity 	      �?EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor 	Type SimpleEnvelope ID DefaultEnvelope1 Bypassed Monophonic 	        Retrigger 	      �?Intensity 	      �?Attack 	      @Release 	      $@LinearMode 	      �?EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors  Processor Type ModulatorChain ID Attack Time Modulation Bypassed Intensity 	      �?EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type ModulatorChain ID PitchModulation Bypassed Intensity 	        EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type EffectChain ID FX Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� macro_controls  macro name 	Macro 1 value 	        midi_cc ���� macro name 	Macro 2 value 	        midi_cc ���� macro name 	Macro 3 value 	        midi_cc ���� macro name 	Macro 4 value 	        midi_cc ���� macro name 	Macro 5 value 	        midi_cc ���� macro name 	Macro 6 value 	        midi_cc ���� macro name 	Macro 7 value 	        midi_cc ���� macro name 	Macro 8 value 	        midi_cc ���� MidiAutomation   MPEData Enabled  