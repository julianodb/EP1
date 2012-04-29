# Microsoft Developer Studio Project File - Name="USPDesigVC6" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=USPDesigVC6 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "USPDesigVC6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "USPDesigVC6.mak" CFG="USPDesigVC6 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "USPDesigVC6 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "USPDesigVC6 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "USPDesigVC6 - Win32 Release"
# Name "USPDesigVC6 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\ANA\anaaproj.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anaarea_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anacaren.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anacatot.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anacentg.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anacgtot.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anadensa.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anadensi.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anadflui.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anaemtot.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anaintot.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anamassa.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anaminrc.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anamtot_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anapesot.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anashow_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anavftot.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anavolum.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANA\anavtot_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANI\anicreat.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANI\aniexibe.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ANI\animlist.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\BasicViewer\BVThreadFun.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camaltcr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camaltpr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camcheck.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camcopy_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camgetpr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camlist_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\cammanip.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camnew__.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camremov.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camrenam.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camsetcr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CAM\camtrncr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\BasicViewer\cBasicViewer.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curaddpt.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curaux__.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curdelem.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\cureline.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curepoly.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curfinde.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curkline.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curkpoly.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curmline.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curmnewe.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curmpoly.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curpleng.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\currempt.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\cursettg.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\CUR\curtest1.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\disbreak.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\discamer.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\discaxis.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\disccolr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\discdisp.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\discfile.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\disclip3.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\disedge_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\dishediv.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\dishevis.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\dishidde.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\dishinfc.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\dishintr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\dishsore.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\dishvisb.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\disledge.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\disltrns.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DIS\dismonit.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulcolor.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\euldoinv.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulerops.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulkef__.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulkemr_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulkev__.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulkfmrh.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulksfmr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulkvsf_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulmef__.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulmekr_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulmev__.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulmfkrh.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulmodif.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulmsfkr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulmvsf_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulnome_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulprint.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulread_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulscoef.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\EUL\eulxundo.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genaxsas.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genaxsds.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\gencopy_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\gendelet.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\gendir__.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genfadjr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genfindn.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\gengrpas.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\gengrpds.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genlist_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genload_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genncopy.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genrenam.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genrotat.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\gensave_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genshow_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\gentrans.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GEN\genwait_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GPH\gphclear.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GPH\gphlintf.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GPH\gphprint.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\GPH\gphwindw.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieassag.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieassas.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieassgg.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieassgs.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieaxscd.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieaxscr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieaxsdf.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieaxsli.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieaxsmn.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieaxsmv.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieaxsrm.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieaxsrn.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieaxsrt.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hieaxstr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiedesag.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiedesas.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiedesgg.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiedesgs.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrpcr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrpcs.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrpcy.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrpdf.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrpli.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrpmn.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrpmv.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrprg.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrprm.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrprn.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrprt.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrpsc.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\HIE\hiegrptf.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowaddel.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowaddhe.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowboxop.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowcheck.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowfaceq.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowfinde.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowgeome.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowinfos.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowlists.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lownewel.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowtrafn.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowtrans.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowtrrot.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowtrrtg.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowtrsca.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\LOW\lowtrtra.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MAN\manhelp_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MAN\manlinin.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MAN\manmainp.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MAN\manstrmp.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MAN\mantrace.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MEM\memsolid.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\MEM\memvirtu.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\PROP.CPP

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\RN1\rndaux__.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\RN1\rndfase1.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\RN1\rndfase2.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\RN1\rndfase3.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\RN1\rndround.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovcirc_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovcone_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovcubo_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovdist_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovdmprc.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovesfer.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovorden.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovperi_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovperl_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovpert_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovplaca.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovprism.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovpyram.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovquad_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovretan.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovtrian.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovtrnds.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\ROV\rovtrntr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setdiffe.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setinter.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setomain.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setopcls.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setopcon.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setopfin.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setopgen.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setopget.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setsinsn.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setsrece.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setsrecs.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\settrace.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SET\setunion.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shpcylin.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shpgeoms.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shpmpris.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shpmrevo.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shppris0.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shpprism.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shppyram.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shprevol.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shpspher.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shpsweep.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shptorus.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SHP\shpxcube.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SPL\splmain_.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SPL\splstep1.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SPL\splstep2.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SPL\splstep3.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SPL\splstep4.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\SPL\splvoron.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\TOP\topclean.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\TOP\topdetac.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\TOP\toplglue.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\TOP\topmerge.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\TOP\topmodif.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\TOP\topnewlc.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\TOP\topremhr.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\TOP\topremsf.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\TOP\toprever.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\TOP\topringm.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\VEC\vectorop.cpp

!IF  "$(CFG)" == "USPDesigVC6 - Win32 Release"

# ADD CPP /MT /D "__ROV__" /D "__curved_env__" /D "_Windows"

!ELSEIF  "$(CFG)" == "USPDesigVC6 - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\INC\analise_.h
# End Source File
# Begin Source File

SOURCE=..\INC\animatio.h
# End Source File
# Begin Source File

SOURCE=..\INC\basrov__.h
# End Source File
# Begin Source File

SOURCE=..\BasicViewer\cBasicViewer.h
# End Source File
# Begin Source File

SOURCE=..\INC\curvgeom.h
# End Source File
# Begin Source File

SOURCE=..\INC\disdispl.h
# End Source File
# Begin Source File

SOURCE=..\INC\eulerops.h
# End Source File
# Begin Source File

SOURCE=..\INC\genfunc_.h
# End Source File
# Begin Source File

SOURCE=..\INC\gphgraph.h
# End Source File
# Begin Source File

SOURCE=..\INC\hiegroup.h
# End Source File
# Begin Source File

SOURCE=..\INC\lowgeome.h
# End Source File
# Begin Source File

SOURCE=..\INC\lowmacro.h
# End Source File
# Begin Source File

SOURCE=..\INC\lowparam.h
# End Source File
# Begin Source File

SOURCE=..\INC\lowsolid.h
# End Source File
# Begin Source File

SOURCE=..\INC\mancommd.h
# End Source File
# Begin Source File

SOURCE=..\INC\memvirtu.h
# End Source File
# Begin Source File

SOURCE=..\INC\mensagem.h
# End Source File
# Begin Source File

SOURCE=..\INC\prop.h
# End Source File
# Begin Source File

SOURCE=..\INC\rndround.h
# End Source File
# Begin Source File

SOURCE=..\INC\sethmain.h
# End Source File
# Begin Source File

SOURCE=..\INC\shpshape.h
# End Source File
# Begin Source File

SOURCE=..\INC\splsplit.h
# End Source File
# Begin Source File

SOURCE=..\INC\vectorop.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
