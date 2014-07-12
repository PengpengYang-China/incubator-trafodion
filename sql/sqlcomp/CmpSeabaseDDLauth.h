/**********************************************************************
// @@@ START COPYRIGHT @@@
//
// (C) Copyright 2013-2014 Hewlett-Packard Development Company, L.P.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
// @@@ END COPYRIGHT @@@
**********************************************************************/

#ifndef _CMP_SEABASE_DDL_AUTH_H_
#define _CMP_SEABASE_DDL_AUTH_H_

// *****************************************************************************
// *
// * File:         CmpSeabaseDDLauth.h
// * Description:  Describes the DDL classes for Trafodion user management
// *               
// * Contents:
// *   class CmpSeabaseDDLauth   
// *   class CmpSeabaseDDLuser
// *
// *****************************************************************************

#include "ComSmallDefs.h"

class StmtDDLRegisterUser;
class StmtDDLAlterUser;
class NAString;

// ----------------------------------------------------------------------------
// class:  CmpSeabaseDDLauth
//
// User management class defining commonality between all authorization IDs
// Authorization IDs consist of users, PUBLIC(TDB), roles(TBD), and groups(TBD)
// ----------------------------------------------------------------------------

class CmpSeabaseDDLauth  
{

   public:

     enum AuthStatus { STATUS_UNKNOWN   = 10,
                       STATUS_GOOD      = 11,
                       STATUS_WARNING   = 12,
                       STATUS_NOTFOUND  = 13,
                       STATUS_ERROR     = 14 };

     CmpSeabaseDDLauth ();

     AuthStatus   getAuthDetails (const char *pAuthName, 
                                    bool isExternal = false);
     AuthStatus   getAuthDetails (Int32 authID);
     bool         authExists     (const NAString &authName, 
                                    bool isExternal = false);
     virtual bool describe       (const NAString &authName, 
                                    NAString &authText) = 0;

     // accessors
     Int32          getAuthCreator() const    { return authCreator_; }
     Int64          getAuthCreateTime() const { return authCreateTime_;}
     const NAString getAuthDbName() const     { return authDbName_; }
     const NAString getAuthExtName() const    { return authExtName_; }
     Int32          getAuthID() const         { return authID_; }
     Int64          getAuthRedefTime() const  { return authRedefTime_; }
     ComIdClass     getAuthType() const       { return authType_; }

     bool  isAuthImmutable() const { return authImmutable_; }
     bool  isAuthValid() const     { return authValid_; }
     bool  isPublic() const        { return false; }
     bool  isRole()   const        { return authType_ == COM_ROLE_CLASS; }
     bool  isUser()   const        { return authType_ == COM_USER_CLASS; }

 protected:

    bool isAuthNameReserved (const NAString &authName);
    bool isAuthNameValid    (const NAString &authName);
    void verifyAuthority    (void);

    virtual Int32 getUniqueID (void) = 0;

    // mutators
    void setAuthCreator      (const Int32 authCreator)
      {authCreator_ = authCreator;}
    void setAuthCreateTime   (const Int64 authCreateTime)
      { authCreateTime_ = authCreateTime;}
    void setAuthDbName       (const NAString &authDbName)
      {authDbName_=authDbName;}
    void setAuthExtName      (const NAString &authExtName)
      {authExtName_=authExtName;}
    void setAuthID           (const Int32 authID)
      {authID_ = authID;}
    void setAuthImmutable    (bool isImmutable)
       {authImmutable_ = isImmutable;}
     void setAuthRedefTime    (const Int64 authRedefTime)
       { authRedefTime_ = authRedefTime;}
     void setAuthType        (ComIdClass authType)
       {authType_ = authType;}
     void setAuthValid       (bool isValid)
       {authValid_ = isValid;}

    // metadata access methods
    void deleteRow      (const NAString &authName);
    void insertRow      (void);
    void updateRow      (const NAString & setClause);
    AuthStatus selectExactRow (const NAString &cmd); 
    Int64      selectCount    (const NAString & whereClause);
    Int32      selectMaxAuthID(const NAString &whereClause);

 private:

    Int32             authCreator_;
    Int64             authCreateTime_;
    NAString          authDbName_;
    NAString          authExtName_;
    Int32             authID_;
    bool              authImmutable_;
    Int64             authRedefTime_;
    ComIdClass        authType_;
    bool              authValid_;

};

// ----------------------------------------------------------------------------
// class:  CmpSeabaseDDLuser
//
// Class that manages user authorization IDs
//
// Child class of CmpSeabaseDDLauth
// ----------------------------------------------------------------------------
class CmpSeabaseDDLuser : public CmpSeabaseDDLauth
{
   public:

     CmpSeabaseDDLuser ();

     // Execute level methods
     void alterUser(StmtDDLAlterUser * pNode);
     void registerUser(StmtDDLRegisterUser * pNode);
     void unregisterUser(StmtDDLRegisterUser * pNode);
     
     CmpSeabaseDDLauth::AuthStatus getUserDetails(const char *pUserName, 
                                                    bool isExternal = false);
     CmpSeabaseDDLauth::AuthStatus getUserDetails(Int32 userID);

     bool describe (const NAString &authName, NAString &authText);

   protected:

     Int32 getUniqueID (void);
};
#endif // _CMP_SEABASE_DDL_AUTH_H_