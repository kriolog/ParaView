/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkKWTextProperty.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2000-2001 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither the name of Kitware nor the names of any contributors may be used
   to endorse or promote products derived from this software without specific 
   prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkKWTextProperty.h"

#include "vtkActor2D.h"
#include "vtkKWApplication.h"
#include "vtkKWChangeColorButton.h"
#include "vtkKWCheckButton.h"
#include "vtkKWCheckButtonSet.h"
#include "vtkKWLabel.h"
#include "vtkKWLabeledCheckButtonSet.h"
#include "vtkKWLabeledOptionMenu.h"
#include "vtkKWLabeledPushButtonSet.h"
#include "vtkKWOptionMenu.h"
#include "vtkKWPushButton.h"
#include "vtkKWPushButtonSet.h"
#include "vtkKWScale.h"
#include "vtkKWTkUtilities.h"
#include "vtkObjectFactory.h"
#include "vtkProperty2D.h"
#include "vtkTextProperty.h"

#define VTK_KW_TEXT_PROP_ARIAL   "Arial"
#define VTK_KW_TEXT_PROP_COURIER "Courier"
#define VTK_KW_TEXT_PROP_TIMES   "Times"

#define VTK_KW_TEXT_PROP_BOLD_ID   0
#define VTK_KW_TEXT_PROP_ITALIC_ID 1
#define VTK_KW_TEXT_PROP_SHADOW_ID 2

#define VTK_KW_TEXT_PROP_COPY_ID 0

/* 
 * This part was generated by ImageConvert from image:
 *    bold.png (zlib, base64)
 */
#define image_bold_width         12
#define image_bold_height        16
#define image_bold_pixel_size    4
#define image_bold_buffer_length 152

static unsigned char image_bold[] = 
  "eNr7//8/w38aYiBQAuI0IH4HxP+huByIZ0LFzkD56GA3knoYMEYzg5B6ZPF3NFCP7J40PO"
  "ph/gWx7wKxCxH+VULS846A+cgAOZxdSPAvepjiUn8Gh58Jhc8ZJH+hp4dVQNyBlh4EQYr/"
  "0zh9AgBC7tWi";

/* 
 * This part was generated by ImageConvert from image:
 *    italic.png (zlib, base64)
 */
#define image_italic_width         12
#define image_italic_height        16
#define image_italic_pixel_size    4
#define image_italic_buffer_length 140

static unsigned char image_italic[] = 
  "eNr7//8/w38aYyzABYjvAjFIsgOIlQioB4HdUD1gQIT6d0A8k0j1LlC3hBKpvgOqXpBI9W"
  "eg7mcgQr0S1OxyItWnQdUrEal+FXI4EqH+HdS/xKgPRQ9HHOoFoWrOIKcBXOppiQHpUhBU";

/* 
 * This part was generated by ImageConvert from image:
 *    shadow.png (zlib, base64)
 */
#define image_shadow_width         12
#define image_shadow_height        16
#define image_shadow_pixel_size    4
#define image_shadow_buffer_length 384

static unsigned char image_shadow[] = 
  "eNq10s8rRFEYxvFhRmZIzSATXSnNlR/JwsiMsiALyo+sxFqUmhVFdjaSBWv/go2VlY38Bf"
  "4DS6uJsrC8vk89i9PZO/Xpdmeec977vvdmWZbL/pnXOR58fcQN1nLBCvJHznShH8N4wz6q"
  "6EM+yL/gFgmaWMIFTl1jEqUgr7O//P8u5jCCA2xiQTWC/Dy+9Yh4Rl31Ucasz++N+t3Ah/"
  "f84EQZ7xlAd5RXT8t4wq/3XWMIRXQG+XHdYxAruMQn2tjBKApBXjPv8DxVfwZ3rnGFLT1T"
  "NE/NseLaBc/q1XNtIY3m+Y577OHYZ0z4Xr1PB3mdO4ZDnHlWZfe17R6SaD76Btb9vhruYR"
  "GrmIrer1bJ30OKmq+pf+vRPJT9A5tnxYs=";

/* 
 * This part was generated by ImageConvert from image:
 *    copy.png (zlib, base64)
 */
#define image_copy_width         15
#define image_copy_height        16
#define image_copy_pixel_size    4
#define image_copy_buffer_length 124

static unsigned char image_copy[] = 
  "eNqlkwkKACAIBHu6P9+CLjNlpYSlCKZJJAAFj2kFL1nWVpaPWLVSVrnGXtwwb+fFPdO89e"
  "5IeN/kmffsxfaQ997vz3nN7Bfrh3vj2XEvY1+9LPj4Rz+pVM55tQ==";

// ----------------------------------------------------------------------------
vtkStandardNewMacro(vtkKWTextProperty);
vtkCxxRevisionMacro(vtkKWTextProperty, "1.16");

int vtkKWTextPropertyCommand(ClientData cd, Tcl_Interp *interp,
                      int argc, char *argv[]);

