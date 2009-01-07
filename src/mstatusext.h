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
// $Id: mstatusext.h,v 1.2 2006/05/26 07:18:59 zhangshun Exp $
//
////////////////////////////////////////////////////////////////////////////

#ifndef __MSTATUSEXT_H__
#define __MSTATUSEXT_H__

#include <maya/MStatus.h>
#include "idebug.h"

#define MS_STATUS		_mstatus

#define MS_ENTRANCE		MStatus _mstatus = MStatus::kSuccess; do {

#define MS_CHECK(x)		{\
							_mstatus = (x);\
							if (_mstatus.error()) {\
								ILOG4 ("MAYA_API ERROR '"#x << "' : " << _mstatus);\
								break;\
							}\
						}

#define MS_CHECK_RELAY	if (_mstatus.error()) break;

#define MS_EXIT			} while(false);

#define MS_STORE(x)		x = _mstatus;

#define MS_RETURN		return _mstatus;

#endif	// #ifndef __MSTATUSEXT_H__
