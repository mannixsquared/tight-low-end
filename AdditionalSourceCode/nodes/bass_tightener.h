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

template <int NV>
using soft_bypass_t_ = container::chain<parameter::empty, 
                                        wrap::fix<2, comp_t<NV>>, 
                                        snex_shaper_t<NV>, 
                                        core::gain<NV>>;

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
using wrapband22_t = container::chain<parameter::empty, 
                                      wrap::fix<2, band2_t<NV>>>;

namespace freq_split3_t_parameters
{
}

template <int NV>
using freq_split3_t = container::split<parameter::plain<jdsp::jlinkwitzriley, 0>, 
                                       wrap::fix<2, band1_t<NV>>, 
                                       wrapband22_t<NV>>;

template <int NV>
using multi_t = container::multi<parameter::empty, 
                                 wrap::fix<1, math::mul<NV>>, 
                                 wrap::fix<1, filters::linkwitzriley<NV>>>;

template <int NV>
using chain_t = container::chain<parameter::empty, 
                                 wrap::fix<2, routing::ms_decode>, 
                                 multi_t<NV>, 
                                 routing::ms_encode>;

template <int NV>
using wrappma_unscaled1_t = container::chain<parameter::empty, 
                                             wrap::fix<2, pma_unscaled_t<NV>>, 
                                             filters::svf_eq<NV>>;

template <int NV>
using soft_bypass1_t_ = container::chain<parameter::empty, 
                                         wrap::fix<2, chain_t<NV>>, 
                                         wrappma_unscaled1_t<NV>>;

template <int NV>
using soft_bypass1_t = bypass::smoothed<20, soft_bypass1_t_<NV>>;

namespace bass_tightener_t_parameters
{
// Parameter list for bass_tightener_impl::bass_tightener_t ----------------------------------------

template <int NV>
using CutOff = parameter::chain<ranges::Identity, 
                                parameter::plain<bass_tightener_impl::freq_split3_t<NV>, 0>, 
                                parameter::plain<jdsp::jlinkwitzriley, 0>>;

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

DECLARE_PARAMETER_RANGE_SKEW(Tightness_1Range, 
                             0., 
                             6., 
                             5.42227);

template <int NV>
using Tightness_1 = parameter::from0To1<core::gain<NV>, 
                                        0, 
                                        Tightness_1Range>;

template <int NV>
using Tightness = parameter::chain<Tightness_InputRange, 
                                   Tightness_0<NV>, 
                                   Tightness_1<NV>>;

template <int NV>
using SoloLowEnd = parameter::from0To1_inv<core::gain<NV>, 
                                           0, 
                                           Tightness_0Range>;

template <int NV>
using Enabled_0 = parameter::bypass<bass_tightener_impl::soft_bypass_t<NV>>;

template <int NV>
using Enabled_1 = parameter::bypass<bass_tightener_impl::soft_bypass1_t<NV>>;

template <int NV>
using Enabled = parameter::chain<ranges::Identity, 
                                 Enabled_0<NV>, 
                                 Enabled_1<NV>, 
                                 Enabled_0<NV>>;

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
                                               SoloLowEnd<NV>, 
                                               Enabled<NV>>;
}

