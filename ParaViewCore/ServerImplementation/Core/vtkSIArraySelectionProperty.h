/*=========================================================================

  Program:   ParaView
  Module:    vtkSIArraySelectionProperty.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSIArraySelectionProperty - InformationOnly property
// .SECTION Description
// SIProperty that deals with ArraySelection object

#ifndef vtkSIArraySelectionProperty_h
#define vtkSIArraySelectionProperty_h

#include "vtkPVServerImplementationCoreModule.h" //needed for exports
#include "vtkSIProperty.h"

class VTKPVSERVERIMPLEMENTATIONCORE_EXPORT vtkSIArraySelectionProperty : public vtkSIProperty
{
public:
  static vtkSIArraySelectionProperty* New();
  vtkTypeMacro(vtkSIArraySelectionProperty, vtkSIProperty);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  vtkSIArraySelectionProperty();
  ~vtkSIArraySelectionProperty();

  friend class vtkSIProxy;

  // Description:
  // Pull the current state of the underneath implementation
  virtual bool Pull(vtkSMMessage*);

private:
  vtkSIArraySelectionProperty(const vtkSIArraySelectionProperty&) VTK_DELETE_FUNCTION;
  void operator=(const vtkSIArraySelectionProperty&) VTK_DELETE_FUNCTION;

};

#endif
