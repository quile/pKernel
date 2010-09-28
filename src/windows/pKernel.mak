# Microsoft Developer Studio Generated NMAKE File, Format Version 40001
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=pKernel - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to pKernel - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "pKernel - Win32 Release" && "$(CFG)" !=\
 "pKernel - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "pKernel.mak" CFG="pKernel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pKernel - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "pKernel - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "pKernel - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pKernel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\pKernel.exe"

CLEAN : 
	-@erase ".\Release\pKernel.exe"
	-@erase ".\Release\pOpcode.obj"
	-@erase ".\Release\pCompiler.obj"
	-@erase ".\Release\pFile.obj"
	-@erase ".\Release\pAddress.obj"
	-@erase ".\Release\pThread.obj"
	-@erase ".\Release\pNote.obj"
	-@erase ".\Release\pScale.obj"
	-@erase ".\Release\pEvent.obj"
	-@erase ".\Release\pLineBuffer.obj"
	-@erase ".\Release\pTest.obj"
	-@erase ".\Release\pScript.obj"
	-@erase ".\Release\pHarm.obj"
	-@erase ".\Release\pKernel.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/pKernel.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/pKernel.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/pKernel.pdb" /machine:I386 /out:"$(OUTDIR)/pKernel.exe" 
LINK32_OBJS= \
	"$(INTDIR)/pOpcode.obj" \
	"$(INTDIR)/pCompiler.obj" \
	"$(INTDIR)/pFile.obj" \
	"$(INTDIR)/pAddress.obj" \
	"$(INTDIR)/pThread.obj" \
	"$(INTDIR)/pNote.obj" \
	"$(INTDIR)/pScale.obj" \
	"$(INTDIR)/pEvent.obj" \
	"$(INTDIR)/pLineBuffer.obj" \
	"$(INTDIR)/pTest.obj" \
	"$(INTDIR)/pScript.obj" \
	"$(INTDIR)/pHarm.obj" \
	"$(INTDIR)/pKernel.obj"

"$(OUTDIR)\pKernel.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pKernel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\pKernel.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\pKernel.exe"
	-@erase ".\Debug\pAddress.obj"
	-@erase ".\Debug\pHarm.obj"
	-@erase ".\Debug\pCompiler.obj"
	-@erase ".\Debug\pFile.obj"
	-@erase ".\Debug\pScale.obj"
	-@erase ".\Debug\pEvent.obj"
	-@erase ".\Debug\pNote.obj"
	-@erase ".\Debug\pScript.obj"
	-@erase ".\Debug\pThread.obj"
	-@erase ".\Debug\pLineBuffer.obj"
	-@erase ".\Debug\pKernel.obj"
	-@erase ".\Debug\pOpcode.obj"
	-@erase ".\Debug\pTest.obj"
	-@erase ".\Debug\pKernel.ilk"
	-@erase ".\Debug\pKernel.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/pKernel.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/pKernel.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/pKernel.pdb" /debug /machine:I386 /out:"$(OUTDIR)/pKernel.exe" 
LINK32_OBJS= \
	"$(INTDIR)/pAddress.obj" \
	"$(INTDIR)/pHarm.obj" \
	"$(INTDIR)/pCompiler.obj" \
	"$(INTDIR)/pFile.obj" \
	"$(INTDIR)/pScale.obj" \
	"$(INTDIR)/pEvent.obj" \
	"$(INTDIR)/pNote.obj" \
	"$(INTDIR)/pScript.obj" \
	"$(INTDIR)/pThread.obj" \
	"$(INTDIR)/pLineBuffer.obj" \
	"$(INTDIR)/pKernel.obj" \
	"$(INTDIR)/pOpcode.obj" \
	"$(INTDIR)/pTest.obj"

"$(OUTDIR)\pKernel.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "pKernel - Win32 Release"
# Name "pKernel - Win32 Debug"

!IF  "$(CFG)" == "pKernel - Win32 Release"

!ELSEIF  "$(CFG)" == "pKernel - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pAddress.cpp

!IF  "$(CFG)" == "pKernel - Win32 Release"

