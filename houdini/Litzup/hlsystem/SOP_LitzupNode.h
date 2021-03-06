/*
 * Copyright (c) 2019
 *	Side Effects Software Inc.  All rights reserved.
 *
 * Redistribution and use of Houdini Development Kit samples in source and
 * binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. The name of Side Effects Software may not be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY SIDE EFFECTS SOFTWARE `AS IS' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 * NO EVENT SHALL SIDE EFFECTS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *----------------------------------------------------------------------------
 * This is a simple particle system...  Not too much to it, but this can be
 * used as a template for your own...
 */


#ifndef __SOP_LitzupNode_h__
#define __SOP_LitzupNode_h__

#include <SOP/SOP_Node.h>
#include "lightGrid.h"

#define INT_PARM(name, idx, vidx, t)	\
	    return evalInt(name, &myOffsets[idx], vidx, t);
//
//#define FLT_PARM(name, idx, vidx, t)	\
//	    return evalFloat(name, &myOffsets[idx], vidx, t);

#define 	FLT_PARM(name, vi, t)   { return evalFloat(name, vi, t); }

class GEO_ParticleVertex;
class GEO_PrimParticle;
class GU_RayIntersect;

class SOP_LitzupNode : public SOP_Node
{
public:
	     SOP_LitzupNode(OP_Network *net, const char *name, OP_Operator *op);
    virtual ~SOP_LitzupNode();

    static PRM_Template		 myTemplateList[];
    static OP_Node		*myConstructor(OP_Network*, const char *,
							    OP_Operator *);

	void		buildLGH(const GU_Detail *mySource, fpreal currframe, OP_Context &context);

	OP_Node *	createCopyAndLightNode(OP_Context & context, int level, float radius);
	void		updateNodeParameter(OP_Context & context, fpreal cellSize, fpreal alpha);
	int			maxLevel;
	fpreal		m_cellsize;
	fpreal		m_alpha;

protected:
    virtual const char          *inputLabel(unsigned idx) const;

    void		birthParticle();
	void		birthParticle(UT_Vector3 pos, UT_Vector3 col, int level);
	void		birthParticles1(std::vector<UT_Vector3> &pos, std::vector<UT_Vector3> &col, int numLights, int level);

    int			moveParticle(GA_Offset ptoff,
				     const UT_Vector3 &force);

    void		initSystem();
    void		timeStep(fpreal now);

    // Method to cook geometry for the SOP
    virtual OP_ERROR		 cookMySop(OP_Context &context) override;

	std::vector<OP_Node*> copy_nodes;

	bool	missingNode(int numLevels);

private:
    // These use defines to make it easy to add parms and remove them.
    // The evaluation routines use the indexed name lookup which is quite
    //	fast, yet easy to change indices (since the order of the indices
    //	doesn't have to be in sequential order...
    int			 ALPHA()	{	  INT_PARM("alpha", 0, 0, 0) }

	/*fpreal		 CELL_SIZE() { FLT_PARM("cell_size", 2, 0, 0) }*/

	fpreal		 CELL_SIZE(fpreal t) { FLT_PARM("cell_size", 0, t) }

    const GU_Detail	*mySource;
    GA_Index		 mySourceNum;		// Source point to birth from
    GA_ROHandleV3	 mySourceVel;		// Velocity attrib in source
	GA_ROHandleV3	 mySourceCol;		// Color attrib in source


    GEO_PrimParticle	*mySystem;
    fpreal		 myLastCookTime;	// Last cooked time
    GA_RWHandleV3	 myVelocity;		// My velocity attribute

	GA_RWHandleV3	 myColor;		// My color attribute
	GA_RWHandleF	 myLevel;		// My Level attribute

    static int		*myOffsets;


};


#endif
