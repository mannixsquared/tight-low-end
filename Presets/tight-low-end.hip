Processor 
Type SynthChain ID tight-low-end Bypassed Gain 	      �?Balance 	        VoiceLimit 	      P@KillFadeTime 	      4@IconColour 0 packageName  BuildVersion �  EditorStates BodyShown 0 Visible 1 Solo 0 Folded 1  ChildProcessors  Processor Type MidiProcessorChain ID Midi Processor Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor Type ScriptProcessor ID Interface Bypassed NumDisplayBuffers    Script �include("LookAndFeel.js");

Content.makeFrontInterface(720, 430);

const TightFX = Synth.getEffect("TightFX");
const var Panel1 = Content.getComponent("Panel1");
const var LowCut = Synth.getEffect("LowCut");

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

inline function onButton2Control(component, value)
{
	TightFX.setAttribute(6, value ? 0.0 : 1.0);
	LowCut.setBypassed(value);
};

Content.getComponent("Button2").setControlCallback(onButton2Control);

const var MainPanel = Content.getComponent("MainPanel");
MainPanel.setLocalLookAndFeel(laf);function onNoteOn()
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
  EditorStates BodyShown 1 Visible 1 Solo 0 contentShown 1 onInitOpen 1  ChildProcessors   Content  Control type ScriptSlider id Tightness value 	   `ffV@ Control type ScriptSlider id Gain value 	   @33$@ Control type ScriptSlider id CutOff value 	     �V@ Control type ScriptSlider id 	Release value 	   �=�T@ Control type ScriptSlider id Harmonics value 	   �Q��? Control type ScriptButton id 	Button1 value      Control type ScriptButton id 	Button2 value 	         UIData  ContentProperties DeviceType 	Desktop Component type ScriptPanel id MainPanel x 	        y 	        width 	     ��@height 	     �z@bgColour 	1514537 itemColour 4278387459 itemColour2 4278387459 textColour 4281019179 locked 1 borderSize 	        borderRadius 	        Component type ScriptLabel id 	Version x 	      �@y 	      $@text V1.0 alignment right fontStyle Bold parentComponent MainPanel fontName Avenir Next fontSize 	      0@textColour 4280339679 editable 0  Component type ScriptLabel id 	Company x 	     ��@y 	      >@alignment right text © Mannix Squared, 2024 textColour 4292926663 parentComponent MainPanel fontName Avenir Next fontSize 	      0@width 	     �b@editable 0  Component type ScriptLabel id Title x 	     �S@y 	      3@width 	     �f@height 	      D@parentComponent MainPanel editable 0 text TIGHT LOW END fontName Avenir Next fontStyle 	Regular fontSize 	      :@alignment left textColour 4292926663  Component type ScriptImage id Logo x 	      4@y 	      $@width 	     �N@height 	      N@parentComponent MainPanel fileName {PROJECT_FOLDER}logo.png  Component type ScriptSlider id Tightness x 	      =@y 	     pu@middlePosition 	     �T@stepSize 	   ����?max 	      Y@height 	      P@bgColour 0 itemColour 0 itemColour2 0 width 	     @`@numStrips 	      P@showTextBox 0 processorId 	TightFX parameterId Tightness pluginParameterName Tightness isPluginParameter 1 isMetaParameter 1 parentComponent MainPanel  Component 
