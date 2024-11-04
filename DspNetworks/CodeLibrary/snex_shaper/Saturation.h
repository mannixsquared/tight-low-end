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
		return x + drive * chebyshev(1, x) + drive * 0.5f * chebyshev(2, x);
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
