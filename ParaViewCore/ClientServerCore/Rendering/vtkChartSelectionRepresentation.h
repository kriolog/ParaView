/*=========================================================================

  Program:   ParaView
  Module:    vtkChartSelectionRepresentation.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkChartSelectionRepresentation - representation for showing selections
// in chart views.
// .SECTION Description
// vtkChartSelectionRepresentation is a representation showing selections in
// chart views. It works with vtkChartSelectionRepresentation (namely
// vtkChartSelectionRepresentation::MapSelectionToView) to map the input
// vtkSelection to selection form that the view can understand and then passes
// it alone to the vtkPVContextView.
// For client-server cases, this class also handles delivering of the selection
// to all "rendering" nodes.

#ifndef vtkChartSelectionRepresentation_h
#define vtkChartSelectionRepresentation_h

#include "vtkPVClientServerCoreRenderingModule.h" //needed for exports
#include "vtkPVDataRepresentation.h"
#include "vtkWeakPointer.h" // needed for vtkWeakPointer.
#include "vtkSmartPointer.h" // needed for vtkWeakPointer.

class vtkChartRepresentation;
class vtkPVContextView;

class VTKPVCLIENTSERVERCORERENDERING_EXPORT vtkChartSelectionRepresentation : public vtkPVDataRepresentation
{
public:
  static vtkChartSelectionRepresentation* New();
  vtkTypeMacro(vtkChartSelectionRepresentation, vtkPVDataRepresentation);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // This needs to be called on all instances of vtkChartSelectionRepresentation when
  // the input is modified.
  virtual void MarkModified()
    { this->Superclass::MarkModified(); }

  // Description:
  // Set visibility of the representation.
  virtual void SetVisibility(bool visible);

  // Description:
  // Get/Set the chart representation for which this is a selection
  // representation. This is not reference counted.
  void SetChartRepresentation(vtkChartRepresentation* repr);

protected:
  vtkChartSelectionRepresentation();
  ~vtkChartSelectionRepresentation();

  virtual int FillInputPortInformation(int port, vtkInformation* info);
  virtual int RequestData(vtkInformation*,
    vtkInformationVector** inputVector, vtkInformationVector*);

  // Description:
  // Adds the representation to the view.  This is called from
  // vtkView::AddRepresentation().  Subclasses should override this method.
  // Returns true if the addition succeeds.
  virtual bool AddToView(vtkView* view);

  // Description:
  // Removes the representation to the view.  This is called from
  // vtkView::RemoveRepresentation().  Subclasses should override this method.
  // Returns true if the removal succeeds.
  virtual bool RemoveFromView(vtkView* view);

  vtkWeakPointer<vtkPVContextView> ContextView;
  vtkWeakPointer<vtkChartRepresentation> ChartRepresentation;

  // Flag updated in AddToView to indicate if the configuration requires
  // server-side rendering.
  bool EnableServerSideRendering;

private:
  vtkChartSelectionRepresentation(const vtkChartSelectionRepresentation&) VTK_DELETE_FUNCTION;
  void operator=(const vtkChartSelectionRepresentation&) VTK_DELETE_FUNCTION;

};

#endif
