# Microsoft Developer Studio Generated NMAKE File, Format Version 40001
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=MidiKernel - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to MidiKernel - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MidiKernel - Win32 Release" && "$(CFG)" !=\
 "MidiKernel - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "MidiKernel.mak" CFG="MidiKernel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MidiKernel - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MidiKernel - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "MidiKernel - Win32 Debug"
CPP=cl.exe
MTL=mktyplib.exe
RSC=rc.exe

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\MidiKernel.exe" "$(OUTDIR)\MidiKernel.pch"

CLEAN : 
	-@erase ".\Release\MidiKernel.pch"
	-@erase ".\Release\MidiKernel.exe"
	-@erase ".\Release\pOpcode.obj"
	-@erase ".\Release\pNote.obj"
	-@erase ".\Release\pHarm.obj"
	-@erase ".\Release\pAddress.obj"
	-@erase ".\Release\pScript.obj"
	-@erase ".\Release\pThread.obj"
	-@erase ".\Release\StdAfx.obj"
	-@erase ".\Release\pFile.obj"
	-@erase ".\Release\pKernel.obj"
	-@erase ".\Release\pCompiler.obj"
	-@erase ".\Release\pEvent.obj"
	-@erase ".\Release\MidiKernelDlg.obj"
	-@erase ".\Release\pLineBuffer.obj"
	-@erase ".\Release\MidiKernel.obj"
	-@erase ".\Release\pScale.obj"
	-@erase ".\Release\MidiKernel.res"
	-@erase ".\Release\pMidiEvent.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /c
# SUBTRACT CPP /YX /Yc /Yu
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MidiKernel.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MidiKernel.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/MidiKernel.pdb" /machine:I386 /out:"$(OUTDIR)/MidiKernel.exe" 
LINK32_OBJS= \
	"$(INTDIR)/pOpcode.obj" \
	"$(INTDIR)/pNote.obj" \
	"$(INTDIR)/pHarm.obj" \
	"$(INTDIR)/pAddress.obj" \
	"$(INTDIR)/pScript.obj" \
	"$(INTDIR)/pThread.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/pFile.obj" \
	"$(INTDIR)/pKernel.obj" \
	"$(INTDIR)/pCompiler.obj" \
	"$(INTDIR)/pEvent.obj" \
	"$(INTDIR)/MidiKernelDlg.obj" \
	"$(INTDIR)/pLineBuffer.obj" \
	"$(INTDIR)/MidiKernel.obj" \
	"$(INTDIR)/pScale.obj" \
	"$(INTDIR)/pMidiEvent.obj" \
	"$(INTDIR)/MidiKernel.res"

"$(OUTDIR)\MidiKernel.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\MidiKernel.exe" "$(OUTDIR)\MidiKernel.pch"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\MidiKernel.pch"
	-@erase ".\Debug\MidiKernel.exe"
	-@erase ".\Debug\pKernel.obj"
	-@erase ".\Debug\MidiKernel.obj"
	-@erase ".\Debug\pLineBuffer.obj"
	-@erase ".\Debug\pFile.obj"
	-@erase ".\Debug\pScale.obj"
	-@erase ".\Debug\pEvent.obj"
	-@erase ".\Debug\MidiKernelDlg.obj"
	-@erase ".\Debug\pCompiler.obj"
	-@erase ".\Debug\pAddress.obj"
	-@erase ".\Debug\StdAfx.obj"
	-@erase ".\Debug\pOpcode.obj"
	-@erase ".\Debug\pNote.obj"
	-@erase ".\Debug\pHarm.obj"
	-@erase ".\Debug\pScript.obj"
	-@erase ".\Debug\pThread.obj"
	-@erase ".\Debug\MidiKernel.res"
	-@erase ".\Debug\pMidiEvent.obj"
	-@erase ".\Debug\MidiKernel.ilk"
	-@erase ".\Debug\MidiKernel.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /c
# SUBTRACT CPP /YX /Yc /Yu
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MidiKernel.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MidiKernel.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/MidiKernel.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/MidiKernel.exe" 
LINK32_OBJS= \
	"$(INTDIR)/pKernel.obj" \
	"$(INTDIR)/MidiKernel.obj" \
	"$(INTDIR)/pLineBuffer.obj" \
	"$(INTDIR)/pFile.obj" \
	"$(INTDIR)/pScale.obj" \
	"$(INTDIR)/pEvent.obj" \
	"$(INTDIR)/MidiKernelDlg.obj" \
	"$(INTDIR)/pCompiler.obj" \
	"$(INTDIR)/pAddress.obj" \
	"$(INTDIR)/StdAfx.obj" \
	"$(INTDIR)/pOpcode.obj" \
	"$(INTDIR)/pNote.obj" \
	"$(INTDIR)/pHarm.obj" \
	"$(INTDIR)/pScript.obj" \
	"$(INTDIR)/pThread.obj" \
	"$(INTDIR)/pMidiEvent.obj" \
	"$(INTDIR)/MidiKernel.res"

"$(OUTDIR)\MidiKernel.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "MidiKernel - Win32 Release"
# Name "MidiKernel - Win32 Debug"

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MidiKernel.cpp

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

DEP_CPP_MIDIK=\
	".\StdAfx.h"\
	".\MidiKernel.h"\
	".\MidiKernelDlg.h"\
	".\..\include\pKernel.hpp"\
	

