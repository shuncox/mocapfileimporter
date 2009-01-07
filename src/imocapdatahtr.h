////////////////////////////////////////////////////////////////////////////
//
//  MoCap File Importer
//  Copyright(c) Shun Cox (shuncox@gmail.com)
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
////////////////////////////////////////////////////////////////////////////
//
// $Id: imocapdatahtr.h,v 1.2 2006/05/26 07:18:59 zhangshun Exp $
//
////////////////////////////////////////////////////////////////////////////

#ifndef __IMOCAPDATAHTR_H__
#define __IMOCAPDATAHTR_H__

#include "imocapdata.h"

///////////////////////////////////////////////////////////////////////////////
// class for HTR mocap files
//
class iMocapDataHtr : public iMocapData {
public:
	iMocapDataHtr(istream *in, iSkeleton *sk) : iMocapData(in, sk) {}
private:
	//////////////////////////////////////
	// inner class for file parsing
	//
	class iTokenizerHtr {
		istream *input;
	public:
		iTokenizerHtr() : input(NULL) {}
		iTokenizerHtr(istream *in) : input(in) {}
		int attach(istream *in);
		int getWords(vector<string> &words);
	};
	//
	//////////////////////////////////////

	//////////////////////////////////////
	// parse mocap data
	int parsing();
};

#endif	// #ifndef __IMOCAPDATAHTR_H__
