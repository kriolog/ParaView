/*=========================================================================

  Program:   ParaView
  Module:    vtkSMStateLoader.h

  Copyright (c) Kitware, Inc.
  All rights reserved.
  See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSMStateLoader - Utility class to load state from XML
// .SECTION Description
// vtkSMStateLoader can load server manager state from a given 
// vtkPVXMLElement. This element is usually populated by a vtkPVXMLParser.
// .SECTION See Also
// vtkPVXMLParser vtkPVXMLElement

#ifndef vtkSMStateLoader_h
#define vtkSMStateLoader_h

#include "vtkPVServerManagerCoreModule.h" //needed for exports
#include "vtkSMDeserializerXML.h"

#include <map> // needed for API
#include <string> // needed for API

class vtkPVXMLElement;
class vtkSMProxy;
class vtkSMProxyLocator;

struct vtkSMStateLoaderInternals;

class VTKPVSERVERMANAGERCORE_EXPORT vtkSMStateLoader : public vtkSMDeserializerXML
{
public:
  static vtkSMStateLoader* New();
  vtkTypeMacro(vtkSMStateLoader, vtkSMDeserializerXML);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Load the state from the given root element.
  int LoadState(vtkPVXMLElement* rootElement, bool keepOriginalId = false);

  // Description:
  // Get/Set the proxy locator to use. Default is 
  // vtkSMProxyLocator will be used.
  void SetProxyLocator(vtkSMProxyLocator* loc);
  vtkGetObjectMacro(ProxyLocator, vtkSMProxyLocator);

  // Description:
  // Allow the loader to keep the mapping between the id from the loaded state
  // to the current proxy attributed id.
  vtkSetMacro(KeepIdMapping, int);
  vtkGetMacro(KeepIdMapping, int);
  vtkBooleanMacro(KeepIdMapping, int);

  // Description:
  // Return an array of ids. The ids are stored in the following order
  // and the size of the array is provided as argument.
  // [key, value, key, value, ...]
  // The array is kept internaly using a std::vector
  vtkTypeUInt32* GetMappingArray(int &size);
protected:
  vtkSMStateLoader();
  ~vtkSMStateLoader();

  // Description:
  // The rootElement must be the <ServerManagerState /> xml element.
  // If rootElement is not a <ServerManagerState /> element, then we try to
  // locate the first <ServerManagerState /> nested element and load that.
  // Load the state from the given root element.
  virtual int LoadStateInternal(vtkPVXMLElement* rootElement);

  // Description:
  // Called after a new proxy is created. The main responsibility of this method
  // is to ensure that proxy gets a GlobalId (either automatically or using the
  // id from the state if LoadState() was called with \c keepOriginalId set to
  // true). It also called vtkSMProxy::UpdateVTKObjects() and
  // vtkSMProxy::UpdatePipelineInformation() (if applicable) to ensure that the
  // state loaded on the proxy is "pushed" and any info properties updated.
  // We also create a list to track the order in which proxies are created.
  // This order is a dependency order too and hence helps us register proxies in
  // order of dependencies.
  virtual void CreatedNewProxy(vtkTypeUInt32 id, vtkSMProxy* proxy);

  // Description:
  // Overridden so that when new views are to be created, we create views
  // suitable for the connection. 
  virtual vtkSMProxy* CreateProxy(const char* xmlgroup, const char* xmlname,
                                  const char* subProxyName = NULL);

  virtual int HandleProxyCollection(vtkPVXMLElement* collectionElement);
  virtual void HandleCustomProxyDefinitions(vtkPVXMLElement* element);
  int HandleLinks(vtkPVXMLElement* linksElement);
  virtual int BuildProxyCollectionInformation(vtkPVXMLElement*);

  // Description:
  // This method scans through the internal data structure built 
  // during BuildProxyCollectionInformation() and registers the proxy. 
  // The DS keeps info
  // about each proxy ID and the groups and names 
  // the proxy should be registered as (as indicated in the state file).
  virtual void RegisterProxy(vtkTypeUInt32 id, vtkSMProxy* proxy);
  virtual void RegisterProxyInternal(const char* group, 
    const char* name, vtkSMProxy* proxy);

  // Description:
  // Update a proxy's registration group and name before it gets registered.
  // Default implementation handles helper group group names.
  // Returns false to skip registering the proxy.
  // @param[in,out] group  proxy registration group
  // @param[in,out] name   proxy registration name
  // @param[in]     proxy  proxy being registered
  // @returns true to continue registering the proxy, false to skip registering it.
  virtual bool UpdateRegistrationInfo(
    std::string& group, std::string& name, vtkSMProxy* proxy);

  // Description:
  // Return the xml element for the state of the proxy with the given id.
  // This is used by NewProxy() when the proxy with the given id
  // is not located in the internal CreatedProxies map.
  virtual vtkPVXMLElement* LocateProxyElement(vtkTypeUInt32 id);

  // Description:
  // Used by LocateProxyElement(). Recursively tries to locate the
  // proxy state element for the proxy.
  vtkPVXMLElement* LocateProxyElementInternal(vtkPVXMLElement* root, vtkTypeUInt32 id);

  // Description:
  // Checks the root element for version. If failed, return false.
  virtual bool VerifyXMLVersion(vtkPVXMLElement* rootElement);

  // Description:
  // Returns the first proxy already registered using any of the registration
  // (group, name) assigned to the proxy with the given id in the state file.
  vtkSMProxy* LocateExistingProxyUsingRegistrationName(vtkTypeUInt32 id);

  vtkPVXMLElement* ServerManagerStateElement;
  vtkSMProxyLocator* ProxyLocator;
  int KeepIdMapping;
private:
  vtkSMStateLoader(const vtkSMStateLoader&) VTK_DELETE_FUNCTION;
  void operator=(const vtkSMStateLoader&) VTK_DELETE_FUNCTION;

  vtkSMStateLoaderInternals* Internal;
};

#endif