DEP_CPP_PADDR=\
	".\..\include\pAddress.hpp"\
	".\..\include\pSystem.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pAddress.obj" : $(SOURCE) $(DEP_CPP_PADDR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "pKernel - Win32 Debug"

DEP_CPP_PADDR=\
	".\..\include\pAddress.hpp"\
	

"$(INTDIR)\pAddress.obj" : $(SOURCE) $(DEP_CPP_PADDR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pCompiler.cpp

!IF  "$(CFG)" == "pKernel - Win32 Release"

DEP_CPP_PCOMP=\
	".\..\include\pScript.hpp"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pAddress.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pNote.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pCompiler.obj" : $(SOURCE) $(DEP_CPP_PCOMP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "pKernel - Win32 Debug"

DEP_CPP_PCOMP=\
	".\..\include\pScript.hpp"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pAddress.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pSystem.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	".\..\include\pFile.h"\
	

"$(INTDIR)\pCompiler.obj" : $(SOURCE) $(DEP_CPP_PCOMP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pEvent.cpp

!IF  "$(CFG)" == "pKernel - Win32 Release"

DEP_CPP_PEVEN=\
	".\..\include\pEvent.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pEvent.obj" : $(SOURCE) $(DEP_CPP_PEVEN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "pKernel - Win32 Debug"

DEP_CPP_PEVEN=\
	".\..\include\pEvent.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	
NODEP_CPP_PEVEN=\
	".\..\src\dump"\
	

"$(INTDIR)\pEvent.obj" : $(SOURCE) $(DEP_CPP_PEVEN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pFile.c
DEP_CPP_PFILE=\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pFile.obj" : $(SOURCE) $(DEP_CPP_PFILE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pHarm.cpp
DEP_CPP_PHARM=\
	".\..\include\pHarm.hpp"\
	".\..\include\pKernel.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	".\..\include\pNote.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pThread.hpp"\
	".\..\include\pEvent.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pScript.hpp"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pAddress.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pHarm.obj" : $(SOURCE) $(DEP_CPP_PHARM) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pKernel.cpp
DEP_CPP_PKERN=\
	".\..\include\pThread.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	".\..\include\pKernel.hpp"\
	".\..\include\pNote.hpp"\
	".\..\include\pScript.hpp"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pAddress.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	".\..\include\pEvent.hpp"\
	

"$(INTDIR)\pKernel.obj" : $(SOURCE) $(DEP_CPP_PKERN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pLineBuffer.cpp
DEP_CPP_PLINE=\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pSystem.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pLineBuffer.obj" : $(SOURCE) $(DEP_CPP_PLINE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pNote.cpp
DEP_CPP_PNOTE=\
	".\..\include\pNote.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	".\..\include\pKernel.hpp"\
	".\..\include\pScale.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	".\..\include\pThread.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pEvent.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pScript.hpp"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pAddress.hpp"\
	

"$(INTDIR)\pNote.obj" : $(SOURCE) $(DEP_CPP_PNOTE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pOpcode.cpp
DEP_CPP_POPCO=\
	".\..\include\pOpcode.hpp"\
	

"$(INTDIR)\pOpcode.obj" : $(SOURCE) $(DEP_CPP_POPCO) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pScale.cpp
DEP_CPP_PSCAL=\
	".\..\include\pScale.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pScale.obj" : $(SOURCE) $(DEP_CPP_PSCAL) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pScript.cpp
DEP_CPP_PSCRI=\
	".\..\include\pScript.hpp"\
	".\..\include\pKernel.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pNote.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	".\..\include\pThread.hpp"\
	".\..\include\pEvent.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pAddress.hpp"\
	

"$(INTDIR)\pScript.obj" : $(SOURCE) $(DEP_CPP_PSCRI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pTest.cpp

!IF  "$(CFG)" == "pKernel - Win32 Release"

DEP_CPP_PTEST=\
	".\..\include\pKernel.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pThread.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pNote.hpp"\
	".\..\include\pEvent.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pScript.hpp"\
	".\..\include\pFile.h"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pAddress.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pTest.obj" : $(SOURCE) $(DEP_CPP_PTEST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "pKernel - Win32 Debug"

DEP_CPP_PTEST=\
	".\..\include\pKernel.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pThread.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pNote.hpp"\
	".\..\include\pEvent.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pScript.hpp"\
	".\..\include\pFile.h"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pAddress.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	
NODEP_CPP_PTEST=\
	".\..\..\..\..\\---------------------------------------"\
	

"$(INTDIR)\pTest.obj" : $(SOURCE) $(DEP_CPP_PTEST) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pThread.cpp
DEP_CPP_PTHRE=\
	".\..\include\pNote.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pScript.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	".\..\include\pKernel.hpp"\
	".\..\include\pThread.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pAddress.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	".\..\include\pEvent.hpp"\
	

"$(INTDIR)\pThread.obj" : $(SOURCE) $(DEP_CPP_PTHRE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
