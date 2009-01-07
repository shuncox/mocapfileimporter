////////////////////////////////////////////////////////////////////////////
//
//  imocapUtilz
//  Copyright(c) Shun Cox (shuncox@gmail.com)
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
////////////////////////////////////////////////////////////////////////////
//
// $Id: pluginmain.cpp,v 1.4 2006/12/27 13:22:24 zhangshun Exp $
//
////////////////////////////////////////////////////////////////////////////


#include "imocapimport.h"
#include "idebug.h"

#include <maya/MFnPlugin.h>

const char *IM_IMPORTER_NAME = "mocap";
const char *IM_VENDOR_NAME = "Shun Cox @ CAPG, ZJU";

const char *const imocapImportOptionScript = "imocapImportOptions";
const char *const imocapImportDefaultOptions = 
	"bonesOnly=false;merge=false;scale=1;rotationOrder=none;startFrame=0;endFrame=2147483648";

//-----------------------------------------------------------------------------
// Initialize Plug-in
//-----------------------------------------------------------------------------
MStatus initializePlugin(MObject obj)
{
	MStatus stat = MS::kFailure;

	MFnPlugin impPlugIn(obj, IM_VENDOR_NAME, "0.8.5");
	stat = impPlugIn.registerFileTranslator(IM_IMPORTER_NAME, "none",
											imocapImport::creator,
											(char *)imocapImportOptionScript,
											(char *)imocapImportDefaultOptions, 
											false);

	ILOG2("Plug-in was loaded.");

//	if (stat != MS::kSuccess) {
//		return stat;
//	}
//
//	MFnPlugin expPlugIn(obj, IM_VENDOR_NAME, "0.8.0");
//	stat = expPlugIn.registerFileTranslator("imocapExport", "",
//										imocapExport::creator,
//										(char *)imocapExportOptionScript,
//										(char *)imocapExportDefaultOptions,
//										false);

	return stat;
}

//-----------------------------------------------------------------------------
// Uninitialize Plug-in
//-----------------------------------------------------------------------------
MStatus uninitializePlugin(MObject obj)
{
	MStatus stat = MS::kFailure;

	MFnPlugin impPlugIn(obj);
	stat = impPlugIn.deregisterFileTranslator(IM_IMPORTER_NAME);

	ILOG2("Plug-in was unloaded.");

	//	if (stat != MS::kSuccess) {
//		return stat;
//	}

//	MFnPlugin expPlugIn(obj);
//	stat = expPlugIn.deregisterFileTranslator("imocapExport");

	return stat;
}
