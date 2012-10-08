// 
// 
// Copyright (C) 2007 Pingtel Corp., certain elements licensed under a Contributor Agreement.  
// Contributors retain copyright to elements licensed under a Contributor Agreement.
// Licensed to the User under the LGPL license.
// 
// $$
//////////////////////////////////////////////////////////////////////////////

#ifndef SIPREDIRECTORFALLBACK_H
#define SIPREDIRECTORFALLBACK_H

// SYSTEM INCLUDES
//#include <...>

// DEFINES
//#define __USE_OLD_FALLBACKRULES_SCHEMA__  // fallbackrules.xml is assumed to be of the old format that does not 
                                            // support locations, e.g. <callerLocationMatch> and <callerLocation>.
                                            // then this flag, is defined, the fallbackrules.xml is parsed using the old
                                            // the format that predates the introduction of location-based routing.
                                            // Note:  Builds made after Revision 14092 use the fallback rules format
                                            //        meaning that this line should be commented for such builds.

// APPLICATION INCLUDES
#include "registry/RedirectPlugin.h"
#ifndef __USE_OLD_FALLBACKRULES_SCHEMA__
   #include "digitmaps/FallbackRulesUrlMapping.h"
#else
   #include "digitmaps/MappingRulesUrlMapping.h"
#endif

// MACROS
// EXTERNAL FUNCTIONS
// EXTERNAL VARIABLES
// CONSTANTS
// STRUCTS
// TYPEDEFS
// FORWARD DECLARATIONS
class LocationDB;
class UserLocationDB;

/**
 * SipRedirectorFallback is a class redirector plug-in that maps a
 * request-URI to its corresponding ordered list of gateways using
 * the request-URI as well as the caller's location as inputs.
 * 
 * That mapping is performed based on information contained in the 
 * fallbackrules.xml configuration file generated by sipXconfig. 
 */

class SipRedirectorFallback : public RedirectPlugin
{
  public:

   explicit SipRedirectorFallback(const UtlString& instanceName);

   ~SipRedirectorFallback();

   /**
    *
    * MAPPING_RULES_FILENAME - full filename of the mapping rules file
    * to load (e.g., "fallbackrules.xml")
    */
   virtual void readConfig(OsConfigDb& configDb);

   virtual OsStatus initialize(OsConfigDb& configDb,
                               int redirectorNo,
                               const UtlString& localDomainHost);

   virtual void finalize();

   virtual RedirectPlugin::LookUpStatus lookUp(
      const SipMessage& message,
      UtlString& requestString,
      Url& requestUri,
      const UtlString& method,
      ContactList& contactList,
      RequestSeqNo requestSeqNo,
      int redirectorNo,
      SipRedirectorPrivateStorage*& privateStorage,
      ErrorDescriptor& errorDescriptor);
   
   virtual const UtlString& name( void ) const;

  protected:

   // String to use in place of class name in log messages:
   // "[instance] class".
   UtlString mLogName;

   /**
    * Set to OS_SUCCESS once the file of mapping rules is loaded into memory.
    */
   OsStatus mMappingRulesLoaded;

   /**
    * The mapping rules parsed from the file.
    */
#ifndef __USE_OLD_FALLBACKRULES_SCHEMA__   
   FallbackRulesUrlMapping mMap;
#else
   MappingRulesUrlMapping mMap;
#endif   

   /**
    * Full name of file containing the mapping rules.
    */
   UtlString mFileName;
  
  private:
     /**
      * Method used map the asserted identity of the originator of the request 
      * to its provisioned location.
      */ 
     OsStatus
     determineCallerLocation( const SipMessage& message,   ///< request from the caller to locate  
                              UtlString& callerLocation ); ///< will hold the location if found, otherwise will be blank ("")
     
     OsStatus 
     determineCallerLocationFromCallerIpAddress( const SipMessage& message,
                                                 UtlString& callerLocation );
     
     OsStatus 
     determineCallerLocationFromProvisionedUserLocation( const SipMessage& message,
                                                         UtlString& callerLocation );
   
     LocationDB*     mpLocationDbInstance;     ///< Pointer to database that holds every location's attributes such as its name and topology information 
     UserLocationDB* mpUserLocationDbInstance; ///< Pointer to database that holds mappings between user identities and their location attribute
     
     friend class SipRedirectorFallbackTest;
};

#endif // SIPREDIRECTORFALLBACK_H
