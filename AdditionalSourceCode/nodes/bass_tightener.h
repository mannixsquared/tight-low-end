#pragma once

// These will improve the readability of the connection definition

#define getT(Idx) template get<Idx>()
#define connectT(Idx, target) template connect<Idx>(target)
#define getParameterT(Idx) template getParameter<Idx>()
#define setParameterT(Idx, value) template setParameter<Idx>(value)
#define setParameterWT(Idx, value) template setWrapParameter<Idx>(value)
using namespace scriptnode;
using namespace snex;
using namespace snex::Types;

namespace bass_tightener_impl
{
// ==============================| Node & Parameter type declarations |==============================

template <int NumVoices> struct Saturation
{
	SNEX_NODE(Saturation);
	float drive = 1.0f;
	float chebyshev(int n, float x) {
		if (n == 0) {
				return 1.0f;
			} else if (n == 1) {
			return x;
			} else {
			float Tn_2 = 1.0f; 	// T_0(x)
			float Tn_1 = x; 	// T_1(x)
			float Tn = 0.0f;
			for (int i = 2; i <= n; i++) {
					Tn = 2.0f * x * Tn_1 - Tn_2;
				Tn_2 = Tn_1;
				Tn_1 = Tn;
			}
			return Tn;
		}
	}
	// Implement the Waveshaper here...
	float getSample(float x)
	{
		return (1.0f - drive) * x + drive * Math.tanh((1.0f + drive) * x);
	}
	// These functions are the glue code that call the function above
	template <typename T> void process(T& data)
	{
		for(auto ch: data)
		{
			for(auto& s: data.toChannelData(ch))
			{
				s = getSample(s);
			}
		}
	}
	template <typename T> void processFrame(T& data)
	{
		for(auto& s: data)
			s = getSample(s);
	}
	void reset()
	{
	}
	void prepare(PrepareSpecs ps)
	{
	}
	void setExternalData(const ExternalData& d, int index)
	{
	}
	template <int P> void setParameter(double v)
	{
		drive = (float) v;
	}
};

template <int NV>
using snex_shaper_t = wrap::no_data<core::snex_shaper<Saturation<NV>>>;
template <int NV>
using smoothed_parameter_t = wrap::mod<parameter::plain<snex_shaper_t<NV>, 0>, 
                                       control::smoothed_parameter<NV, smoothers::linear_ramp<NV>>>;

template <int NV>
using pma_unscaled_t = control::pma_unscaled<NV, 
                                             parameter::plain<filters::svf_eq<NV>, 2>>;
template <int NV>
using comp_t = wrap::mod<parameter::plain<pma_unscaled_t<NV>, 0>, 
                         wrap::data<dynamics::comp, data::external::displaybuffer<0>>>;
template <int NV> struct GainControl
{
	SNEX_NODE(GainControl);
	// Initialise the processing specs here
	void prepare(PrepareSpecs ps)
	{
	}
	// Reset the processing pipeline here
	void reset()
	{
	}
	// Process the signal here
	template <typename ProcessDataType> void process(ProcessDataType& data)
	{
		// Create a dyn reference to the left channel
		auto l = data[0];
		auto r = data[1];
		// Convert to mono
		l += r;
		l *= 0.5f;
		r = l;
	}
	// Process the signal as frame here
	template <int C> void processFrame(span<float, C>& data)
	{
	}
	// Process the MIDI events here
	void handleHiseEvent(HiseEvent& e)
	{
	}
	// Use this function to setup the external data
	void setExternalData(const ExternalData& d, int index)
	{
	}
	// Set the parameters here
	template <int P> void setParameter(double v)
	{
	}
};

template <int NV>
using soft_bypass_t_ = container::chain<parameter::empty, 
                                        wrap::fix<2, comp_t<NV>>, 
                                        snex_shaper_t<NV>, 
                                        GainControl<NV>>;

template <int NV>
using soft_bypass_t = bypass::smoothed<20, soft_bypass_t_<NV>>;

template <int NV>
using band1_t = container::chain<parameter::empty, 
                                 wrap::fix<2, jdsp::jlinkwitzriley>, 
                                 soft_bypass_t<NV>>;

template <int NV>
using band2_t = container::chain<parameter::empty, 
                                 wrap::fix<2, jdsp::jlinkwitzriley>, 
                                 core::gain<NV>>;

template <int NV>
using wrapband21_t = container::chain<parameter::empty, 
                                      wrap::fix<2, band2_t<NV>>>;

namespace freq_split3_t_parameters
{

using Band1 = parameter::chain<ranges::Identity, 
                               parameter::plain<jdsp::jlinkwitzriley, 0>, 
                               parameter::plain<jdsp::jlinkwitzriley, 0>>;

}

template <int NV>
using freq_split3_t = container::split<freq_split3_t_parameters::Band1, 
                                       wrap::fix<2, band1_t<NV>>, 
                                       wrapband21_t<NV>>;

namespace bass_tightener_t_parameters
{
// Parameter list for bass_tightener_impl::bass_tightener_t ----------------------------------------

template <int NV>
using CutOff = parameter::chain<ranges::Identity, 
                                parameter::plain<bass_tightener_impl::freq_split3_t<NV>, 0>, 
                                parameter::plain<filters::svf_eq<NV>, 0>>;

DECLARE_PARAMETER_RANGE_STEP(Tightness_InputRange, 
                             0., 
                             100., 
                             0.1);
DECLARE_PARAMETER_RANGE_SKEW_INV(Tightness_0Range, 
                                 -100., 
                                 0., 
                                 5.42227);

template <int NV>
using Tightness_0 = parameter::from0To1_inv<bass_tightener_impl::comp_t<NV>, 
                                            0, 
                                            Tightness_0Range>;

template <int NV>
using Tightness = parameter::chain<Tightness_InputRange, Tightness_0<NV>>;

template <int NV>
using SoloLowEnd = parameter::from0To1_inv<core::gain<NV>, 
                                           0, 
                                           Tightness_0Range>;

template <int NV>
using Gain = parameter::plain<bass_tightener_impl::pma_unscaled_t<NV>, 
                              1>;
template <int NV>
using Release = parameter::plain<bass_tightener_impl::comp_t<NV>, 
                                 2>;
template <int NV>
using Harmonics = parameter::plain<bass_tightener_impl::smoothed_parameter_t<NV>, 
                                   0>;
template <int NV>
using bass_tightener_t_plist = parameter::list<CutOff<NV>, 
                                               Tightness<NV>, 
                                               Gain<NV>, 
                                               Release<NV>, 
                                               Harmonics<NV>, 
                                               SoloLowEnd<NV>>;
}

template <int NV>
using bass_tightener_t_ = container::chain<bass_tightener_t_parameters::bass_tightener_t_plist<NV>, 
                                           wrap::fix<2, smoothed_parameter_t<NV>>, 
                                           filters::one_pole<NV>, 
                                           filters::one_pole<NV>, 
                                           freq_split3_t<NV>, 
                                           pma_unscaled_t<NV>, 
                                           filters::svf_eq<NV>>;

// =================================| Root node initialiser class |=================================

template <int NV> struct instance: public bass_tightener_impl::bass_tightener_t_<NV>
{
	
	struct metadata
	{
		static const int NumTables = 0;
		static const int NumSliderPacks = 0;
		static const int NumAudioFiles = 0;
		static const int NumFilters = 0;
		static const int NumDisplayBuffers = 1;
		
		SNEX_METADATA_ID(bass_tightener);
		SNEX_METADATA_NUM_CHANNELS(2);
		SNEX_METADATA_ENCODED_PARAMETERS(102)
		{
			0x005B, 0x0000, 0x4300, 0x7475, 0x664F, 0x0066, 0x0000, 0x41A0, 
            0x4000, 0x469C, 0xF2BD, 0x433E, 0x6C1A, 0x3E6B, 0x0000, 0x0000, 
            0x015B, 0x0000, 0x5400, 0x6769, 0x7468, 0x656E, 0x7373, 0x0000, 
            0x0000, 0x0000, 0xC800, 0x6642, 0x9B66, 0x0042, 0x8000, 0xCD3F, 
            0xCCCC, 0x5B3D, 0x0002, 0x0000, 0x6147, 0x6E69, 0x0000, 0x0000, 
            0x0000, 0xC800, 0xCD42, 0xCA8C, 0x0041, 0x8000, 0x003F, 0x0000, 
            0x5B00, 0x0003, 0x0000, 0x6552, 0x656C, 0x7361, 0x0065, 0x0000, 
            0x0000, 0x0000, 0x437A, 0xCCCD, 0x400C, 0x81A3, 0x3EDC, 0xCCCD, 
            0x3DCC, 0x045B, 0x0000, 0x4800, 0x7261, 0x6F6D, 0x696E, 0x7363, 
            0x0000, 0x0000, 0x0000, 0x8000, 0xDD3F, 0xF824, 0x003E, 0x8000, 
            0x003F, 0x0000, 0x5B00, 0x0005, 0x0000, 0x6F53, 0x6F6C, 0x6F4C, 
            0x4577, 0x646E, 0x0000, 0x0000, 0x0000, 0x8000, 0x003F, 0x0000, 
            0x0000, 0x8000, 0x003F, 0x8000, 0x003F, 0x0000
		};
	};
	
	instance()
	{
		// Node References -------------------------------------------------------------------------
		
		auto& smoothed_parameter = this->getT(0);                  // bass_tightener_impl::smoothed_parameter_t<NV>
		auto& one_pole = this->getT(1);                            // filters::one_pole<NV>
		auto& one_pole1 = this->getT(2);                           // filters::one_pole<NV>
		auto& freq_split3 = this->getT(3);                         // bass_tightener_impl::freq_split3_t<NV>
		auto& band1 = this->getT(3).getT(0);                       // bass_tightener_impl::band1_t<NV>
		auto& lr1_1 = this->getT(3).getT(0).getT(0);               // jdsp::jlinkwitzriley
		auto& soft_bypass = this->getT(3).getT(0).getT(1);         // bass_tightener_impl::soft_bypass_t<NV>
		auto& comp = this->getT(3).getT(0).getT(1).getT(0);        // bass_tightener_impl::comp_t<NV>
		auto& snex_shaper = this->getT(3).getT(0).getT(1).getT(1); // bass_tightener_impl::snex_shaper_t<NV>
		auto& snex_node = this->getT(3).getT(0).getT(1).getT(2);   // GainControl<NV>
		auto& wrapband21 = this->getT(3).getT(1);                  // bass_tightener_impl::wrapband21_t<NV>
		auto& band2 = this->getT(3).getT(1).getT(0);               // bass_tightener_impl::band2_t<NV>
		auto& lr2_1 = this->getT(3).getT(1).getT(0).getT(0);       // jdsp::jlinkwitzriley
		auto& gain = this->getT(3).getT(1).getT(0).getT(1);        // core::gain<NV>
		auto& pma_unscaled = this->getT(4);                        // bass_tightener_impl::pma_unscaled_t<NV>
		auto& svf_eq1 = this->getT(5);                             // filters::svf_eq<NV>
		
		// Parameter Connections -------------------------------------------------------------------
		
		auto& Band1_p = freq_split3.getParameterT(0);
		Band1_p.connectT(0, lr1_1); // Band1 -> lr1_1::Frequency
		Band1_p.connectT(1, lr2_1); // Band1 -> lr2_1::Frequency
		auto& CutOff_p = this->getParameterT(0);
		CutOff_p.connectT(0, freq_split3); // CutOff -> freq_split3::Band1
		CutOff_p.connectT(1, svf_eq1);     // CutOff -> svf_eq1::Frequency
		
		this->getParameterT(1).connectT(0, comp); // Tightness -> comp::Threshhold
		
		this->getParameterT(2).connectT(0, pma_unscaled); // Gain -> pma_unscaled::Multiply
		
		this->getParameterT(3).connectT(0, comp); // Release -> comp::Release
		
		this->getParameterT(4).connectT(0, smoothed_parameter); // Harmonics -> smoothed_parameter::Value
		
		this->getParameterT(5).connectT(0, gain); // SoloLowEnd -> gain::Gain
		
		// Modulation Connections ------------------------------------------------------------------
		
		smoothed_parameter.getParameter().connectT(0, snex_shaper);          // smoothed_parameter -> snex_shaper::drive
		pma_unscaled.getWrappedObject().getParameter().connectT(0, svf_eq1); // pma_unscaled -> svf_eq1::Gain
		comp.getParameter().connectT(0, pma_unscaled);                       // comp -> pma_unscaled::Value
		
		// Default Values --------------------------------------------------------------------------
		
		;                                          // smoothed_parameter::Value is automated
		smoothed_parameter.setParameterT(1, 250.); // control::smoothed_parameter::SmoothingTime
		smoothed_parameter.setParameterT(2, 1.);   // control::smoothed_parameter::Enabled
		
		one_pole.setParameterT(0, 20.);  // filters::one_pole::Frequency
		one_pole.setParameterT(1, 1.);   // filters::one_pole::Q
		one_pole.setParameterT(2, 0.);   // filters::one_pole::Gain
		one_pole.setParameterT(3, 0.01); // filters::one_pole::Smoothing
		one_pole.setParameterT(4, 1.);   // filters::one_pole::Mode
		one_pole.setParameterT(5, 1.);   // filters::one_pole::Enabled
		
		one_pole1.setParameterT(0, 20.);  // filters::one_pole::Frequency
		one_pole1.setParameterT(1, 1.);   // filters::one_pole::Q
		one_pole1.setParameterT(2, 0.);   // filters::one_pole::Gain
		one_pole1.setParameterT(3, 0.01); // filters::one_pole::Smoothing
		one_pole1.setParameterT(4, 1.);   // filters::one_pole::Mode
		one_pole1.setParameterT(5, 1.);   // filters::one_pole::Enabled
		
		; // freq_split3::Band1 is automated
		
		;                           // lr1_1::Frequency is automated
		lr1_1.setParameterT(1, 0.); // jdsp::jlinkwitzriley::Type
		
		;                           // comp::Threshhold is automated
		comp.setParameterT(1, 25.); // dynamics::comp::Attack
		;                           // comp::Release is automated
		comp.setParameterT(3, 32.); // dynamics::comp::Ratio
		comp.setParameterT(4, 1.);  // dynamics::comp::Sidechain
		
		; // snex_shaper::drive is automated
		
		;                           // lr2_1::Frequency is automated
		lr2_1.setParameterT(1, 1.); // jdsp::jlinkwitzriley::Type
		
		;                           // gain::Gain is automated
		gain.setParameterT(1, 20.); // core::gain::Smoothing
		gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		;                                        // pma_unscaled::Value is automated
		;                                        // pma_unscaled::Multiply is automated
		pma_unscaled.setParameterT(2, 0.010125); // control::pma_unscaled::Add
		
		;                                   // svf_eq1::Frequency is automated
		svf_eq1.setParameterT(1, 0.328628); // filters::svf_eq::Q
		;                                   // svf_eq1::Gain is automated
		svf_eq1.setParameterT(3, 0.0175);   // filters::svf_eq::Smoothing
		svf_eq1.setParameterT(4, 2.);       // filters::svf_eq::Mode
		svf_eq1.setParameterT(5, 1.);       // filters::svf_eq::Enabled
		
		this->setParameterT(0, 190.948);
		this->setParameterT(1, 77.7);
		this->setParameterT(2, 25.3188);
		this->setParameterT(3, 2.2);
		this->setParameterT(4, 0.484656);
		this->setParameterT(5, 0.);
		this->setExternalData({}, -1);
	}
	~instance() override
	{
		// Cleanup external data references --------------------------------------------------------
		
		this->setExternalData({}, -1);
	}
	
	static constexpr bool isPolyphonic() { return NV > 1; };
	
	static constexpr bool hasTail() { return true; };
	
	static constexpr bool isSuspendedOnSilence() { return false; };
	
	void setExternalData(const ExternalData& b, int index)
	{
		// External Data Connections ---------------------------------------------------------------
		
		this->getT(3).getT(0).getT(1).getT(0).setExternalData(b, index); // bass_tightener_impl::comp_t<NV>
		this->getT(3).getT(0).getT(1).getT(1).setExternalData(b, index); // bass_tightener_impl::snex_shaper_t<NV>
	}
};
}

#undef getT
#undef connectT
#undef setParameterT
#undef setParameterWT
#undef getParameterT
// ======================================| Public Definition |======================================

namespace project
{
// polyphonic template declaration

template <int NV>
using bass_tightener = wrap::node<bass_tightener_impl::instance<NV>>;
}


