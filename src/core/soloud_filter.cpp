/*
SoLoud audio engine
Copyright (c) 2013-2014 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#include "soloud.h"

namespace SoLoud
{

	Filter::Filter()
	{
	}

	Filter::~Filter()
	{
	}
	
	FilterInstance::FilterInstance()
	{
		mNumParams = 0;
		mParamChanged = 0;
		mParam = 0;
		mParamFader = 0;
	}

	void FilterInstance::initParams(int aNumParams)
	{		
		mNumParams = aNumParams;
		mParam = new float[mNumParams];
		mParamFader = new Fader[mNumParams];
		int i;
		for (i = 0; i < mNumParams; i++)
		{
			mParam[i] = 0;
			mParamFader[i].mActive = 0;
		}
		mParam[0] = 1; // set 'wet' to 1
	}

	void FilterInstance::updateParams(float aTime)
	{
		int i;
		for (i = 0; i < mNumParams; i++)
		{
			if (mParamFader[i].mActive > 0)
			{
				mParamChanged |= 1 << i;
				mParam[i] = mParamFader[i].get(aTime);
			}
		}
	}

	FilterInstance::~FilterInstance()
	{
		delete[] mParam;
		delete[] mParamFader;
	}

	void FilterInstance::setFilterParameter(int aAttributeId, float aValue)
	{
		if (aAttributeId < 0 || aAttributeId >= mNumParams)
			return;

		mParamFader[aAttributeId].mActive = 0;
		mParam[aAttributeId] = aValue;
		mParamChanged |= 1 << aAttributeId;
	}

	void FilterInstance::fadeFilterParameter(int aAttributeId, float aTo, float aTime, float aStartTime)
	{
		if (aAttributeId < 0 || aAttributeId >= mNumParams || aTime <= 0 || aTo == mParam[aAttributeId])
			return;

		mParamFader[aAttributeId].set(mParam[aAttributeId], aTo, aTime, aStartTime);
	}

	void FilterInstance::oscillateFilterParameter(int aAttributeId, float aFrom, float aTo, float aTime, float aStartTime)
	{
		if (aAttributeId < 0 || aAttributeId >= mNumParams || aTime <= 0 || aFrom == aTo)
			return;

		mParamFader[aAttributeId].setLFO(aFrom, aTo, aTime, aStartTime);
	}

	float FilterInstance::getFilterParameter(int aAttributeId)
	{
		if (aAttributeId < 0 || aAttributeId >= mNumParams)
			return 0;

		return mParam[aAttributeId];
	}

	void FilterInstance::filter(float *aBuffer, int aSamples, int aChannels, float aSamplerate, float aTime)
	{
		int i;
		for (i = 0; i < aChannels; i++)
		{
			filterChannel(aBuffer + i* aSamples, aSamples, aSamplerate, aTime, i, aChannels);
		}
	}

	void FilterInstance::filterChannel(float *aBuffer, int aSamples, float aSamplerate, float aTime, int aChannel, int aChannels)
	{
	}

};

