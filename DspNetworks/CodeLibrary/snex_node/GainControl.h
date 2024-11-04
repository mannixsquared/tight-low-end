template <int NV> struct GainControl
{
	SNEX_NODE(GainControl);
	
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
	
	float getSample(float x)
	{		
		return x + 0.1f * chebyshev(2, x) + 0.05f * chebyshev(3, x);
	}
	
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
		 
		// multiply the entire block with 0.5f (-6dB)
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
