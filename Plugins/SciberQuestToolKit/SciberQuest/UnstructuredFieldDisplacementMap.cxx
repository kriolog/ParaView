/*
 * Copyright 2012 SciberQuest Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of SciberQuest Inc. nor the names of any contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "UnstructuredFieldDisplacementMap.h"

#include "IdBlock.h"
#include "FieldLine.h"
#include "TerminationCondition.h"
#include "vtkSQCellGenerator.h"
#include "vtkDataSet.h"
#include "vtkPoints.h"
#include "vtkUnstructuredGrid.h"
#include "vtkFloatArray.h"
#include "vtkCellArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkIdTypeArray.h"

//-----------------------------------------------------------------------------
UnstructuredFieldDisplacementMap::~UnstructuredFieldDisplacementMap()
{
  this->ClearSource();
  this->ClearOut();
}

//-----------------------------------------------------------------------------
void UnstructuredFieldDisplacementMap::ClearSource()
{
  if (this->SourceGen){ this->SourceGen->Delete(); }
  if (this->SourcePts){ this->SourcePts->Delete(); }
  if (this->SourceCells){ this->SourceCells->Delete(); }
  if (this->SourceTypes){ this->SourceTypes->Delete(); }
  this->SourceGen=0;
  this->SourcePts=0;
  this->SourceCells=0;
  this->SourceTypes=0;
  this->IdMap.clear();
}

//-----------------------------------------------------------------------------
void UnstructuredFieldDisplacementMap::ClearOut()
{
  if (this->OutPts){ this->OutPts->Delete(); }
  if (this->OutCells){ this->OutCells->Delete(); }
  if (this->OutTypes){ this->OutTypes->Delete(); }
  if (this->OutLocs){ this->OutLocs->Delete(); }
  this->OutPts=0;
  this->OutCells=0;
  this->OutTypes=0;
  this->OutLocs=0;
  this->IdMap.clear();
}

//-----------------------------------------------------------------------------
void UnstructuredFieldDisplacementMap::SetSource(vtkSQCellGenerator *sourceGen)
{
  if (this->SourceGen==sourceGen){ return; }
  if (this->SourceGen){ this->SourceGen->Delete(); }
  this->SourceGen=sourceGen;
  if (this->SourceGen){ this->SourceGen->Register(0); }
}

//-----------------------------------------------------------------------------
void UnstructuredFieldDisplacementMap::SetSource(vtkDataSet *s)
{
  this->ClearSource();

  vtkUnstructuredGrid *source=dynamic_cast<vtkUnstructuredGrid*>(s);
  if (source==0)
    {
    std::cerr << "Error: Source must be unstructured. " << s->GetClassName() << std::endl;
    return;
    }

  this->SourcePts=dynamic_cast<vtkFloatArray*>(source->GetPoints()->GetData());
  if (this->SourcePts==0)
    {
    std::cerr << "Error: Points are not float precision." << std::endl;
    return;
    }
  this->SourcePts->Register(0);

  this->SourceCells=source->GetCells();
  this->SourceCells->Register(0);

  this->SourceTypes=source->GetCellTypesArray();
  this->SourceTypes->Register(0);
}

//-----------------------------------------------------------------------------
void UnstructuredFieldDisplacementMap::SetOutput(vtkDataSet *o)
{
  this->FieldDisplacementMapData::SetOutput(o);

  this->ClearOut();

  vtkUnstructuredGrid *out=dynamic_cast<vtkUnstructuredGrid*>(o);
  if (out==0)
    {
    std::cerr << "Error: Out must be unstructured grid. " << o->GetClassName() << std::endl;
    return;
    }

  vtkPoints *opts=vtkPoints::New();
  out->SetPoints(opts);
  opts->Delete();

  this->OutPts=dynamic_cast<vtkFloatArray*>(opts->GetData());
  this->OutPts->Register(0);

  this->OutCells=vtkCellArray::New();
  this->OutTypes=vtkUnsignedCharArray::New();
  this->OutLocs=vtkIdTypeArray::New();

  out->SetCells(this->OutTypes,this->OutLocs,this->OutCells);
}

//-----------------------------------------------------------------------------
vtkIdType UnstructuredFieldDisplacementMap::InsertCells(IdBlock *SourceIds)
{
  if (this->SourceGen)
    {
    return this->InsertCellsFromGenerator(SourceIds);
    }
  return this->InsertCellsFromDataset(SourceIds);
}

//-----------------------------------------------------------------------------
vtkIdType UnstructuredFieldDisplacementMap::InsertCellsFromGenerator(IdBlock *SourceIds)
{
  vtkIdType startCellId=SourceIds->first();
  vtkIdType nCellsLocal=SourceIds->size();

//   // input points
//   float *pSourcePts=this->SourcePts->GetPointer(0);
//   unsigned char *pSourceTypes=this->SourceTypes->GetPointer(0);

  // output points
  vtkIdType nOutPts=this->OutPts->GetNumberOfTuples();

  // output cells
  vtkIdTypeArray *outCells=this->OutCells->GetData();
  vtkIdType nCellIds=outCells->GetNumberOfTuples();
  vtkIdType nOutCells=this->OutCells->GetNumberOfCells();
  this->OutCells->SetNumberOfCells(nOutCells+nCellsLocal);

  // output cell types
  vtkIdType endOfTypes=this->OutTypes->GetNumberOfTuples();
  unsigned char *pOutTypes=this->OutTypes->WritePointer(endOfTypes,nCellsLocal);

  // output cell locations
  vtkIdType endOfLocs=this->OutLocs->GetNumberOfTuples();
  vtkIdType *pOutLocs=this->OutLocs->WritePointer(endOfLocs,nCellsLocal);

  std::vector<float> sourcePts;
  std::vector<vtkIdType> sourceIds;

  vtkIdType sourceCellId=startCellId;

  // For each cell asigned to us we'll get its center to use as a
  // seed point and copy the cell into the output, The output only
  // will have the cells assigned to this process, while the input
  // may have all of the cells.
  for (vtkIdType i=0; i<nCellsLocal; ++i)
    {
    // get the cell that belong to us.
    vtkIdType nPtIds=this->SourceGen->GetNumberOfCellPoints(sourceCellId);
    sourcePts.resize(3*nPtIds);
    sourceIds.resize(nPtIds);

    this->SourceGen->GetCellPointIndexes(sourceCellId,&sourceIds[0]);
    this->SourceGen->GetCellPoints(sourceCellId,&sourcePts[0]);

    // set the new cell's location
    *pOutLocs=nCellIds;
    ++pOutLocs;

    // copy its type.
    *pOutTypes=(unsigned char)this->SourceGen->GetCellType(sourceCellId);
    ++pOutTypes;

    // Get location to write new cell.
    vtkIdType *pOutCells=outCells->WritePointer(nCellIds,nPtIds+1);

    // update next cell write location.
    nCellIds+=nPtIds+1;

    // number of points in this cell
    *pOutCells=nPtIds;
    ++pOutCells;

    // Get location to write new point. assumes we need to copy all
    // but this is wrong as there will be many duplicates. ignored.
    float *pOutPts=this->OutPts->WritePointer(3*nOutPts,3*nPtIds);

    // transfer from input to output (only what we own)
    for (vtkIdType j=0; j<nPtIds; ++j)
      {
      vtkIdType idx=3*j;
      // do we already have this point?
      MapElement elem(sourceIds[j],nOutPts);
      MapInsert ret=this->IdMap.insert(elem);
      if (ret.second==true)
        {
        // this point hasn't previsouly been coppied
        // copy the point.
        pOutPts[0]=sourcePts[idx  ];
        pOutPts[1]=sourcePts[idx+1];
        pOutPts[2]=sourcePts[idx+2];

        FieldLine *line=new FieldLine(pOutPts,nOutPts);
        line->AllocateTrace();
        this->Lines.push_back(line);

        pOutPts+=3;
        nOutPts+=1;
        }

      // insert the point id.
      *pOutCells=(*ret.first).second;
      ++pOutCells;
      }

    ++sourceCellId;
    }

  // correct the length of the point array, above we assumed
  // that all points from each cell needed to be inserted
  // and allocated that much space.
  this->OutPts->Resize(nOutPts);

  return (vtkIdType)this->Lines.size();
}

//-----------------------------------------------------------------------------
vtkIdType UnstructuredFieldDisplacementMap::InsertCellsFromDataset(IdBlock *SourceIds)
{
  vtkIdType startCellId=SourceIds->first();
  vtkIdType nCellsLocal=SourceIds->size();

  // Cells are sequentially acccessed so explicitly skip all cells
  // we aren't interested in.
  this->SourceCells->InitTraversal();
  for (vtkIdType i=0; i<startCellId; ++i)
    {
    vtkIdType n;
    vtkIdType *ptIds;
    this->SourceCells->GetNextCell(n,ptIds);
    }

  // input points
  float *pSourcePts=this->SourcePts->GetPointer(0);
  unsigned char *pSourceTypes=this->SourceTypes->GetPointer(0);

  // output points
  vtkIdType nOutPts=this->OutPts->GetNumberOfTuples();

  // output cells
  vtkIdTypeArray *outCells=this->OutCells->GetData();
  vtkIdType nCellIds=outCells->GetNumberOfTuples();
  vtkIdType nOutCells=this->OutCells->GetNumberOfCells();
  this->OutCells->SetNumberOfCells(nOutCells+nCellsLocal);

  // output cell types
  vtkIdType endOfTypes=this->OutTypes->GetNumberOfTuples();
  unsigned char *pOutTypes=this->OutTypes->WritePointer(endOfTypes,nCellsLocal);

  // output cell locations
  vtkIdType endOfLocs=this->OutLocs->GetNumberOfTuples();
  vtkIdType *pOutLocs=this->OutLocs->WritePointer(endOfLocs,nCellsLocal);

  vtkIdType sourceCellId=startCellId;

  // For each cell asigned to us we'll get its center to use as a
  // seed point and copy the cell into the output, The output only
  // will have the cells assigned to this process, while the input
  // may have all of the cells.
  for (vtkIdType i=0; i<nCellsLocal; ++i)
    {
    // get the cell that belong to us.
    vtkIdType nPtIds=0;
    vtkIdType *ptIds=0;
    this->SourceCells->GetNextCell(nPtIds,ptIds);

    // set the new cell's location
    *pOutLocs=nCellIds;
    ++pOutLocs;

    // copy its type.
    *pOutTypes=pSourceTypes[sourceCellId];
    ++pOutTypes;

    // Get location to write new cell.
    vtkIdType *pOutCells=outCells->WritePointer(nCellIds,nPtIds+1);

    // update next cell write location.
    nCellIds+=nPtIds+1;

    // number of points in this cell
    *pOutCells=nPtIds;
    ++pOutCells;

    // Get location to write new point. assumes we need to copy all
    // but this is wrong as there will be many duplicates. ignored.
    float *pOutPts=this->OutPts->WritePointer(3*nOutPts,3*nPtIds);

    // transfer from input to output (only what we own)
    for (vtkIdType j=0; j<nPtIds; ++j)
      {
      vtkIdType idx=3*ptIds[j];
      // do we already have this point?
      MapElement elem(ptIds[j],nOutPts);
      MapInsert ret=this->IdMap.insert(elem);
      if (ret.second==true)
        {
        // this point hasn't previsouly been coppied
        // copy the point.
        pOutPts[0]=pSourcePts[idx  ];
        pOutPts[1]=pSourcePts[idx+1];
        pOutPts[2]=pSourcePts[idx+2];

        // insert the new point id.
        *pOutCells=nOutPts;

        FieldLine *line=new FieldLine(pOutPts,nOutPts);
        line->AllocateTrace();
        this->Lines.push_back(line);

        pOutPts+=3;
        nOutPts+=1;
        }
      else
        {
        // this point has been coppied, do not add a duplicate.
        // insert the other point id.
        *pOutCells=(*ret.first).second;
        }

      ++pOutCells;
      }

    ++sourceCellId;
    }

  // correct the length of the point array, above we assumed
  // that all points from each cell needed to be inserted
  // and allocated that much space.
  this->OutPts->Resize(nOutPts);

  return (vtkIdType)this->Lines.size();
}