// ----------------------------------------------------------------------------
vtkKWTextProperty::vtkKWTextProperty()
{
  this->TextProperty = NULL;
  this->Actor2D = NULL;

  this->LongFormat = 0;
  this->LabelOnTop = 1;

  this->ShowLabel = 0;
  this->Label = vtkKWLabel::New();

  this->ShowColor = 1;
  this->ChangeColorButton = vtkKWChangeColorButton::New();

  this->ShowFontFamily = 1;
  this->FontFamilyOptionMenu = vtkKWLabeledOptionMenu::New();

  this->ShowStyles = 1;
  this->StylesCheckButtonSet = vtkKWLabeledCheckButtonSet::New();

  this->ShowOpacity = 1;
  this->OpacityScale = vtkKWScale::New();

  this->OnChangeCommand = NULL;
  this->OnColorChangeCommand = NULL;

  this->ShowCopy = 0;
  this->PushButtonSet = vtkKWLabeledPushButtonSet::New();
}

// ----------------------------------------------------------------------------
vtkKWTextProperty::~vtkKWTextProperty()
{
  this->SetTextProperty(NULL);
  this->SetActor2D(NULL);

  this->SetOnChangeCommand(NULL);
  this->SetOnColorChangeCommand(NULL);

  if (this->Label)
    {
    this->Label->Delete();
    this->Label = NULL;
    }

  if (this->ChangeColorButton)
    {
    this->ChangeColorButton->Delete();
    this->ChangeColorButton = NULL;
    }

  if (this->FontFamilyOptionMenu)
    {
    this->FontFamilyOptionMenu->Delete();
    this->FontFamilyOptionMenu = NULL;
    }

  if (this->StylesCheckButtonSet)
    {
    this->StylesCheckButtonSet->Delete();
    this->StylesCheckButtonSet = NULL;
    }

  if (this->OpacityScale)
    {
    this->OpacityScale->Delete();
    this->OpacityScale = NULL;
    }

  if (this->PushButtonSet)
    {
    this->PushButtonSet->Delete();
    this->PushButtonSet = NULL;
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::Create(vtkKWApplication *app)
{
  // Set the application

  if (this->IsCreated())
    {
    vtkErrorMacro("vtkKWTextProperty already created");
    return;
    }
  this->SetApplication(app);

  // Create container

  this->Script("frame %s -borderwidth 0 -relief flat", this->GetWidgetName());

  // Label

  this->Label->SetParent(this);
  this->Label->Create(this->Application, "");

  // Color

  this->ChangeColorButton->SetParent(this);
  this->ChangeColorButton->LabelOutsideButtonOn();
  this->ChangeColorButton->Create(this->Application, "");
  this->ChangeColorButton->SetCommand(this, "ChangeColorButtonCallback");
  this->ChangeColorButton->SetBalloonHelpString("Select the text color.");

  // Font Family

  this->FontFamilyOptionMenu->SetParent(this);
  this->FontFamilyOptionMenu->Create(this->Application);
  this->Script("%s config -width 7 -padx 0 -pady 2", 
               this->FontFamilyOptionMenu->GetOptionMenu()->GetWidgetName());
  this->FontFamilyOptionMenu->SetBalloonHelpString("Select the font family.");

  this->FontFamilyOptionMenu->GetOptionMenu()->AddEntryWithCommand(
    VTK_KW_TEXT_PROP_ARIAL, this, "FontFamilyCallback");

  this->FontFamilyOptionMenu->GetOptionMenu()->AddEntryWithCommand(
    VTK_KW_TEXT_PROP_COURIER, this, "FontFamilyCallback");

  this->FontFamilyOptionMenu->GetOptionMenu()->AddEntryWithCommand(
    VTK_KW_TEXT_PROP_TIMES, this, "FontFamilyCallback");

  // Styles (bold, italic, shadow)

  this->StylesCheckButtonSet->SetParent(this);
  this->StylesCheckButtonSet->PackHorizontallyOn();
  this->StylesCheckButtonSet->Create(this->Application);
  this->StylesCheckButtonSet->SetBalloonHelpString("Select the font styles.");

  vtkKWCheckButtonSet *cbs = this->StylesCheckButtonSet->GetCheckButtonSet();
  cbs->PackHorizontallyOn();

  vtkKWCheckButton *cb;
  const char *styles_options = "-indicator 0 -padx 0 -pady 0";

  cbs->AddButton(VTK_KW_TEXT_PROP_BOLD_ID, 
                 0,
                 this, "BoldCallback",
                 "Select bold style.");
  cb = cbs->GetButton(VTK_KW_TEXT_PROP_BOLD_ID);
  this->Script("%s configure %s", cb->GetWidgetName(), styles_options);

  ostrstream bold;
  bold << cb->GetWidgetName() << ".boldimg" << ends;
  if (!vtkKWTkUtilities::UpdatePhoto(this->Application->GetMainInterp(),
                                     bold.str(), 
                                     image_bold, 
                                     image_bold_width, 
                                     image_bold_height, 
                                     image_bold_pixel_size,
                                     image_bold_buffer_length,
                                     cb->GetWidgetName()))
    {
    vtkWarningMacro(<< "Error creating photo (bold)");
    cb->SetText("Bold");
    }
  else
    {
    this->Script("%s configure -image %s", cb->GetWidgetName(), bold.str());
    }
  bold.rdbuf()->freeze(0);
  ostrstream bold_s;
  bold_s << cb->GetWidgetName() << ".bold_simg" << ends;
  if (!vtkKWTkUtilities::UpdatePhoto(this->Application->GetMainInterp(),
                                     bold_s.str(), 
                                     image_bold, 
                                     image_bold_width, 
                                     image_bold_height, 
                                     image_bold_pixel_size,
                                     image_bold_buffer_length,
                                     cb->GetWidgetName(),
                                     "-selectcolor"))
    {
    vtkWarningMacro(<< "Error creating photo (bold)");
    }
  else
    {
    this->Script("%s configure -selectimage %s", 
                 cb->GetWidgetName(), bold_s.str());
    }
  bold_s.rdbuf()->freeze(0);

  cbs->AddButton(VTK_KW_TEXT_PROP_ITALIC_ID, 
                 0, 
                 this, "ItalicCallback",
                 "Select italic style.");
  cb = cbs->GetButton(VTK_KW_TEXT_PROP_ITALIC_ID);
  this->Script("%s configure %s", cb->GetWidgetName(), styles_options);

  ostrstream italic;
  italic << cb->GetWidgetName() << ".italicimg" << ends;
  if (!vtkKWTkUtilities::UpdatePhoto(this->Application->GetMainInterp(),
                                     italic.str(), 
                                     image_italic, 
                                     image_italic_width, 
                                     image_italic_height, 
                                     image_italic_pixel_size,
                                     image_italic_buffer_length,
                                     cb->GetWidgetName()))
    {
    vtkWarningMacro(<< "Error creating photo (italic)");
    cb->SetText("Italic");
    }
  else
    {
    this->Script("%s configure -image %s", 
                 cb->GetWidgetName(), italic.str());
    }
  italic.rdbuf()->freeze(0);
  ostrstream italic_s;
  italic_s << cb->GetWidgetName() 
           << ".italic_simg" << ends;
  if (!vtkKWTkUtilities::UpdatePhoto(this->Application->GetMainInterp(),
                                     italic_s.str(), 
                                     image_italic, 
                                     image_italic_width, 
                                     image_italic_height, 
                                     image_italic_pixel_size,
                                     image_italic_buffer_length,
                                     cb->GetWidgetName(),
                                     "-selectcolor"))
    {
    vtkWarningMacro(<< "Error creating photo (italic)");
    }
  else
    {
    this->Script("%s configure -selectimage %s", 
                 cb->GetWidgetName(), italic_s.str());
    }
  italic_s.rdbuf()->freeze(0);

  cbs->AddButton(VTK_KW_TEXT_PROP_SHADOW_ID, 
                 0, 
                 this, "ShadowCallback",
                 "Select shadow style.");
  cb = cbs->GetButton(VTK_KW_TEXT_PROP_SHADOW_ID);
  this->Script("%s configure %s", cb->GetWidgetName(), styles_options);

  ostrstream shadow;
  shadow << cb->GetWidgetName() << ".shadowimg" << ends;
  if (!vtkKWTkUtilities::UpdatePhoto(this->Application->GetMainInterp(),
                                     shadow.str(), 
                                     image_shadow, 
                                     image_shadow_width, 
                                     image_shadow_height, 
                                     image_shadow_pixel_size,
                                     image_shadow_buffer_length,
                                     cb->GetWidgetName()))
    {
    vtkWarningMacro(<< "Error creating photo (shadow)");
    cb->SetText("Shadow");
    }
  else
    {
    this->Script("%s configure -image %s", 
                 cb->GetWidgetName(), shadow.str());
    }
  shadow.rdbuf()->freeze(0);
  ostrstream shadow_s;
  shadow_s << cb->GetWidgetName() 
           << ".shadow_simg" << ends;
  if (!vtkKWTkUtilities::UpdatePhoto(this->Application->GetMainInterp(),
                                     shadow_s.str(), 
                                     image_shadow, 
                                     image_shadow_width, 
                                     image_shadow_height, 
                                     image_shadow_pixel_size,
                                     image_shadow_buffer_length,
                                     cb->GetWidgetName(),
                                     "-selectcolor"))
    {
    vtkWarningMacro(<< "Error creating photo (shadow)");
    }
  else
    {
    this->Script("%s configure -selectimage %s", 
                 cb->GetWidgetName(), shadow_s.str());
    }
  shadow_s.rdbuf()->freeze(0);

  // Opacity

  this->OpacityScale->SetParent(this);
  this->OpacityScale->PopupScaleOn();
  this->OpacityScale->SetResolution(0.01);
  this->OpacityScale->SetRange(0.0, 1.0);
  this->OpacityScale->Create(this->Application, "");
  this->OpacityScale->DisplayEntry();
  this->OpacityScale->DisplayLabel("");
  this->OpacityScale->SetEntryWidth(4);
  this->OpacityScale->DisplayEntryAndLabelOnTopOff();
  this->OpacityScale->SetCommand(this, "OpacityCallback");
  this->OpacityScale->SetEndCommand(this, "OpacityEndCallback");
  this->OpacityScale->SetEntryCommand(this, "OpacityEndCallback");
  this->OpacityScale->SetBalloonHelpString("Select the text opacity.");

  // Buttons

  this->PushButtonSet->SetParent(this);
  this->PushButtonSet->PackHorizontallyOn();
  this->PushButtonSet->Create(this->Application);

  vtkKWPushButtonSet *pbs = this->PushButtonSet->GetPushButtonSet();
  pbs->PackHorizontallyOn();

  vtkKWPushButton *pb;

  // Buttons : copy button

  pbs->AddButton(VTK_KW_TEXT_PROP_COPY_ID);
  pb = pbs->GetButton(VTK_KW_TEXT_PROP_COPY_ID);

  ostrstream copy;
  copy << pb->GetWidgetName() << ".copyimg" << ends;
  if (!vtkKWTkUtilities::UpdatePhoto(this->Application->GetMainInterp(),
                                     copy.str(), 
                                     image_copy, 
                                     image_copy_width, 
                                     image_copy_height, 
                                     image_copy_pixel_size,
                                     image_copy_buffer_length,
                                     pb->GetWidgetName()))
    {
    vtkWarningMacro(<< "Error creating photo (copy)");
    pb->SetLabel("Copy");
    }
  else
    {
    this->Script("%s configure -image %s", 
                 pb->GetWidgetName(), copy.str());
    }
  copy.rdbuf()->freeze(0);

  // Pack

  this->Pack();

  // Update

  this->UpdateInterface();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::Pack()
{
  if (!this->IsCreated())
    {
    return;
    }

  ostrstream tk_cmd;

  this->ChangeColorButton->UnpackSiblings();

  if (this->LongFormat)
    {
    this->ChangeColorButton->GetLabel()->SetLabel("Color:");
    this->ChangeColorButton->ShowLabelOn();

    this->FontFamilyOptionMenu->GetOptionMenu()->IndicatorOn();
    this->FontFamilyOptionMenu->GetLabel()->SetLabel("Family:");
    this->FontFamilyOptionMenu->ShowLabelOn();

    this->StylesCheckButtonSet->GetLabel()->SetLabel("Styles:");
    this->StylesCheckButtonSet->ShowLabelOn();

    this->OpacityScale->DisplayLabel("Opacity:");

    this->PushButtonSet->GetLabel()->SetLabel("Functions:");
    this->PushButtonSet->ShowLabelOn();

    int row = 0, col = 0;

    tk_cmd << "grid " 
           << this->Label->GetWidgetName() 
           << " -sticky nsw -column " << col << " -row " << row << endl;

    if (this->LabelOnTop)
      {
      row++;
      }
    else
      {
      col = 1;
      }

    const char *options = " -sticky nsw -pady 1 -padx 10";
    
    tk_cmd << "grid " << this->ChangeColorButton->GetWidgetName() << options 
           << " -column " << col << " -row " << row++ << endl;
    tk_cmd << "grid " << this->FontFamilyOptionMenu->GetWidgetName() << options 
           << " -column " << col << " -row " << row++ << endl;
    tk_cmd << "grid " << this->StylesCheckButtonSet->GetWidgetName() << options 
           << " -column " << col << " -row " << row++ << endl;
    tk_cmd << "grid " << this->OpacityScale->GetWidgetName() << options 
             << " -column " << col << " -row " << row++ << endl;
    tk_cmd  << "grid " << this->PushButtonSet->GetWidgetName() << options 
           << " -column " << col << " -row " << row++ << endl;
    }
  else
    {
    this->ChangeColorButton->ShowLabelOff();

    this->FontFamilyOptionMenu->GetOptionMenu()->IndicatorOff();
    this->FontFamilyOptionMenu->ShowLabelOff();

    this->StylesCheckButtonSet->ShowLabelOff();

    this->OpacityScale->DisplayLabel("");

    this->PushButtonSet->ShowLabelOff();

    tk_cmd << "grid "
           << this->Label->GetWidgetName() << " "
           << this->ChangeColorButton->GetWidgetName() << " "
           << this->FontFamilyOptionMenu->GetWidgetName() << " "
           << this->StylesCheckButtonSet->GetWidgetName() << " "
           << this->OpacityScale->GetWidgetName() << " "
           << this->PushButtonSet->GetWidgetName()
           << " -row 0 -sticky news -padx 1" << endl;
    }

  tk_cmd << ends;
  this->Script(tk_cmd.str());
  tk_cmd.rdbuf()->freeze(0);

  // Synchronize the label width to align everything

  if (this->LongFormat)
    {
    int nb = 0;
    const char *labels[10];
    labels[nb++] = this->ChangeColorButton->GetLabel()->GetWidgetName();
    labels[nb++] = this->FontFamilyOptionMenu->GetLabel()->GetWidgetName();
    labels[nb++] = this->StylesCheckButtonSet->GetLabel()->GetWidgetName();
    labels[nb++] = this->OpacityScale->GetLabel()->GetWidgetName();
    labels[nb++] = this->PushButtonSet->GetLabel()->GetWidgetName();
    vtkKWTkUtilities::SynchroniseLabelsMaximumWidth(
      this->Application->GetMainInterp(), nb, labels, "-anchor w");
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::Update()
{
  this->UpdateInterface();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateInterface()
{
  // Update enable state

  this->UpdateEnableState();

  // Update all components

  this->UpdateLabel();
  this->UpdateColorButton();
  this->UpdateFontFamilyOptionMenu();
  this->UpdateStylesCheckButtonSet();
  this->UpdateOpacityScale();
  this->UpdatePushButtonSet();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetTextProperty(vtkTextProperty *_arg)
{
  if (this->TextProperty == _arg)
    {
    return;
    }

  this->TextProperty = _arg;
  
  if (this->TextProperty)
    {
    this->UpdateInterface();
    }

  this->Modified();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetActor2D(vtkActor2D *_arg)
{
  if (this->Actor2D == _arg)
    {
    return;
    }

  this->Actor2D = _arg;
  
  if (this->Actor2D)
    {
    this->UpdateInterface();
    }

  this->Modified();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetLongFormat(int _arg)
{
  if (this->LongFormat == _arg)
    {
    return;
    }
  this->LongFormat = _arg;
  this->Modified();

  this->Pack();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowLabel(int _arg)
{
  if (this->ShowLabel == _arg)
    {
    return;
    }
  this->ShowLabel = _arg;
  this->Modified();

  this->UpdateLabel();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetLabelOnTop(int _arg)
{
  if (this->LabelOnTop == _arg)
    {
    return;
    }
  this->LabelOnTop = _arg;
  this->Modified();

  this->Pack();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateLabel()
{
  if (this->IsCreated() && this->Label)
    {
    this->Script("grid %s %s",
                 (this->ShowLabel ? "" : "remove"), 
                 this->Label->GetWidgetName());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowColor(int _arg)
{
  if (this->ShowColor == _arg)
    {
    return;
    }
  this->ShowColor = _arg;
  this->Modified();

  this->UpdateColorButton();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetColor(float r, float g, float b) 
{
  float *rgb = this->GetColor();
  if (rgb[0] == r && rgb[1] == g && rgb[2] == b)
    {
    return;
    }

  if (this->TextProperty)
    {
    this->TextProperty->SetColor(r, g, b);
    }
  else if (this->Actor2D && this->Actor2D->GetProperty())
    {
    this->Actor2D->GetProperty()->SetColor(r, g, b);
    }

  this->UpdateColorButton();

  this->AddTraceEntry("$kw(%s) SetColor %f %f %f", this->GetTclName(), r,g,b);

  if (this->OnColorChangeCommand)
    {
    this->Script("eval %s", this->OnColorChangeCommand);
    }

  if (this->OnChangeCommand)
    {
    this->Script("eval %s", this->OnChangeCommand);
    }
}

// ----------------------------------------------------------------------------
float* vtkKWTextProperty::GetColor() 
{
  int use_actor_color = 0;
  if (!this->TextProperty)
    {
    use_actor_color = 1;
    }
  else
    {
    // This test is done to maintain backward compatibility (see
    // vtkOpenGL...TextMapper). The default vtkTextProperty color is
    // -1, -1, -1 so that the mappers know that they have to use
    // the actor's color instead.
    float *rgb = this->TextProperty->GetColor();
    if (rgb[0] < 0.0 && rgb[1] < 0.0 && rgb[2] < 0.0)
      {
      use_actor_color = 1;
      }
    }

  if (use_actor_color && this->Actor2D && this->Actor2D->GetProperty())
    {
    return this->Actor2D->GetProperty()->GetColor();
    }
  else if (this->TextProperty)
    {
    return this->TextProperty->GetColor();
    }
  return 0;
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateColorButton()
{
  if (this->IsCreated() && this->ChangeColorButton)
    {
    if (this->GetColor())
      {
      this->ChangeColorButton->SetColor(this->GetColor());
      }

    this->Script("grid %s %s",
                 (this->ShowColor ? "" : "remove"), 
                 this->ChangeColorButton->GetWidgetName());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::ChangeColorButtonCallback(float r, float g, float b) 
{
  this->SetColor(r, g, b);
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowFontFamily(int _arg)
{
  if (this->ShowFontFamily == _arg)
    {
    return;
    }
  this->ShowFontFamily = _arg;
  this->Modified();

  this->UpdateFontFamilyOptionMenu();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetFontFamily(int v) 
{
  if (this->TextProperty)
    {
    if (this->TextProperty->GetFontFamily() == v)
      {
      return;
      }
    this->TextProperty->SetFontFamily(v);
    }

  this->UpdateFontFamilyOptionMenu();

  this->AddTraceEntry("$kw(%s) SetFontFamily %d", this->GetTclName(), v);

  if (this->OnChangeCommand)
    {
    this->Script("eval %s", this->OnChangeCommand);
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateFontFamilyOptionMenu()
{
  if (this->IsCreated() &&
      this->FontFamilyOptionMenu &&
      this->TextProperty)
    {
    switch (this->TextProperty->GetFontFamily())
      {
      case VTK_ARIAL:
        this->FontFamilyOptionMenu->GetOptionMenu()->SetValue(
          VTK_KW_TEXT_PROP_ARIAL);
        break;
      case VTK_COURIER:
        this->FontFamilyOptionMenu->GetOptionMenu()->SetValue(
          VTK_KW_TEXT_PROP_COURIER);
        break;
      case VTK_TIMES:
        this->FontFamilyOptionMenu->GetOptionMenu()->SetValue(
          VTK_KW_TEXT_PROP_TIMES);
        break;
      }
    this->Script("grid %s %s",
                 (this->ShowFontFamily ? "" : "remove"), 
                 this->FontFamilyOptionMenu->GetWidgetName());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::FontFamilyCallback() 
{
  if (this->FontFamilyOptionMenu->IsCreated())
    {
    const char *value = this->FontFamilyOptionMenu->GetOptionMenu()->GetValue();
    if (!strcmp(value, VTK_KW_TEXT_PROP_ARIAL))
      {
      this->SetFontFamily(VTK_ARIAL);
      }
    else if (!strcmp(value, VTK_KW_TEXT_PROP_COURIER))
      {
      this->SetFontFamily(VTK_COURIER);
      }
    else if (!strcmp(value, VTK_KW_TEXT_PROP_TIMES))
      {
      this->SetFontFamily(VTK_TIMES);
      }
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowStyles(int _arg)
{
  if (this->ShowStyles == _arg)
    {
    return;
    }
  this->ShowStyles = _arg;
  this->Modified();

  this->UpdateStylesCheckButtonSet();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateStylesCheckButtonSet()
{
  if (this->IsCreated() && this->StylesCheckButtonSet)
    {
    this->Script("grid %s %s",
                 (this->ShowStyles ? "" : "remove"), 
                 this->StylesCheckButtonSet->GetWidgetName());
    }

  this->UpdateBoldCheckButton();
  this->UpdateItalicCheckButton();
  this->UpdateShadowCheckButton();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetBold(int v) 
{
  if (this->TextProperty)
    {
    if (this->TextProperty->GetBold() == v)
      {
      return;
      }
    this->TextProperty->SetBold(v);
    }

  this->UpdateBoldCheckButton();

  this->AddTraceEntry("$kw(%s) SetBold %d", this->GetTclName(), v);

  if (this->OnChangeCommand)
    {
    this->Script("eval %s", this->OnChangeCommand);
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateBoldCheckButton()
{
  if (this->IsCreated() && 
      this->StylesCheckButtonSet &&
      this->TextProperty)
    {
    this->StylesCheckButtonSet->GetCheckButtonSet()->SetButtonState(
      VTK_KW_TEXT_PROP_BOLD_ID, this->TextProperty->GetBold());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::BoldCallback() 
{
  if (this->IsCreated())
    {
    this->SetBold(this->StylesCheckButtonSet->GetCheckButtonSet()
                  ->IsButtonSelected(VTK_KW_TEXT_PROP_BOLD_ID));
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetItalic(int v) 
{
  if (this->TextProperty)
    {
    if (this->TextProperty->GetItalic() == v)
      {
      return;
      }
    this->TextProperty->SetItalic(v);
    }

  this->UpdateItalicCheckButton();

  this->AddTraceEntry("$kw(%s) SetItalic %d", this->GetTclName(), v);

  if (this->OnChangeCommand)
    {
    this->Script("eval %s", this->OnChangeCommand);
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateItalicCheckButton()
{
  if (this->IsCreated() && 
      this->StylesCheckButtonSet &&
      this->TextProperty)
    {
    this->StylesCheckButtonSet->GetCheckButtonSet()->SetButtonState(
      VTK_KW_TEXT_PROP_ITALIC_ID, this->TextProperty->GetItalic());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::ItalicCallback() 
{
  if (this->IsCreated())
    {
    this->SetItalic(this->StylesCheckButtonSet->GetCheckButtonSet()
                    ->IsButtonSelected(VTK_KW_TEXT_PROP_ITALIC_ID));
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShadow(int v) 
{
  if (this->TextProperty)
    {
    if (this->TextProperty->GetShadow() == v)
      {
      return;
      }
    this->TextProperty->SetShadow(v);
    }

  this->UpdateShadowCheckButton();

  this->AddTraceEntry("$kw(%s) SetShadow %d", this->GetTclName(), v);

  if (this->OnChangeCommand)
    {
    this->Script("eval %s", this->OnChangeCommand);
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateShadowCheckButton()
{
  if (this->IsCreated() && 
      this->StylesCheckButtonSet &&
      this->TextProperty)
    {
    this->StylesCheckButtonSet->GetCheckButtonSet()->SetButtonState(
      VTK_KW_TEXT_PROP_SHADOW_ID, this->TextProperty->GetShadow());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::ShadowCallback() 
{
  if (this->IsCreated())
    {
    this->SetShadow(this->StylesCheckButtonSet->GetCheckButtonSet()
                    ->IsButtonSelected(VTK_KW_TEXT_PROP_SHADOW_ID));
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowOpacity(int _arg)
{
  if (this->ShowOpacity == _arg)
    {
    return;
    }
  this->ShowOpacity = _arg;
  this->Modified();

  this->UpdateOpacityScale();
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetOpacityNoTrace(float v) 
{
  if (this->GetOpacity() == v)
    {
    return;
    }

  if (this->TextProperty)
    {
    this->TextProperty->SetOpacity(v);
    }
  else if (this->Actor2D && this->Actor2D->GetProperty())
    {
    this->Actor2D->GetProperty()->SetOpacity(v);
    }

  this->UpdateOpacityScale();

  if (this->OnChangeCommand)
    {
    this->Script("eval %s", this->OnChangeCommand);
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetOpacity(float v) 
{
  this->SetOpacityNoTrace(v);
  this->AddTraceEntry("$kw(%s) SetOpacity %f", this->GetTclName(), v);
}

// ----------------------------------------------------------------------------
float vtkKWTextProperty::GetOpacity() 
{
  int use_actor_opacity = 0;
  if (!this->TextProperty)
    {
    use_actor_opacity = 1;
    }
  else
    {
    // This test is done to maintain backward compatibility (see
    // vtkOpenGL...TextMapper). The default vtkTextProperty opacity is
    // -1 so that the mappers know that they have to use
    // the actor's opacity instead.
    if (this->TextProperty->GetOpacity() < 0.0)
      {
      use_actor_opacity = 1;
      }
    }

  if (use_actor_opacity && this->Actor2D && this->Actor2D->GetProperty())
    {
    return this->Actor2D->GetProperty()->GetOpacity();
    }
  else if (this->TextProperty)
    {
    return this->TextProperty->GetOpacity();
    }
  return 0.0;
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateOpacityScale()
{
  if (this->IsCreated() && 
      this->OpacityScale &&
      this->TextProperty)
    {
    this->OpacityScale->SetValue(this->GetOpacity());
    this->Script("grid %s %s",
                 (this->ShowOpacity ? "" : "remove"), 
                 this->OpacityScale->GetWidgetName());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::OpacityCallback() 
{
  if (this->OpacityScale->IsCreated())
    {
    this->SetOpacityNoTrace(this->OpacityScale->GetValue());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::OpacityEndCallback() 
{
  if (this->OpacityScale->IsCreated())
    {
    this->SetOpacity(this->OpacityScale->GetValue());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::SetShowCopy(int _arg)
{
  if (this->ShowCopy == _arg)
    {
    return;
    }
  this->ShowCopy = _arg;
  this->Modified();

  this->UpdatePushButtonSet();
}

// ----------------------------------------------------------------------------
vtkKWPushButton* vtkKWTextProperty::GetCopyButton()
{
  if (this->IsCreated())
    {
    return this->PushButtonSet->GetPushButtonSet()->GetButton(
      VTK_KW_TEXT_PROP_COPY_ID);
    }
  return NULL;
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::UpdatePushButtonSet()
{
  if (this->IsCreated() && this->PushButtonSet)
    {
    this->PushButtonSet->GetPushButtonSet()->SetButtonVisibility(
      VTK_KW_TEXT_PROP_COPY_ID, this->ShowCopy);

    this->Script("grid %s %s",
                 (this->PushButtonSet->GetPushButtonSet()
                  ->GetNumberOfVisibleButtons() ? "" : "remove"), 
                 this->PushButtonSet->GetWidgetName());
    }
}

// ----------------------------------------------------------------------------
void vtkKWTextProperty::CopyValuesFrom(vtkKWTextProperty *widget)
{
  if (widget)
    {
    vtkTextProperty *tprop = widget->GetTextProperty();
    if (tprop)
      {
      this->SetColor(widget->GetColor());
      this->SetFontFamily(tprop->GetFontFamily());
      this->SetBold(tprop->GetBold());
      this->SetItalic(tprop->GetItalic());
      this->SetShadow(tprop->GetShadow());
      this->SetOpacity(widget->GetOpacity());
      }
    }
}

//----------------------------------------------------------------------------
void vtkKWTextProperty::SaveInTclScript(ofstream *file, 
                                        const char *tcl_name)
{
  static int instance_count = 0;

  const char *name;
  char buffer[128];
  if (tcl_name)
    {
    name = tcl_name;
    }
  else
    {
    sprintf(buffer, "TextProperty%d", instance_count++);
    name = buffer;
    *file << "vtkTextProperty " << name << endl;
    }

  vtkTextProperty *tprop = this->TextProperty;
  if (tprop)
    {
    float *rgb = this->GetColor();
    *file << "\t" << name << " SetColor " 
          << rgb[0] << " "  << rgb[1] << " "  << rgb[2] << endl;
    *file << "\t" << name << " SetFontFamily " 
          << tprop->GetFontFamily() << endl;
    *file << "\t" << name << " SetBold " << tprop->GetBold() << endl;
    *file << "\t" << name << " SetItalic " << tprop->GetItalic() << endl;
    *file << "\t" << name << " SetShadow " << tprop->GetShadow() << endl;
    *file << "\t" << name << " SetOpacity " << this->GetOpacity() << endl;
    }
}

//----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateEnableState()
{
  this->Superclass::UpdateEnableState();

  if (this->Label)
    {
    this->Label->SetEnabled(this->Enabled);
    }

  if (this->ChangeColorButton)
    {
    this->ChangeColorButton->SetEnabled(this->Enabled);
    }

  if (this->FontFamilyOptionMenu)
    {
    this->FontFamilyOptionMenu->SetEnabled(this->Enabled);
    }

  if (this->StylesCheckButtonSet)
    {
    this->StylesCheckButtonSet->SetEnabled(this->Enabled);
    }

  if (this->OpacityScale)
    {
    this->OpacityScale->SetEnabled(this->Enabled);
    }
}

//----------------------------------------------------------------------------
void vtkKWTextProperty::FillEvent(vtkKWTextProperty::EventStruct *event)
{
  if (!event || !this->IsCreated() || !this->GetTextProperty())
    {
    return;
    }

  vtkTextProperty *tprop = this->GetTextProperty();

  float *color = this->GetColor();
  event->Color[0] = color[0];
  event->Color[1] = color[1];
  event->Color[2] = color[2];

  event->FontFamily = tprop->GetFontFamily();
  event->Bold = tprop->GetBold();
  event->Italic = tprop->GetItalic();
  event->Shadow = tprop->GetShadow();
  event->Opacity = tprop->GetOpacity();
}

//----------------------------------------------------------------------------
void vtkKWTextProperty::UpdateTextPropertyAccordingToEvent(
  vtkTextProperty *tprop,
  vtkKWTextProperty::EventStruct *event)
{
  if (!tprop || !event)
    {
    return;
    }

  tprop->SetColor(event->Color);
  tprop->SetFontFamily(event->FontFamily);
  tprop->SetBold(event->Bold);
  tprop->SetItalic(event->Italic);
  tprop->SetShadow(event->Shadow);
  tprop->SetOpacity(event->Opacity);
}

//----------------------------------------------------------------------------
void vtkKWTextProperty::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  if (this->TextProperty)
    {
    os << indent << "TextProperty:\n";
    this->TextProperty->PrintSelf(os, indent.GetNextIndent());
    }
  else
    {
    os << indent << "TextProperty: None" << endl;
    }
  if (this->Actor2D)
    {
    os << indent << "Actor2D:\n";
    this->Actor2D->PrintSelf(os, indent.GetNextIndent());
    }
  else
    {
    os << indent << "Actor2D: None" << endl;
    }
  os << indent << "LongFormat: " << (this->LongFormat ? "On" : "Off") << endl;
  os << indent << "ShowLabel: " << (this->ShowLabel ? "On" : "Off") << endl;
  os << indent << "LabelOnTop: " << (this->LabelOnTop ? "On" : "Off") << endl;
  os << indent << "ShowColor: " << (this->ShowColor ? "On" : "Off") << endl;
  os << indent << "ShowFontFamily: " 
     << (this->ShowFontFamily ? "On" : "Off") << endl;
  os << indent << "ShowStyles: " 
     << (this->ShowStyles ? "On" : "Off") << endl;
  os << indent << "ShowOpacity: " 
     << (this->ShowOpacity ? "On" : "Off") << endl;
  os << indent << "ShowCopy: " << (this->ShowCopy ? "On" : "Off") << endl;
  os << indent << "OnChangeCommand: " 
     << (this->OnChangeCommand ? this->OnChangeCommand : "None") << endl;
  os << indent << "OnColorChangeCommand: " 
     << (this->OnColorChangeCommand ? this->OnColorChangeCommand : "None") << endl;
  os << indent << "Label: " << this->Label << endl;
}
