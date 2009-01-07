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
// $Id: imocapdatahtr.cpp,v 1.3 2007/02/25 16:39:19 zhangshun Exp $
//
////////////////////////////////////////////////////////////////////////////

#include "idebug.h"
#include "imocapdatahtr.h"

using namespace imath;

//-----------------------------------------------------------------------------
// attach
//-----------------------------------------------------------------------------
int iMocapDataHtr::iTokenizerHtr::attach(istream *in)
{
	if (NULL == in) {
		ILOG4 ("Error: Invalid stream");
		return MC_INVALID_STREAM;
	}
	input = in;
	ILOG0 ("Success");
	return MC_SUCCESS;
}

//-----------------------------------------------------------------------------
// getWord
//-----------------------------------------------------------------------------
int iMocapDataHtr::iTokenizerHtr::getWords(vector<string> &words)
{
	// check input stream and skeleton
	if (NULL == input) {
		ILOG4 ("Error: Invalid stream");
		return MC_INVALID_STREAM;
	}

	// if EOF then return
	if (input->eof()) {
		ILOG0 ("End Of File");
		return MC_EOF;
	}

	// retrieve a line from stream
	//
	string textLine;
	getline(*input, textLine);
	
	// trim it
	//
	textLine = trimString(textLine);
	
	// get rid of comment
	//
	textLine = uncommentString(textLine);

	// tokenize it
	words.clear();
	tokenizeString(textLine, words);

	return MC_SUCCESS;
}