type ScriptPanel id Panel1 x 	     �r@y 	      Y@width 	      y@height 	     �k@borderRadius 	        borderSize 	        bufferToImage 1 parentComponent MainPanel  Component type ScriptSlider id Gain x 	     `s@y 	     @u@middlePosition 	      I@isPluginParameter 1 height 	      P@bgColour 0 itemColour 0 itemColour2 0 width 	      Y@numStrips 	      P@showTextBox 0 processorId 	TightFX parameterId Gain pluginParameterName Gain isMetaParameter 1 parentComponent MainPanel max 	      Y@stepSize 	   ����? Component type ScriptSlider id CutOff x 	     �e@y 	     Pu@middlePosition 	     �V@stepSize 	   ����?max 	     @@suffix Hz isPluginParameter 1 mode Frequency height 	      P@bgColour 0 itemColour 0 itemColour2 0 width 	      ^@numStrips 	      P@text 	Cut Off defaultValue 	     �V@min 	      4@showTextBox 0 processorId 	TightFX parameterId CutOff pluginParameterName 	Cut Off parentComponent MainPanel isMetaParameter 1  Component type ScriptSlider id 	Release x 	     �z@y 	     @u@middlePosition 	     �b@max 	     �r@suffix ms isPluginParameter 1 height 	      P@bgColour 0 itemColour 0 itemColour2 0 width 	      ^@numStrips 	      P@mode Time defaultValue 	      I@showTextBox 0 processorId 	TightFX parameterId 	Release pluginParameterName 	Release isMetaParameter 1 parentComponent MainPanel  Component type ScriptFloatingTile id FloatingTile1 x 	      >@y 	      Y@width 	     �k@height 	     �k@ContentType AudioAnalyser bgColour 0 itemColour 4280339679 itemColour2 4282729797 textColour 0 Data L{
  "ProcessorId": "Analyser1",
  "Index": 0,
  "FollowWorkspace": false
} parentComponent MainPanel  Component type ScriptSlider id Harmonics x 	     ��@y 	     @u@middlePosition 	      �?isPluginParameter 1 height 	      N@bgColour 0 itemColour 0 itemColour2 0 width 	     �a@numStrips 	      P@showTextBox 0 processorId 	TightFX parameterId Harmonics pluginParameterName Harmonics isMetaParameter 1 parentComponent MainPanel  Component 
type ScriptButton id 	Button1 x 	     @o@y 	      8@text SOLO width 	     @V@height 	      >@processorId 	TightFX parameterId SoloLowEnd parentComponent MainPanel  Component type ScriptButton id 	Button2 x 	     �u@y 	      8@text BYPASS width 	      Y@height 	      >@parentComponent MainPanel  Processor Type ModulatorChain ID GainModulation Bypassed Intensity 	      �?EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type ModulatorChain ID PitchModulation Bypassed Intensity 	        EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type EffectChain ID FX Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor Type 	CurveEq ID LowCut Bypassed NumFilters    Band0 	        Band1 	      >@Band2 	      �?Band3 	      �?Band4 	      �?Band5 	        Band6 	      >@Band7 	      �?Band8 	      �?Band9 	      �?Band10 	        Band11 	      >@Band12 	      �?Band13 	      �?Band14 	      �?Band15 	   @����Band16 	   ��~c@Band17 	   ��@Band18 	      �?Band19 	      @Band20 	    �6@Band21 	    �mq@Band22 	   �p=@Band23 	      �?Band24 	      @FFTEnabled EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Processor Type Hardcoded Master FX ID 	TightFX Bypassed Network bass_tightener CutOff 	     �V@Tightness 	   `ffV@Gain 	   @33$@Release 	   �=�T@Harmonics 	   �Q��?SoloLowEnd 	        Enabled 	      �?EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Processor Type 
ScriptFX ID Script FX1 Bypassed NumDisplayBuffers    Script � function prepareToPlay(sampleRate, blockSize)
{
	
}
 function processBlock(channels)
{
	
}
 function onControl(number, value)
{
	
}
  EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Content   Networks  Network ID bass_tightener Version 0.0.0 AllowCompilation 1  UIData  ContentProperties DeviceType 	Desktop  Processor Type 
Analyser ID Analyser1 Bypassed BufferSize 	      �@PreviewType 	       @EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� Processor Type AudioLooper ID Audio Loop Player1 Bypassed Gain 	   @���?Balance 	        VoiceLimit 	      p@KillFadeTime 	      4@IconColour 0 SyncMode 	      �?PitchTracking 	        LoopEnabled 	      �?RootNote 	      P@SampleStartMod 	        Reversed 	        FileName Z/Volumes/External SSD/Reaper Projects/Sketches/Tight Low End Demo/Tight Low End Demo.mp3 min     max n� loopStart     loopEnd n� EditorStates BodyShown 1 Visible 1 Solo 0 Folded 1  ChildProcessors  Processor Type MidiProcessorChain ID Midi Processor Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type ModulatorChain ID GainModulation Bypassed Intensity 	      �?EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors  Processor 	Type SimpleEnvelope ID DefaultEnvelope1 Bypassed Monophonic 	        Retrigger 	      �?Intensity 	      �?Attack 	      @Release 	      $@LinearMode 	      �?EditorStates BodyShown 1 Visible 1 Solo 0  ChildProcessors  Processor Type ModulatorChain ID Attack Time Modulation Bypassed Intensity 	      �?EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type ModulatorChain ID PitchModulation Bypassed Intensity 	        EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   Processor Type EffectChain ID FX Bypassed EditorStates BodyShown 1 Visible 0 Solo 0  ChildProcessors   RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� RoutingMatrix NumSourceChannels    Channel0     Send0 ����Channel1    Send1 ���� macro_controls  macro name 	Macro 1 value 	        midi_cc ���� macro name 	Macro 2 value 	        midi_cc ���� macro name 	Macro 3 value 	        midi_cc ���� macro name 	Macro 4 value 	        midi_cc ���� macro name 	Macro 5 value 	        midi_cc ���� macro name 	Macro 6 value 	        midi_cc ���� macro name 	Macro 7 value 	        midi_cc ���� macro name 	Macro 8 value 	        midi_cc ���� MidiAutomation   MPEData Enabled  