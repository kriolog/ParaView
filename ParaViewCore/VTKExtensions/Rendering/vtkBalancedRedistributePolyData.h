/*=========================================================================

  Program:   ParaView
  Module:    vtkBalancedRedistributePolyData.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*----------------------------------------------------------------------------
 Copyright (c) Los Alamos National Laboratory
 See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.
----------------------------------------------------------------------------*/

// .NAME vtkBalancedRedistributePolyData - do balance of cells on processors

#ifndef vtkBalancedRedistributePolyData_h
#define vtkBalancedRedistributePolyData_h

#include "vtkWeightedRedistributePolyData.h"
#include "vtkPVVTKExtensionsRenderingModule.h" // needed for export macro
class vtkMultiProcessController;


//*******************************************************************

class VTKPVVTKEXTENSIONSRENDERING_EXPORT vtkBalancedRedistributePolyData : public vtkWeightedRedistributePolyData
{
public:
  vtkTypeMacro(vtkBalancedRedistributePolyData, vtkWeightedRedistributePolyData);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  static vtkBalancedRedistributePolyData *New();

protected:
  vtkBalancedRedistributePolyData();
  ~vtkBalancedRedistributePolyData();
  void MakeSchedule (vtkPolyData*, vtkCommSched*);

private:
  vtkBalancedRedistributePolyData(const vtkBalancedRedistributePolyData&) VTK_DELETE_FUNCTION;
  void operator=(const vtkBalancedRedistributePolyData&) VTK_DELETE_FUNCTION;
};

//****************************************************************

#endif