"$(INTDIR)\MidiKernel.obj" : $(SOURCE) $(DEP_CPP_MIDIK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

DEP_CPP_MIDIK=\
	".\StdAfx.h"\
	".\MidiKernel.h"\
	".\MidiKernelDlg.h"\
	".\..\include\pKernel.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pThread.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	".\..\include\pNote.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pScript.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pFile.h"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	

"$(INTDIR)\MidiKernel.obj" : $(SOURCE) $(DEP_CPP_MIDIK) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MidiKernelDlg.cpp

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

DEP_CPP_MIDIKE=\
	".\StdAfx.h"\
	".\MidiKernel.h"\
	".\MidiKernelDlg.h"\
	".\pMidiEvent.h"\
	".\..\include\pKernel.hpp"\
	".\..\include\pNote.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pSystem.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	
NODEP_CPP_MIDIKE=\
	".\;"\
	

"$(INTDIR)\MidiKernelDlg.obj" : $(SOURCE) $(DEP_CPP_MIDIKE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

DEP_CPP_MIDIKE=\
	".\StdAfx.h"\
	".\MidiKernel.h"\
	".\MidiKernelDlg.h"\
	".\pMidiEvent.h"\
	".\..\include\pKernel.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pThread.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	".\..\include\pNote.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pScript.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pFile.h"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	

"$(INTDIR)\MidiKernelDlg.obj" : $(SOURCE) $(DEP_CPP_MIDIKE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MidiKernel.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MidiKernel.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MidiKernel.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MidiKernel.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MidiKernel.rc
DEP_RSC_MIDIKER=\
	".\res\MidiKernel.ico"\
	".\res\MidiKernel.rc2"\
	

"$(INTDIR)\MidiKernel.res" : $(SOURCE) $(DEP_RSC_MIDIKER) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pAddress.cpp
DEP_CPP_PADDR=\
	".\..\include\pAddress.hpp"\
	".\..\include\pSystem.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pAddress.obj" : $(SOURCE) $(DEP_CPP_PADDR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pCompiler.cpp
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


# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pEvent.cpp
DEP_CPP_PEVEN=\
	".\..\include\pEvent.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pEvent.obj" : $(SOURCE) $(DEP_CPP_PEVEN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


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

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

DEP_CPP_PHARM=\
	".\..\include\pHarm.hpp"\
	".\..\include\pKernel.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	".\..\include\pNote.hpp"\
	".\..\include\pScale.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pHarm.obj" : $(SOURCE) $(DEP_CPP_PHARM) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

DEP_CPP_PHARM=\
	".\..\include\pHarm.hpp"\
	".\..\include\pKernel.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	".\..\include\pNote.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pThread.hpp"\
	".\..\include\pScript.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pHarm.obj" : $(SOURCE) $(DEP_CPP_PHARM) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pKernel.cpp

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

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
	

"$(INTDIR)\pKernel.obj" : $(SOURCE) $(DEP_CPP_PKERN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

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
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pKernel.obj" : $(SOURCE) $(DEP_CPP_PKERN) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pLineBuffer.cpp

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

DEP_CPP_PLINE=\
	".\..\include\pLineBuffer.hpp"\
	".\..\include\pSystem.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pLineBuffer.obj" : $(SOURCE) $(DEP_CPP_PLINE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

DEP_CPP_PLINE=\
	".\..\include\pLineBuffer.hpp"\
	

"$(INTDIR)\pLineBuffer.obj" : $(SOURCE) $(DEP_CPP_PLINE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pNote.cpp

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

DEP_CPP_PNOTE=\
	".\..\include\pNote.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	".\..\include\pKernel.hpp"\
	".\..\include\pScale.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pNote.obj" : $(SOURCE) $(DEP_CPP_PNOTE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

DEP_CPP_PNOTE=\
	".\..\include\pNote.hpp"\
	".\..\include\pSystem.h"\
	".\..\include\pFile.h"\
	".\..\include\pKernel.hpp"\
	".\..\include\pScale.hpp"\
	{$(INCLUDE)}"\sys\Types.h"\
	".\..\include\pThread.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pScript.hpp"\
	".\..\include\pCompiler.hpp"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	

"$(INTDIR)\pNote.obj" : $(SOURCE) $(DEP_CPP_PNOTE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

DEP_CPP_PSCRI=\
	".\..\include\pScript.hpp"\
	".\..\include\pKernel.hpp"\
	".\..\include\pHarm.hpp"\
	".\..\include\pNote.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pSystem.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pScript.obj" : $(SOURCE) $(DEP_CPP_PSCRI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

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
	".\..\include\pCompiler.hpp"\
	".\..\include\pOpcode.hpp"\
	".\..\include\pLineBuffer.hpp"\
	

"$(INTDIR)\pScript.obj" : $(SOURCE) $(DEP_CPP_PSCRI) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Users\Kyle\Thesis\src\pThread.cpp

!IF  "$(CFG)" == "MidiKernel - Win32 Release"

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
	

"$(INTDIR)\pThread.obj" : $(SOURCE) $(DEP_CPP_PTHRE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MidiKernel - Win32 Debug"

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
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pThread.obj" : $(SOURCE) $(DEP_CPP_PTHRE) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pMidiEvent.cpp
DEP_CPP_PMIDI=\
	".\pMidiEvent.h"\
	".\..\include\pNote.hpp"\
	".\..\include\pScale.hpp"\
	".\..\include\pSystem.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

"$(INTDIR)\pMidiEvent.obj" : $(SOURCE) $(DEP_CPP_PMIDI) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