template <int NV>
using bass_tightener_t_ = container::chain<bass_tightener_t_parameters::bass_tightener_t_plist<NV>, 
                                           wrap::fix<2, smoothed_parameter_t<NV>>, 
                                           freq_split3_t<NV>, 
                                           soft_bypass1_t<NV>>;

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
		SNEX_METADATA_ENCODED_PARAMETERS(118)
		{
			0x005B, 0x0000, 0x4300, 0x7475, 0x664F, 0x0066, 0x0000, 0x41A0, 
            0x4000, 0x469C, 0x0000, 0x42B4, 0x6C1A, 0x3E6B, 0x0000, 0x0000, 
            0x015B, 0x0000, 0x5400, 0x6769, 0x7468, 0x656E, 0x7373, 0x0000, 
            0x0000, 0x0000, 0xC800, 0x0042, 0x0000, 0x0000, 0x8000, 0xCD3F, 
            0xCCCC, 0x5B3D, 0x0002, 0x0000, 0x6147, 0x6E69, 0x0000, 0x0000, 
            0x0000, 0xC800, 0x0042, 0x0000, 0x0000, 0x8000, 0x003F, 0x0000, 
            0x5B00, 0x0003, 0x0000, 0x6552, 0x656C, 0x7361, 0x0065, 0x0000, 
            0x0000, 0x0000, 0x437A, 0x0000, 0x4248, 0x81A3, 0x3EDC, 0xCCCD, 
            0x3DCC, 0x045B, 0x0000, 0x4800, 0x7261, 0x6F6D, 0x696E, 0x7363, 
            0x0000, 0x0000, 0x0000, 0x8000, 0x003F, 0x0000, 0x0000, 0x8000, 
            0x003F, 0x0000, 0x5B00, 0x0005, 0x0000, 0x6F53, 0x6F6C, 0x6F4C, 
            0x4577, 0x646E, 0x0000, 0x0000, 0x0000, 0x8000, 0x003F, 0x0000, 
            0x0000, 0x8000, 0x003F, 0x8000, 0x5B3F, 0x0006, 0x0000, 0x6E45, 
            0x6261, 0x656C, 0x0064, 0x0000, 0x0000, 0x0000, 0x3F80, 0x0000, 
            0x3F80, 0x0000, 0x3F80, 0x0000, 0x3F80, 0x0000
		};
	};
	
	instance()
	{
		// Node References -------------------------------------------------------------------------
		
		auto& smoothed_parameter = this->getT(0);                    // bass_tightener_impl::smoothed_parameter_t<NV>
		auto& freq_split3 = this->getT(1);                           // bass_tightener_impl::freq_split3_t<NV>
		auto& band1 = this->getT(1).getT(0);                         // bass_tightener_impl::band1_t<NV>
		auto& lr1_1 = this->getT(1).getT(0).getT(0);                 // jdsp::jlinkwitzriley
		auto& soft_bypass = this->getT(1).getT(0).getT(1);           // bass_tightener_impl::soft_bypass_t<NV>
		auto& comp = this->getT(1).getT(0).getT(1).getT(0);          // bass_tightener_impl::comp_t<NV>
		auto& snex_shaper = this->getT(1).getT(0).getT(1).getT(1);   // bass_tightener_impl::snex_shaper_t<NV>
		auto& gain1 = this->getT(1).getT(0).getT(1).getT(2);         // core::gain<NV>
		auto& wrapband22 = this->getT(1).getT(1);                    // bass_tightener_impl::wrapband22_t<NV>
		auto& band2 = this->getT(1).getT(1).getT(0);                 // bass_tightener_impl::band2_t<NV>
		auto& lr2_1 = this->getT(1).getT(1).getT(0).getT(0);         // jdsp::jlinkwitzriley
		auto& gain = this->getT(1).getT(1).getT(0).getT(1);          // core::gain<NV>
		auto& soft_bypass1 = this->getT(2);                          // bass_tightener_impl::soft_bypass1_t<NV>
		auto& chain = this->getT(2).getT(0);                         // bass_tightener_impl::chain_t<NV>
		auto& ms_decode = this->getT(2).getT(0).getT(0);             // routing::ms_decode
		auto& multi = this->getT(2).getT(0).getT(1);                 // bass_tightener_impl::multi_t<NV>
		auto& mul = this->getT(2).getT(0).getT(1).getT(0);           // math::mul<NV>
		auto& linkwitzriley = this->getT(2).getT(0).getT(1).getT(1); // filters::linkwitzriley<NV>
		auto& ms_encode = this->getT(2).getT(0).getT(2);             // routing::ms_encode
		auto& wrappma_unscaled1 = this->getT(2).getT(1);             // bass_tightener_impl::wrappma_unscaled1_t<NV>
		auto& pma_unscaled = this->getT(2).getT(1).getT(0);          // bass_tightener_impl::pma_unscaled_t<NV>
		auto& svf_eq1 = this->getT(2).getT(1).getT(1);               // filters::svf_eq<NV>
		
		// Parameter Connections -------------------------------------------------------------------
		
		freq_split3.getParameterT(0).connectT(0, lr1_1); // Band1 -> lr1_1::Frequency
		auto& CutOff_p = this->getParameterT(0);
		CutOff_p.connectT(0, freq_split3); // CutOff -> freq_split3::Band1
		CutOff_p.connectT(1, lr2_1);       // CutOff -> lr2_1::Frequency
		
		auto& Tightness_p = this->getParameterT(1);
		Tightness_p.connectT(0, comp);  // Tightness -> comp::Threshhold
		Tightness_p.connectT(1, gain1); // Tightness -> gain1::Gain
		
		this->getParameterT(2).connectT(0, pma_unscaled); // Gain -> pma_unscaled::Multiply
		
		this->getParameterT(3).connectT(0, comp); // Release -> comp::Release
		
		this->getParameterT(4).connectT(0, smoothed_parameter); // Harmonics -> smoothed_parameter::Value
		
		this->getParameterT(5).connectT(0, gain); // SoloLowEnd -> gain::Gain
		
		auto& Enabled_p = this->getParameterT(6);
		Enabled_p.connectT(0, soft_bypass);  // Enabled -> soft_bypass::Bypassed
		Enabled_p.connectT(1, soft_bypass1); // Enabled -> soft_bypass1::Bypassed
		Enabled_p.connectT(2, soft_bypass);  // Enabled -> soft_bypass::Bypassed
		
		// Modulation Connections ------------------------------------------------------------------
		
		smoothed_parameter.getParameter().connectT(0, snex_shaper);          // smoothed_parameter -> snex_shaper::drive
		pma_unscaled.getWrappedObject().getParameter().connectT(0, svf_eq1); // pma_unscaled -> svf_eq1::Gain
		comp.getParameter().connectT(0, pma_unscaled);                       // comp -> pma_unscaled::Value
		
		// Default Values --------------------------------------------------------------------------
		
		;                                          // smoothed_parameter::Value is automated
		smoothed_parameter.setParameterT(1, 250.); // control::smoothed_parameter::SmoothingTime
		smoothed_parameter.setParameterT(2, 1.);   // control::smoothed_parameter::Enabled
		
		; // freq_split3::Band1 is automated
		
		;                           // lr1_1::Frequency is automated
		lr1_1.setParameterT(1, 0.); // jdsp::jlinkwitzriley::Type
		
		;                           // comp::Threshhold is automated
		comp.setParameterT(1, 10.); // dynamics::comp::Attack
		;                           // comp::Release is automated
		comp.setParameterT(3, 4);   // dynamics::comp::Ratio
		comp.setParameterT(4, 1.);  // dynamics::comp::Sidechain
		
		; // snex_shaper::drive is automated
		
		;                            // gain1::Gain is automated
		gain1.setParameterT(1, 20.); // core::gain::Smoothing
		gain1.setParameterT(2, 0.);  // core::gain::ResetValue
		
		;                           // lr2_1::Frequency is automated
		lr2_1.setParameterT(1, 1.); // jdsp::jlinkwitzriley::Type
		
		;                           // gain::Gain is automated
		gain.setParameterT(1, 20.); // core::gain::Smoothing
		gain.setParameterT(2, 0.);  // core::gain::ResetValue
		
		mul.setParameterT(0, 1.); // math::mul::Value
		
		linkwitzriley.setParameterT(0, 120.);     // filters::linkwitzriley::Frequency
		linkwitzriley.setParameterT(1, 0.336454); // filters::linkwitzriley::Q
		linkwitzriley.setParameterT(2, 0.);       // filters::linkwitzriley::Gain
		linkwitzriley.setParameterT(3, 0.01);     // filters::linkwitzriley::Smoothing
		linkwitzriley.setParameterT(4, 1.);       // filters::linkwitzriley::Mode
		linkwitzriley.setParameterT(5, 1.);       // filters::linkwitzriley::Enabled
		
		;                                        // pma_unscaled::Value is automated
		;                                        // pma_unscaled::Multiply is automated
		pma_unscaled.setParameterT(2, 0.010125); // control::pma_unscaled::Add
		
		svf_eq1.setParameterT(0, 60.);     // filters::svf_eq::Frequency
		svf_eq1.setParameterT(1, 1.23145); // filters::svf_eq::Q
		;                                  // svf_eq1::Gain is automated
		svf_eq1.setParameterT(3, 0.01675); // filters::svf_eq::Smoothing
		svf_eq1.setParameterT(4, 4.);      // filters::svf_eq::Mode
		svf_eq1.setParameterT(5, 1.);      // filters::svf_eq::Enabled
		
		this->setParameterT(0, 90.);
		this->setParameterT(1, 0.);
		this->setParameterT(2, 0.);
		this->setParameterT(3, 50.);
		this->setParameterT(4, 0.);
		this->setParameterT(5, 0.);
		this->setParameterT(6, 1.);
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
		
		this->getT(1).getT(0).getT(1).getT(0).setExternalData(b, index); // bass_tightener_impl::comp_t<NV>
		this->getT(1).getT(0).getT(1).getT(1).setExternalData(b, index); // bass_tightener_impl::snex_shaper_t<NV>
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