//-----------------------------------------------------------------------------
// parse mocap data
//-----------------------------------------------------------------------------
int iMocapDataHtr::parsing()
{
	int result = MC_SUCCESS;
	
	// check input stream and skeleton
	//
	if (NULL == input) {
		ILOG4 ("Error: Invalid stream");
		return MC_INVALID_STREAM;
	}
	if (NULL == skeleton) {
		ILOG4 ("Error: Invalid skeleton");
		return MC_INVALID_SKELETON;
	}
	// initialization
	//
	enum MC_HTR_STAGE { MC_HTR_STAGE_NONE, MC_HTR_STAGE_HEADER, MC_HTR_STAGE_SEGMENT,
		MC_HTR_STAGE_BASE, MC_HTR_STAGE_FRAMES, MC_HTR_STAGE_FINISH };
	MC_HTR_STAGE stage = MC_HTR_STAGE_NONE;

	unsigned int frameCount = 0;	// quantity of frames
	float frameTime = 0;			// time of frame

	vector<string> words;
	iTokenizerHtr tokenHtr(input);
	unsigned int argsCount = 0;

	// variables
	iSkeleton::iJoint *joint;
	iVec offset, rotation, rootOffset;
	float length = 0.0;
	iSkeleton::iFrame frame;
	bool haveTranslation = false;

	// accessory
	vector<iSkeleton::iJoint*> jointIndex;
	string firstJointName;
	iSkeleton::iJoint *currentJoint = NULL;

	// variables from file header
	string htrFileType("htr");
	string htrDataType("HTRS");
	int htrVersion = 1;
	int htrSegments = 0;
	int htrFrameRate = 60;
	int htrFrames = 0;
	int htrOrder = Rotation::MC_RO_ZYX;
	bool htrRotationUnits = true;	// true for degrees, false for radians

	float htrScaleFactor = 1.0F;		// scale factor
	float htrProportion = 0.1F;		// determined by unit

	// parsering loop
	//
	while ((stage != MC_HTR_STAGE_FINISH) &&
		((result = tokenHtr.getWords(words)) == MC_SUCCESS)) {

		// empty line
		if (words.empty()) continue;

		// get the first word
		vector<string>::iterator iter = words.begin();
		const string title(*iter);
		++iter;
		argsCount = static_cast<int>(words.size() - 1);

		///////////////////////////////
		// lay these codes temporarily

		//string s;

		//s = "'";
		//s.append(*iter);
		//s.append("'");
		//++iter;
		//if (iter != words.end()) {
		//	s.append(" : ");
		//	for (; iter != words.end(); ++iter) {
		//		s.append(*iter);
		//		s.append("  ");
		//	}
		//}

		//ILOG0 (s);

		//	
		///////////////////////////////

		switch (stage) {

		/////////////////////////////////////
		// The begining of HTR file
		case MC_HTR_STAGE_NONE:
			if (!compareNoncase(title, "[Header]")) {
				stage = MC_HTR_STAGE_HEADER;
				ILOG0 ("Goto Header Section");
				continue;
			}
			// nothing to do
			break;

		/////////////////////////////////////
		// Header section
		case MC_HTR_STAGE_HEADER:
			if (!compareNoncase(title, "[SegmentNames&Hierarchy]")) {
				// verify file header
				//
				result = MC_ILLEAGAL_DATA;
				if (compareNoncase(htrFileType, "htr")) {
					ILOG4 ("Error: Unsupported file type in Header");
					break;
				}
				if (compareNoncase(htrDataType, "HTRS")) {
					ILOG4 ("Error: Unsupported data type in Header");
					break;
				}
				if (htrVersion != 1 && htrVersion !=2) {
					ILOG4 ("Error: Unsupported file version in Header");
					break;
				}
				if (htrFrameRate < 1 && htrFrameRate > maxFrameRate) {
					ILOG4 ("Error: Illegal frame rate in Header");
					break;
				}
				if (htrSegments < 1 && htrSegments > maxNumJoints) {
					ILOG4 ("Error: Illegal number of segments in Header");
					break;
				}
				if (htrOrder == Rotation::MC_RO_NONE) {
					ILOG4 ("Error: Unsupported rotation order in Header");
					break;
				}

				result = MC_SUCCESS;

				// goto next stage
				stage = MC_HTR_STAGE_SEGMENT;
				ILOG0 ("Goto SegmentNames&Hierarchy Section");
				continue;
			}

			// todo: get header
			//
			// check arguments
			if (argsCount != 1) {
				result = MC_ILLEAGAL_DATA;
				ILOG4 ("Error: The number of arguments in Header was invalid");
				break;
			}

			//[Header]						# Header keywords are followed by a single value
			//FileType				htr		# single word string
			//DataType				HTRS	# Hierarchical translations followed by rotations and Scale
			//FileVersion			1		# integer
			//NumSegments			18		# integer
			//NumFrames				2		# integer
			//DataFrameRate			30		# integer
			//EulerRotationOrder	ZYX		# one word string
			//CalibrationUnits		mm		# one word string
			//RotationUnits			Degrees	# one word string
			//GlobalAxisofGravity	Y		# character, X or Y or Z
			//BoneLengthAxis		Y
			//ScaleFactor			1

			if (!compareNoncase(title, "FileType")) {
				htrFileType = words[1];
				ILOG2 ("Info: Gotta Header.FileType - " << htrFileType);
			} else if (!compareNoncase(title, "DataType")) {
				htrDataType = words[1];
				ILOG2 ("Info: Gotta Header.DataType - " << htrDataType);
			} else if (!compareNoncase(title, "FileVersion")) {
				htrVersion = fromString<int>(words[1]);
				ILOG2 ("Info: Gotta Header.FileVersion - " << htrVersion);
			} else if (!compareNoncase(title, "DataFrameRate")) {
				htrFrameRate = fromString<int>(words[1]);
				ILOG2 ("Info: Gotta Header.DataFrameRate - " << htrFrameRate);
			} else if (!compareNoncase(title, "NumSegments")) {
				htrSegments = fromString<int>(words[1]);
				ILOG2 ("Info: Gotta Header.NumSegments - " << htrSegments);
			} else if (!compareNoncase(title, "NumFrames")) {
				htrFrames = fromString<int>(words[1]);
				ILOG2 ("Info: Gotta Header.NumFrames - " << htrFrames);
			} else if (!compareNoncase(title, "EulerRotationOrder")) {
				htrOrder = Rotation::getOrderFromString(words[1]);
				ILOG2 ("Info: Gotta Header.EulerRotationOrder - " << words[1]);
			} else if (!compareNoncase(title, "ScaleFactor")) {
				htrScaleFactor = fromString<float>(words[1]);
				ILOG2 ("Info: Gotta Header.ScaleFactor - " << htrScaleFactor);
			} else if (!compareNoncase(title, "CalibrationUnits")) {
				if (!compareNoncase(words[1], "mm")) {
					htrProportion = 0.1F;
				} else if (!compareNoncase(words[1], "cm")) {
					htrProportion = 1.0F;
				} else if (!compareNoncase(words[1], "m")) {
					htrProportion = 100.0F;
				}
				ILOG2 ("Info: Gotta Header.CalibrationUnits - " << htrProportion);
			} else if (!compareNoncase(title, "RotationUnits")) {
				htrRotationUnits = !compareNoncase(words[1], "Degrees");
				ILOG2 ("Info: Gotta Header.RotationUnits - " << title);
			} else if (!compareNoncase(title, "GlobalAxisofGravity")) {
				// unavailable now
				ILOG2 ("Info: Gotta Header.GlobalAxisofGravity - " << title);
			} else if (!compareNoncase(title, "BoneLengthAxis")) {
				// unavailable now
				ILOG2 ("Info: Gotta Header.BoneLengthAxis - " << title);
			}
			break;

		/////////////////////////////////////
		// Segment & hierarchy section
		case MC_HTR_STAGE_SEGMENT:
			if (!compareNoncase(title, "[BasePosition]")) {
				stage = MC_HTR_STAGE_BASE;
				ILOG0 ("Goto BasePosition Section");
				continue;
			}
			// todo: build skeleton
			//
			// check arguments
			if (argsCount != 1) {
				result = MC_ILLEAGAL_DATA;
				ILOG4 ("Error: The number of arguments in Segment was invalid");
				break;
			}

			// seek GLOBAL
			if (!skeleton->empty()) {
				// root is exists
				if (!compareNoncase(words[1], "GLOBAL")) {
					// Duplicated GLOBAL
					result = MC_ILLEAGAL_DATA;
					ILOG4 ("Error: Duplicated GLOBAL in Segment");
					break;
				} else {
					// Other joints, seek their parent
					if (skeleton->goHere(skeleton->getJoint(words[1])) != MC_SUCCESS) {
						// Illegal parent name
						result = MC_ILLEAGAL_DATA;
						ILOG4 ("Error: Illegal parent name");
						break;
					}
				}
			} else {
				// root is not exists
				if (compareNoncase(words[1], "GLOBAL")) {
					// The parent is not GLOBAL
					result = MC_ILLEAGAL_DATA;
					ILOG4 ("Error: The first parent in Segment was not GLOBAL");
					break;
				} else {
					// Aha, this is the first joint
					firstJointName = title;
				}
			}

			// add a joint to the skeleton
			result = skeleton->addJoint(title, offset, rotation);
			if (MC_SUCCESS != result) {
				if (MC_DUP_JOINT_NAME == result) {
					ILOG4 ("Error: Joint duplicated");
				}
				break;
			}

			break;

		/////////////////////////////////////
		// Base position section
		case MC_HTR_STAGE_BASE:
			if (htrVersion == 2) {
				// HTR Version 2
				// Frames section start with 'Frame 1:'
				//
				if (!compareNoncase(title, "Frame")) {
					string s = words[1];
					int frameNo = fromString<int>(s.erase(s.length() - 1));
					if ((*(words[1].rbegin()) == ':') && (frameNo == 1)) {
						stage = MC_HTR_STAGE_FRAMES;
						ILOG0 ("Goto Motion Section (HTR 2)");
						continue;
					}
				}
			} else {
				// HTR Version 1 (default)
				//
				if (argsCount == 0 && !compareNoncase(title, "[" + firstJointName + "]")) {
					// set current joint
					if ((currentJoint = skeleton->getJoint(firstJointName)) == NULL) {
						result = MC_ILLEAGAL_DATA;
						ILOG4 ("Error: The joint name in Frames was invalid");
						break;
					}
					stage = MC_HTR_STAGE_FRAMES;
					ILOG0 ("Goto Motion Section (HTR 1)");
					continue;
				}
				//ILOG1 ("Title = " << title << " First Joint Name = " << firstJointName);
			}
			// todo: retrieve base position info.
			//
			// check arguments
			//ILOG1 ("Args in base position = " << argsCount);
			if (argsCount != 7) {
				result = MC_ILLEAGAL_DATA;
				ILOG4 ("Error: The number of arguments in BasePosition was invalid");
				break;
			}

			joint = skeleton->getJoint(title);
			if (joint != NULL) {
				// set offset & rotation & length
				offset.x = fromString<float>(words[1]) * htrProportion * htrScaleFactor;
				offset.y = fromString<float>(words[2]) * htrProportion * htrScaleFactor;
				offset.z = fromString<float>(words[3]) * htrProportion * htrScaleFactor;
				joint->setOffset(offset);

				if (htrRotationUnits) {
					// degrees
					rotation.x = fromString<float>(words[4]);
					rotation.y = fromString<float>(words[5]);
					rotation.z = fromString<float>(words[6]);
				} else {
					// radians
					rotation.x = toDegrees(fromString<float>(words[4]));
					rotation.y = toDegrees(fromString<float>(words[5]));
					rotation.z = toDegrees(fromString<float>(words[6]));
				}

				joint->setRotation(rotation);
				length = fromString<float>(words[7]);
				joint->setLength(length);
				// add to index table
				jointIndex.push_back(joint);
			}

			break;

		/////////////////////////////////////
		// Frames section
		case MC_HTR_STAGE_FRAMES:
			if (!compareNoncase(title, "[EndOfFile]")) {
				stage = MC_HTR_STAGE_FINISH;
				ILOG0 ("The end of sections");

				// assign values to variables in skeleton
				//
				float frameTime = 1.0F / htrFrameRate;
				skeleton->setFrames(htrFrames);
				skeleton->setFrameTime(frameTime);
				skeleton->setRotOrder(htrOrder);
				skeleton->setHaveTranslation(haveTranslation);
				// now, okay!

				continue;
			}
			// todo: retrieve motion data
			//
			if (htrVersion == 2) {
				// HTR Version 2
				// check arguments
				if (compareNoncase(title, "Frame")) {
					string s = title;
					int frameNo = fromString<int>(s.erase(s.length() - 1));
					if ((*(title.rbegin()) == ':') && (frameNo <= htrFrames)) {
						// We ignore the number of frames...just increase it by 1
						//
						ILOG1 ("Frame " << frameNo << " starts...");
						continue;
					} else {
						result = MC_ILLEAGAL_DATA;
						ILOG4 ("Error: The number of frame (HTR 2) is out of bound or syntex is incorrect");
						break;
					}
				}
				if (argsCount != 3 || argsCount != 4) {
					result = MC_ILLEAGAL_DATA;
					ILOG4 ("Error: The number of arguments in Frames (HTR 2) was invalid");
					break;
				}
				// get the number of bones
				//
				string s = words[0];
				int boneNo = fromString<int>(s.erase(s.length() - 1));
				//ILOG2 ("bone no.= " << boneNo)
				if ((*(words[0].rbegin()) != ':') || (boneNo < 0) || (boneNo >= static_cast<int>(jointIndex.size()))) {
					result = MC_ILLEAGAL_DATA;
					ILOG4 ("Error: The number of bone (HTR 2) in the frames is out of bound or syntex is incorrect");
					break;
				}
				// assign
				//
				if (boneNo == 0) {
					// translation of root
					rootOffset.x = fromString<float>(words[1]) * htrProportion * htrScaleFactor;
					rootOffset.y = fromString<float>(words[2]) * htrProportion * htrScaleFactor;
					rootOffset.z = fromString<float>(words[3]) * htrProportion * htrScaleFactor;
				} else {
					currentJoint = jointIndex[boneNo - 1];
					// set rotation
					if (htrRotationUnits) {
						// degrees
						frame.rotation.x = fromString<float>(words[1]);
						frame.rotation.y = fromString<float>(words[2]);
						frame.rotation.z = fromString<float>(words[3]);
					} else {
						// radians
						frame.rotation.x = toDegrees(fromString<float>(words[1]));
						frame.rotation.y = toDegrees(fromString<float>(words[2]));
						frame.rotation.z = toDegrees(fromString<float>(words[3]));
					}
					if (skeleton->isRoot(currentJoint))	{
						frame.offset = rootOffset;
					} else {
						frame.offset = iVec(0.0, 0.0, 0.0);
					}
					// set the length of bone
					// which is elastic in some circumstances
					//
					frame.offset.y += fromString<float>(words[4]) * htrProportion * htrScaleFactor;
					frame.scale = 1.0;

					currentJoint->motion.push_back(frame);
					ILOG0 (currentJoint->getName() << " offset = " << frame.offset << " rotation = " << frame.rotation << " scale = " << frame.scale);
				}

			} else {
				// HTR Version 1 (default)
				// check arguments
				if (argsCount == 0) {
					if (*title.begin() == '[' && *title.rbegin() == ']') {
						string currentJointName(title.substr(1, title.length() - 2));
						ILOG1 ("Current joint -> " << currentJointName);
						if ((currentJoint = skeleton->getJoint(currentJointName)) == NULL) {
							result = MC_ILLEAGAL_DATA;
							ILOG4 ("Error: The joint name in Frames was invalid");
							break;
						}
						break;
					}
				}
				if (argsCount != 7) {
					result = MC_ILLEAGAL_DATA;
					ILOG4 ("Error: The number of arguments in Frames (HTR 1) was invalid");
					break;
				}

				// get frame data
				if (currentJoint  == NULL) {
					result = MC_ILLEAGAL_DATA;
					ILOG4 ("Error: The current joint in Frames was invalid");
					break;
				}
				// offsets
				frame.offset.x = fromString<float>(words[1]) * htrProportion * htrScaleFactor;
				frame.offset.y = fromString<float>(words[2]) * htrProportion * htrScaleFactor;
				frame.offset.z = fromString<float>(words[3]) * htrProportion * htrScaleFactor;
				
				if (currentJoint != skeleton->getJoint(firstJointName)) {
					haveTranslation = haveTranslation ||
						(abs(frame.offset.x) > motionThreshold) ||
						(abs(frame.offset.y) > motionThreshold) ||
						(abs(frame.offset.z) > motionThreshold);
				}

				// rotations
				if (htrRotationUnits) {
					// degrees
					frame.rotation.x = fromString<float>(words[4]);
					frame.rotation.y = fromString<float>(words[5]);
					frame.rotation.z = fromString<float>(words[6]);
				} else {
					// radians
					frame.rotation.x = toDegrees(fromString<float>(words[4]));
					frame.rotation.y = toDegrees(fromString<float>(words[5]));
					frame.rotation.z = toDegrees(fromString<float>(words[6]));
				}
				// scale
				frame.scale = fromString<float>(words[7]);
				// append it
				currentJoint->motion.push_back(frame);
				ILOG0 (currentJoint->getName() << " offset = " << frame.offset << " rotation = " << frame.rotation << " scale = " << frame.scale);

			}
			break;

		/////////////////////////////////////
		// Finish section
		case MC_HTR_STAGE_FINISH:
			break;

		/////////////////////////////////////
		// Default
		default:
			stage = MC_HTR_STAGE_FINISH;
			break;
		}

		// relay failures
		if (result != MC_SUCCESS) break;

	}

	// finnaly
	//
	return result;
}
